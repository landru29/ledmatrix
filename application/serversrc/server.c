#define DEBUG

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "font.h"
#include "ledmatrix.h"
#include "animate.h"
#include "plugins.h"
#include "ini.h"
#include "display.h"
#include "constant.h"

/**
 * Display usage of program on stdout
 *
 * @param argv Parameters of program
 *
 * @return void
 */
void usage(char **argv)
{
    fprintf(stdout, "Newspaper V1.0\n M. Hervo & C. Meichel\n2013, June\n");
    fprintf(stdout, "Syntaxe : %s\n", argv[0]);
    fprintf(stdout, "\t-s:\n\t\tSimulate the matrix\n");
    fprintf(stdout, "\t-v:\n\t\tVerbose the configuration and exit\n");
}

/**
 * Construit le chemin vers le dossier des plugins
 *
 * @return chemin absolu vers les plugins
 */
char* pluginsFolder()
{
	return strdup(LIBDIR);
}

/**
 * Construit le chemin du fichi ini
 *
 * @return chemin absolu vers le fichier ini
 */
char* iniFile()
{
    char confPath[200];
    sprintf(confPath, "%s/conf.ini", CONFDIR);
    return strdup(confPath);
}

/**
 * Vérifie l'existance d'un fichier
 *
 * @param filename fichier à tester
 *
 * @return vrai si le fichier existe
 */
int file_exists (char * fileName)
{
    struct stat buf;
    int i = stat ( fileName, &buf );
    /* File found */
    if ( i == 0 ) {
        return 1;
    }
    return 0;
}

/**
 * Construit le chemin d'une font
 *
 * @return chemin absolu vers le fichier de font
 */
char* fontFile(char* basename)
{
    char confPath[200];
    sprintf(confPath, "%s/%s", FONTDIR, basename);
    return strdup(confPath);
}

/**
 * Charge la configuration depuis conf.ini
 *
 * @param displays nombre de matrices
 * @param matrixHeight hauteur de matrice en pixels
 * @param matrixWidth largeur de matrice en pixels
 *
 * @return void
 */
void loadConfiguration(unsigned int* displays, unsigned int* matrixHeight, unsigned int* matrixWidth)
{
    INI_LINE* configuration;
    int csValueTmp;
    /* read config */
    configuration = iniParse(iniFile());
    /* retrieve data from configuration */
    if (iniHasKey(configuration, "displays")) {
        sscanf(iniGet(configuration, "displays"), "%d", displays);
    } else {
        *displays = 3;
    }
    if (iniHasKey(configuration, "matrixHeight")) {
        sscanf(iniGet(configuration, "matrixHeight"), "%d", matrixHeight);
    } else {
        *matrixHeight = 8;
    }
    if (iniHasKey(configuration, "matrixWidth")) {
        sscanf(iniGet(configuration, "matrixWidth"), "%d", matrixWidth);
    } else {
        *matrixWidth = 32;
    }
    if (iniHasKey(configuration, "CS0")) {
        sscanf(iniGet(configuration, "CS0"), "%d", &csValueTmp);
        setCs(0, (unsigned char)csValueTmp);
    }
    if (iniHasKey(configuration, "CS1")) {
        sscanf(iniGet(configuration, "CS1"), "%d", &csValueTmp);
        setCs(1, (unsigned char)csValueTmp);
    }
    if (iniHasKey(configuration, "CS2")) {
        sscanf(iniGet(configuration, "CS2"), "%d", &csValueTmp);
        setCs(2, (unsigned char)csValueTmp);
    }
    if (iniHasKey(configuration, "CS3")) {
        sscanf(iniGet(configuration, "CS3"), "%d", &csValueTmp);
        setCs(3, (unsigned char)csValueTmp);
    }

    /* Release configuration */
    iniDestroy(configuration);
}

/**
 * Check if matrix must be tested and test it
 *
 * @param testMatrixIndex matrix index to test
 * @param matrixHeight matrix height in dots
 * @param matrixWidth matrix width in dots
 *
 * @return 1 if tested otherwise 0
 */
int checkMatrixToTest(int testMatrixIndex, unsigned int matrixHeight, unsigned int matrixWidth)
{
    char* dataTest;
    if (testMatrixIndex>=0) {
        fprintf(stdout, "Testing matrix %d\n", testMatrixIndex);
        dataTest = (char*) malloc(matrixWidth*matrixHeight/8);
        memset(dataTest, 0xff, matrixWidth*matrixHeight/8);
        writeScreen(testMatrixIndex, dataTest, matrixWidth*matrixHeight/8, matrixWidth, matrixHeight);
        free (dataTest);
        return 1;
    }
    return 0;
}

/**
 * Load plugins
 *
 * @return plugins collections
 */
ANIMATIONPLUGIN** loadAllPlugins()
{
    unsigned int i;
    ANIMATIONPLUGIN** plugins;
    /* Load plugins */
    plugins = loadPlugins(pluginsFolder());
	if (!plugins) {
		printf("No plugins ! Exiting.\n");
		return 0;
	}
    for(i=0; plugins[i]; i++)
		printf("%s was loaded\n", plugins[i]->name);
    return plugins;
}

/**
 * Create the file node for communication
 */
void createNode()
{
    if (file_exists(NODE_NAME)) {
        fprintf(stderr, "%s exists: erasing\n", NODE_NAME);
        unlink(NODE_NAME);
    }
    mode_t oldMask = umask(0044);
    if (mkfifo(NODE_NAME, 0646) !=0) {
        fprintf(stderr, "Could not create the FIFO node\n");
        exit(EXIT_FAILURE);
    }
    umask(oldMask);
}

/**
 * Load fonts
 *
 * @return font collections
 */
FONT** loadFonts()
{
    unsigned int len=0;
    FONT** fonts = (FONT**) malloc(sizeof(FONT*));
    fonts[0]=0;
    fprintf(stdout, "Loading fonts\n");
    len++;
    fonts = (FONT**)realloc(fonts, sizeof(FONT*) * (len+1));
    fonts[len-1] = loadFont(fontFile("basic.font"));
    fonts[len] = 0;
    return fonts;
}


/**
 * Méthode de lancement du programme
 *
 * @param argc Number of parameters
 * @param argv Parameters of program
 *
 * @return status
 */
int main(int argc, char **argv)
{
    char optstring[] = "sv";
    int fifoFd;
    FILE* fifoFile;
    int exitCondition = 1;
    char* dataBuffer;
    int option;
    LEDMATRIX* matrix = 0; // Espace mémoire pour l'écriture sur les matrices
    FONT** fonts = 0;
    unsigned int fontSelector=0;
    ANIMATION_QUEUE* animations=0;
    shared_function getFrames;
    //GIFANIMATION* gif;
    int testMatrixIndex=-1;
    unsigned int matrixHeight; // nombre de ligne par matrice
    unsigned int matrixWidth;  // nombre de colonnes par matrice
    unsigned int displays;     // nombre de matrices
    char* message=0;
    int graphic = 0;
    int simulated=
#ifdef __arm__
    0;
#else
    1;
#endif

    ANIMATIONPLUGIN** plugins;
    ANIMATIONPLUGIN* plugAnimation;
    void* userData = 0;
    unsigned int i;

    /* read config */
    loadConfiguration(&displays, &matrixHeight, &matrixWidth);

    /* Load plugins */
    plugins = loadAllPlugins();

    // read fonts
    fonts = loadFonts();

    /* Check if launched by root */
    if (getuid() !=0)
        fprintf(stdout, "You should be root to launch this program\n");

    /* Matrix initialisation */
    matrix = openLedMatrix(displays*matrixWidth, matrixHeight);

    opterr=0; /* Pas de message d'erreur automatique */

    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 's':
                simulated = 1;
                break;
            case 'v':
                fprintf(stdout, "Nb matrix: %d\n", displays);
                fprintf(stdout, "Matrix height: %d\n", matrixHeight);
                fprintf(stdout, "Matrix width: %d\n", matrixWidth);
                fprintf(stdout, "CS0: %d\nCS1: %d\nCS2: %d\nCS3: %d\n", getCs(0), getCs(1), getCs(2), getCs(3));
                abort();
                break;
            default:
                abort();
                break;
        }
    }
#ifndef __arm__
    simulated = 1;
#endif

    /* Switch on the simulator */
    if (simulated) matrixSetDebugMode(matrix, 1);

    /* Select font */
    for (i=0; fonts[i]; i++);
    fontSelector = (fontSelector<i?fontSelector:0);
    matrixSetFont(matrix, fonts[fontSelector]);

    /* Switch on the simulator */
    if (simulated) {
        matrixSetDebugMode(matrix, 1);
        matrixDebugInit();
    }

    /* Check if launched by root */
    if (getuid() !=0)
        fprintf(stdout, "You should be root to launch this program\n");

    // create the fifo node
    createNode();

    // Animation in action
    animations = createAnimationQueue();

    // getting animation from plugin
    plugAnimation = getPluginAnimation(plugins, "scrollV");
    if (plugAnimation) {
        if (plugAnimation->creation) {
            userData = (plugAnimation->creation)();
        }
        enqueueAnimation(animations, createAnimation(plugAnimation->runtime, 8, 0, 1, 150, userData, plugAnimation->destruction));
    }


    // Open the FIFO
    fifoFd = open(NODE_NAME, O_RDWR);
    fifoFile = fdopen(fifoFd, "r");

    // prepare the buffer
    dataBuffer = (char*)malloc(DATA_BUFFER_SIZE);

    // alive message
    matrixCleanModel(matrix);
    matrixPushString(matrix, "Let's go !");
    matrixSendModel(matrix);
    matrixSendViewport(matrix);

    // main loop
    while (exitCondition) {
        fgets(dataBuffer, DATA_BUFFER_SIZE, fifoFile);
        if (dataBuffer[strlen(dataBuffer)-1] == '\n')
            dataBuffer[strlen(dataBuffer)-1] = 0;
        if (strlen(dataBuffer)) {
            if (strcmp(dataBuffer, "bye")==0) {
                exitCondition = 0;
            } else {
                matrixCleanModel(matrix);
                // send the message to the matrix model
                matrixPushString(matrix, dataBuffer);
                // Animation in action
                animate(matrix, animations);
                // wait for a while
                sleep(5);
            }
        }
    }

    // release buffer
    free(dataBuffer);

    // close the FIFO
    fclose(fifoFile);

    // destroy the node
    unlink(NODE_NAME);

    // Dead message
    matrixCleanModel(matrix);
    matrixPushString(matrix, "I'm dead !");
    matrixSendModel(matrix);
    matrixSendViewport(matrix);

    // Cleaning everything
    for (i=0; fonts[i]; i++)
    destroyFont(fonts[i]);
    free(fonts);
    closeLedMatrix(matrix);
    destroyAnimationQueue(animations);
    closePlugins(plugins);

    return 0;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

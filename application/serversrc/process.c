#include <ctype.h>
#include <stdio.h>
#include <string.h>
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
#include "process.h"

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
 * Create a node file for communication client-server
 */
void createNode()
{
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
 * Select a font by index
 *
 * @param matrix       ledmatrix to assign font
 * @param fonts        fonts collection
 * @param fontSelector index of the font
 *
 * @return number of fonts
 */
int selectFont(LEDMATRIX* matrix, FONT** fonts, unsigned int fontSelector)
{
    unsigned int i;
    /* Select font */
    for (i=0; fonts[i]; i++);
    fontSelector = (fontSelector<i?fontSelector:0);
    if (fontSelector>=0) {
        matrixSetFont(matrix, fonts[fontSelector]);
    }
    return i;
}

/**
 * close all fonts and release memory
 *
 * @param fonts list of all loaded fonts
 */
void unloadFonts(FONT** fonts)
{
    unsigned int i;
    for (i=0; fonts[i]; i++)
    destroyFont(fonts[i]);
    free(fonts);
}

/**
 * Open the communication pipe
 */
FILE* openCommunicationPipe()
{
    int fifoFd;
    FILE* fifoFile;
    if (!file_exists(NODE_NAME)) {
        // Create the node if not exists
        createNode();
    }
    // Open the FIFO
    fifoFd = open(NODE_NAME, O_RDWR);
    fifoFile = fdopen(fifoFd, "r");
    return fifoFile;
}

/**
 * Close the communication pipe
 */
void closeCommunicationPipe(FILE* pipe)
{
    // close the FIFO
    fclose(pipe);
    // destroy the node
    unlink(NODE_NAME);
}

/**
 * Check if a file exists
 *
 * @param filename file to check
 *
 * @return true if exists
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
 * Send a message without any animation
 *
 * @param matrix  the matrix to present the message
 * @param message the message to display
 */
void quickMessage(LEDMATRIX* matrix, char* message)
{
    // alive message
    matrixCleanModel(matrix);
    matrixPushString(matrix, message);
    matrixSendModel(matrix);
    matrixSendViewport(matrix);
}

/**
 * Read a command from the pipe
 *
 * @param pipe Pipe to read
 * @param data Data passed from the command
 *
 * @return command Identifier
 */
int readCommand(FILE* pipe, char* data)
{
    char* dataBuffer;
    dataBuffer = (char*)malloc(DATA_BUFFER_SIZE);
    if (fgets(dataBuffer, DATA_BUFFER_SIZE, pipe)) {
        if (dataBuffer[strlen(dataBuffer)-1] == '\n') {
            dataBuffer[strlen(dataBuffer)-1] = 0;
        }
        if (strcmp(dataBuffer, "bye")==0) {
            return COMMAND_QUIT;
        }
        if (strlen(dataBuffer)) {
            strcpy(data, dataBuffer);
        }
        free(dataBuffer);
        return COMMAND_MSG;
    }
    return COMMAND_BAD;
}

/**
 * Daemon main loop
 *
 * @param matrix  the matrix !
 * @param plugins plugins collection
 * @param fonts   fonts collection
 *
 * @return status
 */
int mainLoop(LEDMATRIX* matrix, ANIMATIONPLUGIN** plugins, FONT** fonts)
{
    char* dataBuffer;
    FILE* fifoFile;
    ANIMATIONPLUGIN* plugAnimation;
    void* userData = 0;
    ANIMATION_QUEUE* animations=0;
    unsigned int fontSelector=0;
    int command;

    exitCondition = 1;

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

    // prepare the buffer
    dataBuffer = (char*)malloc(DATA_BUFFER_SIZE);

    // Open the communication pipe
    fifoFile = openCommunicationPipe();

    /* Select font */
    selectFont(matrix, fonts, fontSelector);

    // alive message
    quickMessage(matrix, "Let's go !");

    // main loop
    while (exitCondition) {
        command = readCommand(fifoFile, dataBuffer);
        switch (command) {
            case COMMAND_MSG:
                matrixCleanModel(matrix);
                // send the message to the matrix model
                matrixPushString(matrix, dataBuffer);
                // Animation in action
                animate(matrix, animations);
                // wait for a while
                sleep(5);
                break;
            case COMMAND_QUIT:
                exitCondition = 0;
                break;
            default: break;
        }
    }

    // destroy the animation structure
    destroyAnimationQueue(animations);

    // close the communication pipe
    closeCommunicationPipe(fifoFile);

    // Dead message
    quickMessage(matrix, "I'm dead !");

    // release buffer
    free(dataBuffer);

    return 0;
}

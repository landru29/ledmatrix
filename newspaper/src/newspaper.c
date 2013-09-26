#define DEBUG

#include <ctype.h>
#include <stdio.h>
#include <wchar.h>    /* Types et fonctions relatifs aux caractères étendus */
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "aipointe.h"
#include "arial8.h"
#include "perso.h"
#include "font.h"
#include "ledmatrix.h"
#include "animate.h"
#include "plugins.h"
#include "ini.h"
#include "display.h"

//#define HEIGHT   8     // Nombre de ligne sur une matrice
//#define WIDTH    32    // Nombre de colonnes sur une matrice

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
    fprintf(stdout, "Syntaxe : %s -m message [-d]\n", argv[0]);
    fprintf(stdout, "\t-m message:\n\t\tmessage to send to the matrix\n");
    fprintf(stdout, "\t-s:\n\t\tSimulate the matrix\n");
    fprintf(stdout, "\t-t number:\n\t\tindex of the matrix to completly switch on for a test\n");
    fprintf(stdout, "\t-v:\n\t\tVerbose the configuration and exit\n");
}

/**
 * Construit le chemin vers le dossier des plugins
 *
 * @return chemin absolu vers les plugins
 */
char* pluginsFolder()
{
	//char currentPath[1000]="/usr/local/lib/newspaper/plugins";
	/*if (getcwd(currentPath, sizeof(currentPath)) != 0) {
		strcpy(&currentPath[strlen(currentPath)], "/src/plugins/.libs");
	}*/
	return strdup(LIBDIR);
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
    char optstring[] = "m:t:sv";
    char confPath[200];
    int option;
    INI_LINE* configuration;
    LEDMATRIX* matrix = 0; // Espace mémoire pour l'écriture sur les matrices
    FONT* font = 0;
    ANIMATION_QUEUE* animations=0;
    shared_function getFrames;
    //GIFANIMATION* gif;
    unsigned char csValueTmp;
    int testMatrixIndex=-1;
    char* dataTest;
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

    /* Load plugins */
    plugins = loadPlugins(pluginsFolder());
	if (!plugins) {
		printf("No plugins ! Exiting.\n");
		return 0;
	}
    for(i=0; plugins[i]; i++)
		printf("%s was loaded\n", plugins[i]->name);


    /* Check if launched by root */
    if (getuid() !=0)
        fprintf(stdout, "You should be root to launch this program\n");

    /* read config */
    sprintf(confPath, "%s/conf.ini", CONFDIR);
    configuration = iniParse(confPath);
    /* retrieve data from configuration */
    if (iniHasKey(configuration, "displays")) {
        sscanf(iniGet(configuration, "displays"), "%d", &displays);
    } else {
        displays = 3;
    }
    if (iniHasKey(configuration, "matrixHeight")) {
        sscanf(iniGet(configuration, "matrixHeight"), "%d", &matrixHeight);
    } else {
        matrixHeight = 8;
    }
    if (iniHasKey(configuration, "matrixWidth")) {
        sscanf(iniGet(configuration, "matrixWidth"), "%d", &matrixWidth);
    } else {
        matrixWidth = 32;
    }
    if (iniHasKey(configuration, "CS0")) {
        sscanf(iniGet(configuration, "CS0"), "%d", &csValueTmp);
        setCs(0, csValueTmp);
    }
    if (iniHasKey(configuration, "CS1")) {
        sscanf(iniGet(configuration, "CS1"), "%d", &csValueTmp);
        setCs(1, csValueTmp);
    }
    if (iniHasKey(configuration, "CS2")) {
        sscanf(iniGet(configuration, "CS2"), "%d", &csValueTmp);
        setCs(2, csValueTmp);
    }
    if (iniHasKey(configuration, "CS3")) {
        sscanf(iniGet(configuration, "CS3"), "%d", &csValueTmp);
        setCs(3, csValueTmp);
    }

    /* Release configuration */
    iniDestroy(configuration);

    /* check if there is at least one argument */
    if (argc<2) {
        usage(argv);
        return 0;
    }

    opterr=0; /* Pas de message d'erreur automatique */

    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'g':
                graphic = 1;
                break;
            case 'm': /* example of options with value */
                message = strdup(optarg);
                break;
            case 's':
                simulated = 1;
                break;
            case 't':
                sscanf(optarg, "%d", &testMatrixIndex);
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


    /* Matrix initialisation */
    matrix = openLedMatrix(displays*matrixWidth, matrixHeight);

    /* Switch on the simulator */
    if (simulated) matrixSetDebugMode(matrix, 1);

    if (testMatrixIndex>=0) {
        fprintf(stdout, "Testing matrix %d\n", testMatrixIndex);
        dataTest = (char*) malloc(matrixWidth*matrixHeight/8);
        memset(dataTest, 0xff, matrixWidth*matrixHeight/8);
        writeScreen(testMatrixIndex, dataTest, matrixWidth*matrixHeight/8, matrixWidth, matrixHeight);
        free (dataTest);
        return 0;
    }

    //printf("Matrices initialisées\n");
    font = createFont(perso_font, perso_info, perso_mapping, 1);
    //font = createFont(arial8_font, arial8_info, arial8_mapping, 1);
    //printf("fontes initialisées\n");
    matrixSetFont(matrix, font);
    //printf("fontes ajoutées aux matrices\n");

	/* check if a message was specified */
    if ((!message) || (!*message)) {
        printf("Le message est vide\n");
        return 0;
    }


    printf("Le message: %s\n", message);

    matrixPushString(matrix, message);
    
    /* Switch on the simulator */
    if (simulated) {
		matrixSetDebugMode(matrix, 1);
		matrixDebugInit();
	}

    /* Animation in action */
    animations = createAnimationQueue();

    /* getting animation from plugin */
    plugAnimation = getPluginAnimation(plugins, "scrollV");
    if (plugAnimation) {
		if (plugAnimation->creation)
			userData = (plugAnimation->creation)();
		enqueueAnimation(animations, createAnimation(plugAnimation->runtime, 8, -8, 1, 150, userData, plugAnimation->destruction));
		userData = 0;
    }

    /*plugAnimation = getPluginAnimation(plugins, "gif");
    if (plugAnimation) {
		if (plugAnimation->creation)
			userData = (plugAnimation->creation)("foo.gif");
		getFrames = getPluginFunction(plugAnimation, "getFrames");
		enqueueAnimation(animations, createAnimation(plugAnimation->runtime, 0, getFrames(userData), 1, 150, userData, plugAnimation->destruction));
		userData = 0;
	}*/
    
    /*enqueueAnimation(animations, createAnimation(gifAnimation, 0, gif->frameCount-1, 1, 150, gif));
    
    enqueueAnimation(animations, createAnimation(scrollV, 8, -8, 1, 150, 0));
    //printf("Message ajouté aux matrices\n");

    int lengthMsg, remaining, position;
    lengthMsg = matrix->modelWidth * matrix->modelHeight;
    //printf("length: %d\n", lengthMsg);
    remaining = lengthMsg;
    position = 0;
    if (lengthMsg > (displays*matrixWidth)) {
        animations = createAnimationQueue();
        remaining = lengthMsg-(displays*matrixWidth);
        //for (i=0; i<3;i++) {
            enqueueAnimation(animations, createAnimation(interval, 0, 1, 1, 500, 0));
            enqueueAnimation(animations, createAnimation(scrollH, 0, -(remaining), 2, 150, 0));
            enqueueAnimation(animations, createAnimation(interval, 0, 1, 1, 500, 0));
            enqueueAnimation(animations, createAnimation(scrollH, -(remaining), 0, 2, 150, 0));
        //}
        animate(matrix, animations);
    } else {
        matrixSendModel(matrix);
        matrixSendViewport(matrix);
    }

    /* Wait for a while */
    //usleep(2000*1000);

    /* Animation in action */
    animate(matrix, animations);
    matrixSendModel(matrix);
    matrixSendViewport(matrix);

	/* Cleaning everything */
    destroyFont(font);
    closeLedMatrix(matrix);
    destroyAnimationQueue(animations);
    closePlugins(plugins);

    return 0;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

#define DEBUG

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include "aipointe.h"
#include "perso.h"
#include "font.h"
#include "ledmatrix.h"
#include "animate.h"
#include "plugins.h"

#define DISPLAYS 3     // Nombre des matrices
#define HEIGHT   8     // Nombre de ligne sur une matrice
#define WIDTH    32    // Nombre de colonnes sur une matrice

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
}

/**
 * Construit le chemin vers le dossier des plugins   
 * 
 * @return chemin absolu vers les plugins
 */
char* pluginsFolder()
{
	char currentPath[1000]="";
	if (getcwd(currentPath, sizeof(currentPath)) != 0) {
		strcpy(&currentPath[strlen(currentPath)], "/src/plugins/.libs");
	}
	return strdup(currentPath);
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
    char optstring[] = "m:s";
    int option;
    LEDMATRIX* matrix = 0; // Espace mémoire pour l'écriture sur les matrices
    FONT* font = 0;
    ANIMATION_QUEUE* animations=0;
    plugin_function getFrames;
    char* message=0;
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
    for(i=0; plugins[i]; i++)
		printf("%s was loaded\n", plugins[i]->name);

	/* check if there is at least one argument */
    if (argc<2) {
        usage(argv);
        return 0;
    }

    opterr=0; /* Pas de message d'erreur automatique */

    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'm': /* example of options with value */
                message = strdup(optarg);
                break;
            case 's':
				simulated = 1;
				break;
            default:
                abort();
                break;
        }
    }
#ifndef __arm__     
    simulated = 1;
#endif

	/* check if a message was specified */
    if ((!message) || (!*message)) {
        printf("Le message est vide\n");
        return 0;
    }
;

	/* Matrix initialisation */
    matrix = openLedMatrix(DISPLAYS*WIDTH, HEIGHT);
    font = createFont(perso_font, perso_info, perso_mapping, 1);
    matrixSetFont(matrix, font);
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
	
	plugAnimation = getPluginAnimation(plugins, "gif");
    if (plugAnimation) {
		if (plugAnimation->creation)
			userData = (plugAnimation->creation)("foo.gif");
		getFrames = getPluginFunction(plugAnimation, "getFrames");
		enqueueAnimation(animations, createAnimation(plugAnimation->runtime, 0, getFrames(userData), 1, 150, userData, plugAnimation->destruction));
		userData = 0;
	}
    
    /*enqueueAnimation(animations, createAnimation(gifAnimation, 0, gif->frameCount-1, 1, 150, gif));
    
    enqueueAnimation(animations, createAnimation(scrollV, 8, -8, 1, 150, 0));
    enqueueAnimation(animations, createAnimation(interval, 0, 1, 1, 500, 0));
    enqueueAnimation(animations, createAnimation(scrollV, -8, 0, 1, 150, 0));
    enqueueAnimation(animations, createAnimation(blink, 0, 1, 1, 2000, 0));
    enqueueAnimation(animations, createAnimation(scrollH, 0, DISPLAYS*WIDTH, 2, 150, 0));
    enqueueAnimation(animations, createAnimation(interval, 0, 1, 1, 500, 0));
    enqueueAnimation(animations, createAnimation(scrollH, DISPLAYS*WIDTH, 0-(strlen(message)*8), 2, 150, 0));
    enqueueAnimation(animations, createAnimation(interval, 0, 1, 1, 500, 0));
    enqueueAnimation(animations, createAnimation(scrollH, -(strlen(message)*8), 0, 2, 150, 0));*/
    animate(matrix, animations);

	/* Cleaning everything */
    destroyFont(font);
    closeLedMatrix(matrix);
    destroyAnimationQueue(animations);
	
	closePlugins(plugins);

    return 0;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

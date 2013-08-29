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
#include "animationpool.h"
#include "animationgif.h"

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
    GIFANIMATION* gif;
    char* message=0;
    int i;
    int graphic = 0;
    int simulated=
#ifdef __arm__
    0;
#else
    1;
#endif

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
            default:
                abort();
                break;
        }
    }

	/* Matrix initialisation */
    matrix = openLedMatrix(DISPLAYS*WIDTH, HEIGHT);
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
    //printf("Message ajouté aux matrices\n");

    /* Switch on the simulator */
    if (simulated) matrixSetDebugMode(matrix, 1);

    int lengthMsg, remaining, position;
    lengthMsg = matrix->modelWidth * matrix->modelHeight;
    //printf("length: %d\n", lengthMsg);
    remaining = lengthMsg;
    position = 0;
    if (lengthMsg > (DISPLAYS*WIDTH)) {
        animations = createAnimationQueue();
        remaining = lengthMsg-(DISPLAYS*WIDTH);
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
    /*gif = openGifFile("./foo.gif");
    animations = createAnimationQueue();
    enqueueAnimation(animations, createAnimation(gifAnimation, 0, gif->frameCount-1, 1, 150, gif));
    */
    /*enqueueAnimation(animations, createAnimation(scrollV, 8, -8, 1, 150, 0));
    enqueueAnimation(animations, createAnimation(interval, 0, 1, 1, 500, 0));
    enqueueAnimation(animations, createAnimation(scrollV, -8, 0, 1, 150, 0));
    enqueueAnimation(animations, createAnimation(fade, 15, 0, 1, 250, 0));
    enqueueAnimation(animations, createAnimation(interval, 0, 1, 1, 500, 0));
    enqueueAnimation(animations, createAnimation(fade, 0, 15, 1, 250, 0));
    enqueueAnimation(animations, createAnimation(blink, 0, 1, 1, 2000, 0));
    enqueueAnimation(animations, createAnimation(scrollH, 0, DISPLAYS*WIDTH, 2, 150, 0));
    enqueueAnimation(animations, createAnimation(interval, 0, 1, 1, 500, 0));
    enqueueAnimation(animations, createAnimation(scrollH, DISPLAYS*WIDTH, 0-(strlen(message)*8), 2, 150, 0));
    enqueueAnimation(animations, createAnimation(interval, 0, 1, 1, 500, 0));
    enqueueAnimation(animations, createAnimation(scrollH, -(strlen(message)*8), 0, 2, 150, 0));
    animate(matrix, animations);
    matrixSendModel(matrix);
    matrixSendViewport(matrix);*/

	/* Cleaning everything */
    destroyFont(font);
    closeLedMatrix(matrix);
    destroyAnimationQueue(animations);
	//closeGifFile(gif);

    return 0;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

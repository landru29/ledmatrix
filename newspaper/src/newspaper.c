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
#include "animationpool.h"


int displays = 3; // Nombre des matrices
int height = 8;   // Nombre de ligne sur une matrice
int width = 32;   // Nombre de colonnes sur une matrice

/**
 * Display usage of program on stdout
 */
void usage()
{
    printf("newspaper -m message\n");
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
    char optstring[] = "m:";
    int option;
    LEDMATRIX* matrix = 0; // Espace mémoire pour l'écriture sur les matrices
    FONT* font = 0;
    ANIMATION_QUEUE* animations=0;
    char* message;

    if (argc<2) {
        usage();
        return 0;
    }

    opterr=0; /* Pas de message d'erreur automatique */

    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 'm': /* example of options with value */
                message = optarg;
                break;
            default:
                abort();
                break;
        }
    }

    if (strlen(message) <= 0) {
        printf("Le message est vide\n");
        return 0;
    }

    matrix = openLedMatrix(displays*width, height);
    font = createFont(perso_font, perso_info, perso_mapping, 1);
    matrixSetFont(matrix, font);
    matrixPushString(matrix, message);
    usleep(2000*1000);

    /* Debugging animation */
    animations = createAnimationQueue();
    enqueueAnimation(animations, createAnimation(scrollV, 8, -8, 1, 150));
    enqueueAnimation(animations, createAnimation(interval, 0, 1, 1, 500));
    enqueueAnimation(animations, createAnimation(scrollV, -8, 0, 1, 150));
    enqueueAnimation(animations, createAnimation(blink, 0, 1, 1, 2000));
    enqueueAnimation(animations, createAnimation(scrollH, 0, displays*width, 2, 150));
    enqueueAnimation(animations, createAnimation(interval, 0, 1, 1, 500));
    enqueueAnimation(animations, createAnimation(scrollH, displays*width, 0-(strlen(message)*8), 2, 150));
    enqueueAnimation(animations, createAnimation(interval, 0, 1, 1, 500));
    enqueueAnimation(animations, createAnimation(scrollH, 0-(strlen(message)*8), 0, 2, 150));
    animate(matrix, animations, 0);
    destroyAnimationQueue(animations);

    destroyFont(font);
    closeLedMatrix(matrix);

    return 0;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

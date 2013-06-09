#ifndef __ANIMATION_POOL_H_
#define __ANIMATION_POOL_H_

#include "animate.h"

/**
 * animation that scroll the text horzontally
 **/
int scrollH(LEDMATRIX* matrix, int frameNumber, void* userData);

/**
 * Animation that write the frame number to the standard output
 **/
int testFrame(LEDMATRIX* matrix, int frameNumber, void* userData);

/**
 * Animation that scroll the text vertically
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 * @return             int
 */
int scrollV(LEDMATRIX* matrix, int frameNumber, void* userData);

/**
 * Animation de pause
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 * @return             int
 */
int interval(LEDMATRIX* matrix, int frameNumber, void* userData);

/**
 * Animation de clignotement
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 * @return             int
 */
int blink(LEDMATRIX* matrix, int frameNumber, void* userData);

#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/
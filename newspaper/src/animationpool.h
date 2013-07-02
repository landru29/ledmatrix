/**
 * Bibliothèque de fonctions d'animations
 *
 * @file    animationpool.h
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#ifndef __ANIMATION_POOL_H_
#define __ANIMATION_POOL_H_

#include "animate.h"
#include "../config.h"

/**
 * Animation that scroll the text horzontally
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return Animation's status
 */
int scrollH(LEDMATRIX* matrix, int frameNumber, void* userData);

/**
 * Animation that write the frame number to the standard output
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return Animation's status
 **/
int testFrame(LEDMATRIX* matrix, int frameNumber, void* userData);

/**
 * Animation that scroll the text vertically
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return Animation's status
 */
int scrollV(LEDMATRIX* matrix, int frameNumber, void* userData);

/**
 * Animation de pause
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return Animation's status
 */
int interval(LEDMATRIX* matrix, int frameNumber, void* userData);

/**
 * Animation de clignotement
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return Animation's status
 */
int blink(LEDMATRIX* matrix, int frameNumber, void* userData);

/**
 * Animation de fadeout
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return animation's status
 */
int fade(LEDMATRIX* matrix, int frameNumber, void* userData);

#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

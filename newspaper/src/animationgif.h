/**
 * Bibliothèque de fonctions d'animations
 *
 * @file    animationpool.h
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#ifndef __ANIMATION_GIF_H_
#define __ANIMATION_GIF_H_


#include "animate.h"
#include <gif_lib.h>

/**
 * Animation that play an animated gif
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return Animation's status
 */
int gifAnimation(LEDMATRIX* matrix, int frameNumber, void* userData);

GifFileType* openGifFile(char* filename);

void closeGifFile(GifFileType* gif);


#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

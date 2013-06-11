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
#include "../config.h"


#include "animate.h"
#ifdef HAS_GIF_LIB
#include <gif_lib.h>
#endif
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

#ifdef HAS_GIF_LIB

GifFileType* openGifFile(char* filename);

void closeGifFile(GifFileType* gif);
#endif

#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

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

typedef struct {
#ifdef HAS_GIF_LIB
	GifFileType* gif;
#endif
	int frameNumber;
} GIFANIMATION;




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

/**
 * Create a new GIF animation
 *
 * @param  chr* filename   filename of the GIF 
 *
 * @return GIFANIMATION*   Animation
 */
GIFANIMATION* openGifFile(char* filename);

/**
 * Close a GIF animation
 *
 * @param  GIFANIMATION* gif animation to close
 */
void closeGifFile(GIFANIMATION* gif);


#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

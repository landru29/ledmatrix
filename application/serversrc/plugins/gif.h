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
#include "../../config.h"
#include "animation.h"

#ifdef HAS_GIF_LIB
#include <gif_lib.h>
#endif

typedef struct {
#ifdef HAS_GIF_LIB
	GifFileType* gif;
#endif
	int frameCount;
} GIFANIMATION;


#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

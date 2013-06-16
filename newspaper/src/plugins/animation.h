/**
 * plugin d'animations
 *
 * @file    animation.h
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#ifndef __ANIMATION_PLUGIN_H_
#define __ANIMATION_PLUGIN_H_

#include "../animate.h"
#include "../../config.h"
#include "../ledmatrix.h"

typedef struct {
	char* name;
	void* runtime;
} ANIMATIONPLUGIN;


#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

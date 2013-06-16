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


typedef int (*host_function) ();

typedef int (*animation_function) ();

typedef struct {
	char* name;
	animation_function runtime;
	void* _pluginHdl;
} ANIMATIONPLUGIN;

typedef ANIMATIONPLUGIN* (*init_function) ();

typedef struct {
	char* name;
	host_function* runtime;
} HOSTFUNCTION;


/**
 * recherche une fonction hôte
 *
 * @param  hostFunctions  table des fonctions hôtes
 * @param  name           nom de la fonction
 *
 * @return fonction
 */
void* getHostFunction(HOSTFUNCTION** hostFunctions, char* name);


#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

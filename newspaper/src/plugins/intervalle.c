/**
 * Plugin d'animation de pause
 *
 * @file    intervalle.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "animation.h"

HOSTFUNCTION** hostFunctions;

/**
 * Animation de pause
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return animation's status
 */
int interval(LEDMATRIX* matrix, int frameNumber, void* userData)
{
	/* return the status */
	return ANIMATION_SUCCESS;
}

/**
 * Return the name of the animation function
 * 
 * @return animation plugin
 **/
ANIMATIONPLUGIN* init(HOSTFUNCTION** hostFunc)
{	
	hostFunctions = hostFunc;
	return createAnimationPlugin("intervalle", interval, 0, 0);
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

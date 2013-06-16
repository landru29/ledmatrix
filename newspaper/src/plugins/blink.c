/**
 * Plugin d'animation de clignottement
 *
 * @file    blink.c
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
 * Animation de clignotement
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return animation's status
 */
int blink(LEDMATRIX* matrix, int frameNumber, void* userData)
{
#ifdef __arm__
	uint8_t i;
	host_function displayBlinkFct;
#endif
	host_function matrixClearViewportFct;
	host_function matrixSendViewportFct;
	uint8_t blink = 0;
	
	/* get functions from host */
	matrixClearViewportFct = (host_function)getHostFunction(hostFunctions, "matrixClearViewport");
	matrixSendViewportFct = (host_function)getHostFunction(hostFunctions, "matrixSendViewport");

	/* définition du statut de blink en fonction du numéro de frame */
	blink = (frameNumber > 0) ? 0 : 1;
	/* Appel de la méthode clignotement sur bloc de la matrice */
#ifdef __arm__
	displayBlinkFct = (host_function)getHostFunction(hostFunctions, "displayBlink");
	for (i=0; i < matrix->viewportWidth/32; i++) {
		displayBlinkFct(i, blink);
	}
#else
	if (blink) 
		matrixSendViewportFct(matrix);
	else
		matrixClearViewportFct(matrix);
#endif
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
	return createAnimationPlugin("blink", blink, 0, 0);
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

/**
 * Plugin d'animation de scrolling vertical
 *
 * @file    scrollv.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "animation.h"

SHAREDFUNCTION** hostFunctions;

/**
 * Animation that scroll the text vertically
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return Animation's status
 */
int scrollV(LEDMATRIX* matrix, int frameNumber, void* userData)
{
	unsigned int i;
	unsigned char data;
	host_function matrixClearViewportFct;
	host_function matrixSendViewportFct;
	
	/* get functions from host */
	matrixClearViewportFct = (host_function)getHostFunction(hostFunctions, "matrixClearViewport");
	matrixSendViewportFct = (host_function)getHostFunction(hostFunctions, "matrixSendViewport");

	/* erase all */
	matrixClearViewportFct(matrix);

	for(i=0; i<matrix->modelWidth; i++) {
		data = matrix->model[i];
		if (frameNumber>0) {
			matrix->viewport[i] = data << frameNumber;
		} else {
			matrix->viewport[i] = data >> -frameNumber;
		}
	}

	/* Send the data to the matrix */
	matrixSendViewportFct(matrix);

	/* return the status */
	return ANIMATION_SUCCESS;
}

/**
 * Return the name of the animation function
 * 
 * @return animation plugin
 **/
ANIMATIONPLUGIN* init(SHAREDFUNCTION** hostFunc)
{
	hostFunctions = hostFunc;
	return createAnimationPlugin("scrollV", scrollV, 0, 0);
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

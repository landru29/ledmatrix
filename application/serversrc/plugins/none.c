/**
 * Plugin d'animation de scrolling vertical
 *
 * @file    none.c
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
 * Animation that display
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return Animation's status
 */
int none(LEDMATRIX* matrix, int frameNumber, void* userData)
{
	unsigned int i;
	unsigned char data;
	unsigned int dataLength = (matrix->modelWidth > matrix->viewportWidth) ? matrix->viewportWidth : matrix->modelWidth;
	shared_function matrixClearViewportFct;
	shared_function matrixSendViewportFct;

	/* get functions from host */
	matrixClearViewportFct = (shared_function)getHostFunction(hostFunctions, "matrixClearViewport");
	matrixSendViewportFct = (shared_function)getHostFunction(hostFunctions, "matrixSendViewport");

	/* erase all */
	matrixClearViewportFct(matrix);

        printf("Plugin none: copying %d bytes to the viewport\n", dataLength);
	for(i=0; i<dataLength; i++) {
		data = matrix->model[i];
                matrix->viewport[i] = data;
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
	return createAnimationPlugin("none", none, 0, 0);
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

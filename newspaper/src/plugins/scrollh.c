/**
 * Plugin d'animation de scrolling horizontal
 *
 * @file    scrollh.c
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
 * Animation that scroll the text horzontally
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return Animation's status
 */
int scrollH(LEDMATRIX* matrix, int frameNumber, void* userData)
{
	host_function matrixClearViewportFct;
	host_function matrixSendViewportFct;
	unsigned int destinationWidth = matrix->viewportWidth-frameNumber;
	unsigned int dataWidth = (destinationWidth<matrix->modelWidth ? destinationWidth : matrix->modelWidth);
	int modelDataStart;

	/* get functions from host */
	matrixClearViewportFct = (host_function)getHostFunction(hostFunctions, "matrixClearViewport");
	matrixSendViewportFct = (host_function)getHostFunction(hostFunctions, "matrixSendViewport");

	/* erase all */
	matrixClearViewportFct(matrix);

	/* copy the data */
	if (frameNumber>=0) {
		memcpy(&matrix->viewport[frameNumber], matrix->model, dataWidth);
	} else {
		modelDataStart = (-frameNumber < matrix->modelWidth ? -frameNumber : matrix->modelWidth);
		memcpy(matrix->viewport, &matrix->model[modelDataStart], matrix->modelWidth-modelDataStart);
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
ANIMATIONPLUGIN* init(HOSTFUNCTION** hostFunc)
{
	ANIMATIONPLUGIN* temp = (ANIMATIONPLUGIN*)malloc(sizeof(ANIMATIONPLUGIN));
	temp->name = strdup("scrollH");
	temp->runtime = scrollH;
	temp->creation = 0;
	temp->destruction = 0;
	hostFunctions = hostFunc;
	return temp;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

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



/**
 * Return the name of the animation function
 * 
 * @return animation function name
 **/
char* getName()
{
	return strdup("scrollH");
}

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
	unsigned int destinationWidth = matrix->viewportWidth-frameNumber;
	unsigned int dataWidth = (destinationWidth<matrix->modelWidth ? destinationWidth : matrix->modelWidth);
	int modelDataStart;

	/* erase all */
	matrixClearViewport(matrix);

	/* copy the data */
	if (frameNumber>=0) {
		memcpy(&matrix->viewport[frameNumber], matrix->model, dataWidth);
	} else {
		modelDataStart = (-frameNumber < matrix->modelWidth ? -frameNumber : matrix->modelWidth);
		memcpy(matrix->viewport, &matrix->model[modelDataStart], matrix->modelWidth-modelDataStart);
	}

	/* Send the data to the matrix */
	matrixSendViewport(matrix);

	/* return the status */
	return ANIMATION_SUCCESS;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

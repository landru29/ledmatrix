#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "animationpool.h"
#include "ledmatrix.h"
#include "display.h"

/**
 * Animation that write the frame number to the standard output
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 * @return             int
 */
int testFrame(LEDMATRIX* matrix, int frameNumber, void* userData)
{
	printf("%d  \r", frameNumber);
	fflush(stdout);
	return ANIMATION_SUCCESS;
}

/**
 * Animation that scroll the text horzontally
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 * @return             int
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

/**
 * Animation that scroll the text vertically
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 * @return             int
 */
int scrollV(LEDMATRIX* matrix, int frameNumber, void* userData)
{
	unsigned int i;
	unsigned char data;

	/* erase all */
	matrixClearViewport(matrix);

	for(i=0; i<matrix->modelWidth; i++) {
		data = matrix->model[i];
		if (frameNumber>0) {
			matrix->viewport[i] = data << frameNumber;
		} else {
			matrix->viewport[i] = data >> -frameNumber;
		}
	}

	/* Send the data to the matrix */
	matrixSendViewport(matrix);

	/* return the status */
	return ANIMATION_SUCCESS;
}

/**
 * Animation de pause
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 * @return             int
 */
int interval(LEDMATRIX* matrix, int frameNumber, void* userData)
{
	/* return the status */
	return ANIMATION_SUCCESS;
}

/**
 * Animation de clignotement
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 * @return             int
 */
int blink(LEDMATRIX* matrix, int frameNumber, void* userData)
{
	uint8_t i, blink = 0;
	/* définition du statut de blink en fonction du numéro de frame */
	blink = (frameNumber > 0) ? 0 : 1;
	/* Appel de la méthode clignotement sur bloc de la matrice */
	for (i=0; i < matrix->viewportWidth/32; i++) {
		displayBlink(i, blink);
	}
	/* return the status */
	return ANIMATION_SUCCESS;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/
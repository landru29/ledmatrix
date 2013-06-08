#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "animationpool.h"
#include "ledmatrix.h"

int testFrame(LEDMATRIX* matrix, int frameNumber, void* userData)
{
	printf("%d  \r", frameNumber);
	fflush(stdout);
	return ANIMATION_SUCCESS;
}

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

/* vim: set expandtab ai ts=4 sw=4 nu:
*/
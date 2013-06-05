#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "animationpool.h"
#include "ledmatrix.h"

int testFrame(LEDMATRIX* matrix, unsigned int frameNumber)
{
	printf("%d  \r", frameNumber);
	fflush(stdout);
	return ANIMATION_SUCCESS;
}

int scrollH(LEDMATRIX* matrix, unsigned int frameNumber)
{
	unsigned int destinationWidth = matrix->viewportWidth-frameNumber;
	unsigned int dataWidth = (destinationWidth>matrix->modelWidth ? destinationWidth : matrix->modelWidth);
	
	/* erase all */
	matrixClearViewport(matrix);
	
	/* copy the data */
	memcpy(&matrix->viewport[frameNumber], matrix->model, dataWidth);
	
	/* Send the data to the matrix */
	matrixSendViewport(matrix);
	
	/* return the status */
	return ANIMATION_SUCCESS;
}

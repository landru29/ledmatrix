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
	memset(matrix->viewport, 0, frameNumber);
	memcpy(&matrix->viewport[frameNumber], matrix->model, matrix->viewportWidth-frameNumber);
	return ANIMATION_SUCCESS;
}

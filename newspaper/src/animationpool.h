#ifndef __ANIMATION_POOL_H_
#define __ANIMATION_POOL_H_

#include "animate.h"

/**
 * animation that scroll the text horzontally
 **/
int scrollH(LEDMATRIX* matrix, int frameNumber, void* userData);

/**
 * Animation that write the frame number to the standard output
 **/
int testFrame(LEDMATRIX* matrix, int frameNumber, void* userData);

/**
 * animation that scroll the text vertically
 **/
int scrollV(LEDMATRIX* matrix, int frameNumber, void* userData);

#endif

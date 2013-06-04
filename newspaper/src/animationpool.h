#ifndef __ANIMATION_POOL_H_
#define __ANIMATION_POOL_H_

#include "animate.h"

/**
 * animation that scroll the text horzontally
 **/
int scrollH(LEDMATRIX* matrix, unsigned int frameNumber);

/**
 * Animation that write the frame number to the standard output
 **/
int testFrame(LEDMATRIX* matrix, unsigned int frameNumber);



#endif

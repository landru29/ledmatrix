#ifndef __ANIMATE_H_
#define __ANIMATE_H_

#include "ledmatrix.h"

#ifndef MYCALL
#if defined(_MSC_VER)
#define MYCALL __cdecl
#elif defined(__GNUC__) && defined(__i386) && !defined(__INTEL_COMPILER)
#define MYCALL __attribute__((cdecl))
#else
/* For any platform which uses this definition and supports more than
   one calling convention, we need to extend this definition to
   declare the convention used on that platform, if it's possible to
   do so.

   If this is the case for your platform, please file a bug report
   with information on how to identify your platform via the C
   pre-processor and how to specify the same calling convention as the
   platform's malloc() implementation.
*/
#define MYCALL
#endif
#endif  /* not defined XMLCALL */


#define ANIMATION_FAILURE -1
#define ANIMATION_SUCCESS 0
#define ANIMATION_END 1

typedef int (MYCALL *animationFrame) (LEDMATRIX* matrix, int frameNumber, void* userData);

typedef struct {
	animationFrame animation;
	int startFrameNumber;
	int endFrameNumber;
	unsigned int step;
	unsigned int millitime;
} ANIMATION;

typedef struct {
	ANIMATION** animationList;
	unsigned int len;
} ANIMATION_QUEUE;


/**
 * Create a new animation
 * 
 * @param animationFrame animationFunction : function that perform a frame state
 * @param int startFrameNumber : start frame of the animation
 * @param int endFrameNumber : end frame of the animation
 * @param unsigned int step : step counter
 * @param unsigned int millitime : delay between two frames (in milliseconds)
 **/ 
ANIMATION* createAnimation(animationFrame animationFunction, unsigned int startFrameNumber, unsigned int endFrameNumber, unsigned int step, unsigned int millitime);

/**
 * Destroy an animation
 * 
 * @param ANIMATION* animation : animation to destroy
 **/
void destroyAnimation(ANIMATION* animation);

/**
 * Create an animation queue
 **/
 ANIMATION_QUEUE* createAnimationQueue();
 
 /**
  * Destroy an animation queue
  * 
  * @param ANIMATION_QUEUE* queue : animation queue to destroy
  **/
void destroyAnimationQueue(ANIMATION_QUEUE* queue);

/**
 * Enqueue an animation in the animation list
 * 
 * @param ANIMATION_QUEUE* queue : animation queue
 * @param ANIMATION* animation : animation to push
 **/
ANIMATION_QUEUE* enqueueAnimation(ANIMATION_QUEUE* queue, ANIMATION* animation);

/**
 * perform an animation
 * 
 * @param LEDMATRIX* matrix : matrix on which the animation is played
 * @param ANIMATION* animation : animation to play
 * @param unsigned int num : number of animations
 **/
int animateOne(LEDMATRIX* matrix, ANIMATION* animation, void* userData);

/**
 * perform many animations
 * 
 * @param LEDMATRIX* matrix : matrix on which the animation is played
 * @param ANIMATION* animation : animation to play
 * @param unsigned int num : number of animations
 **/
int animate(LEDMATRIX* matrix, ANIMATION_QUEUE* animations, void* userData);

#endif

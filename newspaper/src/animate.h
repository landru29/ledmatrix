/**
 * Bibliothèque des fonctions de manipulation des animations
 *
 * @file    animate.h
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#ifndef __ANIMATE_H_
#define __ANIMATE_H_

#include "ledmatrix.h"
#include "../config.h"

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
	void* userData;
} ANIMATION;

typedef struct {
	ANIMATION** animationList;
	unsigned int len;
} ANIMATION_QUEUE;


/**
 * Create a new animation
 *
 * @param animationFunction Function that perform a frame state
 * @param startFrameNumber  Start frame of the animation
 * @param endFrameNumber    End frame of the animation
 * @param step              Step counter
 * @param millitime         Delay between two frames (in milliseconds)
 */
ANIMATION* createAnimation(animationFrame animationFunction, int startFrameNumber, int endFrameNumber,
    unsigned int step, unsigned int millitime, void* userData);

/**
 * Destroy an animation
 *
 * @param animation Animation to destroy
 */
void destroyAnimation(ANIMATION* animation);

/**
 * Create an animation queue
 */
ANIMATION_QUEUE* createAnimationQueue();

/**
 * Destroy an animation queue
 *
 * @param queue Animation queue to destroy
 */
void destroyAnimationQueue(ANIMATION_QUEUE* queue);

/**
 * Enqueue an animation in the animation list
 *
 * @param queue     Animation queue
 * @param animation Animation to push
 */
ANIMATION_QUEUE* enqueueAnimation(ANIMATION_QUEUE* queue, ANIMATION* animation);

/**
 * perform an animation
 *
 * @param matrix    matrix on which the animation is played
 * @param animation animation to play
 * @param userData  number of animations
 *
 * @return Animation's status
 */
int animateOne(LEDMATRIX* matrix, ANIMATION* animation);

/**
 * perform many animations
 *
 * @param matrix     matrix on which the animation is played
 * @param animations animation to play
 * @param userData   number of animations
 *
 * @return Animation's status
 */
int animate(LEDMATRIX* matrix, ANIMATION_QUEUE* animations);

#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

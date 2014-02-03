/**
 * Bibliothèque des fonctions de manipulation des animations
 *
 * @file    animate.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#include "animate.h"
#include <malloc.h>
#include <unistd.h>

/**
 * Create a new animation
 *
 * @param animationFunction Function that perform a frame state
 * @param startFrameNumber  Start frame of the animation
 * @param endFrameNumber    End frame of the animation
 * @param step              Step counter
 * @param millitime         Delay between two frames (in milliseconds)
 * @param userData			User data to pass to the animation function
 * @param constructor       fonction de construction de userData
 * @param destructor        fonction de destruction de userData
 *
 * @return struct animation
 */
ANIMATION* createAnimation(
    pluginFunction animationFunction,
    int startFrameNumber,
    int endFrameNumber,
    unsigned int step,
    unsigned int millitime,
    void* userData,
    pluginFunction constructor,
    pluginFunction destructor)
{
	ANIMATION* animation;
	animation = (ANIMATION*) malloc(sizeof(ANIMATION));
	animation->startFrameNumber = startFrameNumber;
	animation->endFrameNumber = endFrameNumber;
	animation->step = (step<2?1:step);
	animation->millitime = millitime;
	animation->userData = userData;
	animation->animation = animationFunction;
	animation->destructor = destructor;
	animation->constructor = constructor;
	return animation;
}

/**
 * Destroy an animation
 *
 * @param animation Animation to destroy
 */
void destroyAnimation(ANIMATION* animation)
{
	if (animation) free(animation);
}

/**
 * Create an animation queue
 *
 * @return animation queue
 */
ANIMATION_QUEUE* createAnimationQueue()
{
    ANIMATION_QUEUE* queue;
    queue = (ANIMATION_QUEUE*) malloc(sizeof(ANIMATION_QUEUE));
    queue->animationList = (ANIMATION**) malloc(sizeof(ANIMATION*));
    queue->len = 0;
    return queue;
}

/**
 * Destroy an animation queue
 *
 * @param queue Animation queue to destroy
 */
void destroyAnimationQueue(ANIMATION_QUEUE* queue)
{
	int n;
	if (!queue) return;
	for(n=0; n<queue->len; n++) {
		destroyAnimation(queue->animationList[n]);
	}
	free(queue->animationList);
	free(queue);
}

/**
 * Enqueue an animation in the animation list
 *
 * @param queue     Animation queue
 * @param animation Animation to push
 *
 * @return animation queue
 */
ANIMATION_QUEUE* enqueueAnimation(ANIMATION_QUEUE* queue, ANIMATION* animation) {
	queue->len ++;
	queue->animationList = (ANIMATION**) realloc(queue->animationList, sizeof(ANIMATION*) * queue->len);
	queue->animationList[queue->len-1] = animation;
	return queue;
}

/**
 * perform an animation
 *
 * @param matrix    matrix on which the animation is played
 * @param animation animation to play
 *
 * @return Animation's status
 */
int animateOne(LEDMATRIX* matrix, ANIMATION* animation) {
	unsigned long int animationDelay = 100;
	int currentFrameNumber;
	int status = ANIMATION_SUCCESS;
	// Check if everything is allright
	if ((!matrix) || (!animation) || (!animation->animation)) return ANIMATION_FAILURE;
	// check animation delay
	if (animation->millitime) animationDelay = animation->millitime;
	// building userData
	if (animation->constructor) {
		(animation->constructor)(animation->userData);
	}
	// Launching animation
	if (animation->startFrameNumber > animation->endFrameNumber) {
		for(currentFrameNumber=animation->startFrameNumber; ((currentFrameNumber >= (int)animation->endFrameNumber) && (status == ANIMATION_SUCCESS)); currentFrameNumber -= animation->step) {
			status = (int)((animation->animation)(matrix, currentFrameNumber, animation->userData));
			usleep(1000*animationDelay);
		}
	} else {
		for(currentFrameNumber=animation->startFrameNumber; ((currentFrameNumber <= (int)animation->endFrameNumber) && (status == ANIMATION_SUCCESS)); currentFrameNumber += animation->step) {
			status = (int)((animation->animation)(matrix, currentFrameNumber, animation->userData));
			usleep(1000*animationDelay);
		}
	}
	/* checking status*/
	if ((status != ANIMATION_FAILURE) && (currentFrameNumber==animation->endFrameNumber)) status = ANIMATION_END;
	/* destroying userData */
	if (animation->destructor) {
		(animation->destructor)(animation->userData);
	}
	return status;
}

/**
 * perform many animations
 *
 * @param matrix     matrix on which the animation is played
 * @param animations animation to play
 *
 * @return Animation's status
 */
int animate(LEDMATRIX* matrix, ANIMATION_QUEUE* animations) {
	unsigned int n;
	int status = ANIMATION_SUCCESS;
	for(n=0; ((n < animations->len) && (status == ANIMATION_SUCCESS)); n++) {
		status = animateOne(matrix, animations->animationList[n]);
	}
	if ((status != ANIMATION_FAILURE) && (n==animations->len)) status = ANIMATION_END;
	return status;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

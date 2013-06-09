#include "animate.h"
#include <malloc.h>
#include <unistd.h>

/**
 * Create a new animation
 *
 * @param animationFrame animationFunction Function that perform a frame state
 * @param int            startFrameNumber  Start frame of the animation
 * @param int            endFrameNumber    End frame of the animation
 * @param unsigned int   step              Step counter
 * @param unsigned int   millitime         Delay between two frames (in milliseconds)
 *
 * @return ANIMATION*
 **/
ANIMATION* createAnimation(animationFrame animationFunction, int startFrameNumber, int endFrameNumber,
    unsigned int step, unsigned int millitime)
{
	ANIMATION* animation;
	animation = (ANIMATION*) malloc(sizeof(ANIMATION));
	animation->animation = animationFunction;
	animation->startFrameNumber = startFrameNumber;
	animation->endFrameNumber = endFrameNumber;
	animation->step = (step<2?1:step);
	animation->millitime = millitime;
	return animation;
}

/**
 * Destroy an animation
 *
 * @param ANIMATION *animation Animation to destroy
 *
 * @return void
 **/
void destroyAnimation(ANIMATION *animation)
{
	if (animation) free(animation);
}

/**
 * Create an animation queue
 *
 * @return ANIMATION_QUEUE*
 **/
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
 * @param ANIMATION_QUEUE *queue Animation queue to destroy
 *
 * @return void
 **/
void destroyAnimationQueue(ANIMATION_QUEUE *queue)
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
 * @param ANIMATION_QUEUE *queue     Animation queue
 * @param ANIMATION       *animation Animation to push
 *
 * @return ANIMATION_QUEUE*
 **/
ANIMATION_QUEUE* enqueueAnimation(ANIMATION_QUEUE *queue, ANIMATION *animation) {
	queue->len ++;
	queue->animationList = (ANIMATION**) realloc(queue->animationList, sizeof(ANIMATION*) * queue->len);
	queue->animationList[queue->len-1] = animation;
	return queue;
}

/**
 * perform an animation
 *
 * @param LEDMATRIX *matrix    matrix on which the animation is played
 * @param ANIMATION *animation animation to play
 * @param void      *userData  number of animations
 *
 * @return int Status's animation
 **/
int animateOne(LEDMATRIX *matrix, ANIMATION *animation, void *userData) {
	unsigned long int animationDelay = 100;
	int currentFrameNumber;
	int status = ANIMATION_SUCCESS;
	/* Check if everything is allright */
	if ((!matrix) || (!animation) || (!animation->animation)) return ANIMATION_FAILURE;
	/* check animation delay */
	if (animation->millitime) animationDelay = animation->millitime;
	/* Launching animation */
	if (animation->startFrameNumber > animation->endFrameNumber) {
		for(currentFrameNumber=animation->startFrameNumber; ((currentFrameNumber >= (int)animation->endFrameNumber) && (status == ANIMATION_SUCCESS)); currentFrameNumber -= animation->step) {
			status = (animation->animation)(matrix, currentFrameNumber, userData);
			usleep(1000*animationDelay);
		}
	} else {
		for(currentFrameNumber=animation->startFrameNumber; ((currentFrameNumber <= (int)animation->endFrameNumber) && (status == ANIMATION_SUCCESS)); currentFrameNumber += animation->step) {
			status = (animation->animation)(matrix, currentFrameNumber, userData);
			usleep(1000*animationDelay);
		}
	}
	/* checking status*/
	if ((status != ANIMATION_FAILURE) && (currentFrameNumber==animation->endFrameNumber)) status = ANIMATION_END;
	return status;
}

/**
 * perform many animations
 *
 * @param LEDMATRIX       *matrix    matrix on which the animation is played
 * @param ANIMATION_QUEUE *animation animation to play
 * @param void 		 	  *userData  number of animations
 *
 * @return int Status's animation
 **/
int animate(LEDMATRIX *matrix, ANIMATION_QUEUE *animations, void *userData) {
	unsigned int n;
	int status = ANIMATION_SUCCESS;
	for(n=0; ((n < animations->len) && (status == ANIMATION_SUCCESS)); n++) {
		status = animateOne(matrix, animations->animationList[n], userData);
	}
	if ((status != ANIMATION_FAILURE) && (n==animations->len)) status = ANIMATION_END;
	return status;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/
/**
 * Plugin d'animation exemple
 *
 * @file    example.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @version 0.1
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "animation.h"

SHAREDFUNCTION** hostFunctions;

/**
 * Animation that scroll the text horzontally
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return Animation's status
 */
int example(LEDMATRIX* matrix, int frameNumber, void* userData)
{
	shared_function matrixClearViewportFct;
	shared_function matrixSendViewportFct;

	// get functions from host
	matrixClearViewportFct = (shared_function)getHostFunction(hostFunctions, "matrixClearViewport");
	matrixSendViewportFct = (shared_function)getHostFunction(hostFunctions, "matrixSendViewport");

	// erase all
	matrixClearViewportFct(matrix);

    // process data into the matrix

	// Send the data to the matrix
	matrixSendViewportFct(matrix);

	// return the status
	return ANIMATION_SUCCESS;
}

/**
 * Fonction invoked just before animation
 *
 * @param  userData data defined by the user
 *
 * @return void
 */
void creator(void* userData)
{
}

/**
 * Fonction invoked juste after
 *
 * @param  userData data defined by the user
 *
 * @return void
 */
void destructor(void* userData)
{
}

/**
 * This function is invoke during the plugin initialization. it must return a plugin object.
 *
 * @param  hostFunc Set of host functions :
 *             matrixSendViewport(LEDMATRIX* matrix);
 *             matrixClearViewport(LEDMATRIX* matrix);
 *
 * @return animation plugin
 **/
ANIMATIONPLUGIN* init(SHAREDFUNCTION** hostFunc)
{
	hostFunctions = hostFunc;
	return createAnimationPlugin("example", example, creator, destructor);
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

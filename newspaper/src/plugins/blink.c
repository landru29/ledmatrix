/**
 * Plugin d'animation de clignottement
 *
 * @file    blink.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "animation.h"


#ifdef __arm__
    #include "display.h"
#else
 	void displayBlink(uint8_t chip, uint8_t blinky){}
#endif

/**
 * Return the name of the animation function
 * 
 * @return animation function name
 **/
char* getName()
{
	return strdup("blink");
}

/**
 * Animation de clignotement
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return animation's status
 */
int blink(LEDMATRIX* matrix, int frameNumber, void* userData)
{
#ifdef __arm__
	uint8_t i;
#endif
	uint8_t blink = 0;

	/* définition du statut de blink en fonction du numéro de frame */
	blink = (frameNumber > 0) ? 0 : 1;
	/* Appel de la méthode clignotement sur bloc de la matrice */
#ifdef __arm__
	for (i=0; i < matrix->viewportWidth/32; i++) {
		displayBlink(i, blink);
	}
#else
	if (blink) 
		matrixSendViewport(matrix);
	else
		matrixClearViewport(matrix);
#endif
	/* return the status */
	return ANIMATION_SUCCESS;
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

/**
 * Bibliothèque de fonctions de manipulation de la matrice
 *
 * @file    ledmatrix.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#ifndef __LEDMATRIX_H
#define __LEDMATRIX_H

#include "font.h"
#include "../config.h"

/**
 * Structure de la matrice
 */
typedef struct {
	unsigned int viewportWidth;
	unsigned int viewportHeight;
	unsigned int modelWidth;
	unsigned int modelHeight;
	unsigned char* model;
	unsigned char* viewport;
	FONT* font;
	int debugMode;
} LEDMATRIX;


/**
 * Create a new object that hold all the data to print into the led matrix
 *
 * @param width  Matrix width (number of leds)
 * @param height Matrix height (number of leds)
 *
 * @return Matrix struct
 */
LEDMATRIX* openLedMatrix(unsigned int width, unsigned int height);

/**
 * Close and destroy the object that hold the matrix
 *
 * @param matrix Matrix to close
 */
void closeLedMatrix(LEDMATRIX* matrix);

/**
 * Push a letter to the model of the led matrix
 *
 * @param matrix Matrix object
 * @param letter Letter to push
 */
void matrixPushLetter(LEDMATRIX* matrix, LETTER letter);

/**
 * Push a letter to the model of the led matrix
 *
 * @param matrix Matrix object
 * @param string String to push
 */
void matrixPushString(LEDMATRIX* matrix, char* string);

/**
 * Set a font to the matrix
 *
 * @param matrix Matrix object
 * @param font   Font to set
 */
void matrixSetFont(LEDMATRIX* matrix, FONT* font);

/**
 * Erase the model of a matrix
 *
 * @param matrix Matrix object
 */
void matrixCleanModel(LEDMATRIX* matrix);

/**
 * send the viewport data to the output
 *
 * @param matrix Matrix object
 */
void matrixSendViewport(LEDMATRIX* matrix);

/**
 * send the viewport data to the screen
 *
 * @param matrix Matrix object
 */
void matrixDebugViewport(LEDMATRIX* matrix);

/**
 * Erase the viewport of a matrix
 *
 * @param matrix Matrix object
 */
void matrixClearViewport(LEDMATRIX* matrix);

/**
 * Send the model on the viewport
 *
 * @param matrix Matrix object
 */
void matrixSendModel(LEDMATRIX* matrix);

/**
 * Initialize the console for debuging
 */
void matrixDebugInit();

/**
 * Set the debug mode
 *
 * @param matrix Matrix object
 * @param debug  Mode debug = 1, Mode hardware = 0
 */
void matrixSetDebugMode(LEDMATRIX* matrix, unsigned int debug);

/**
 * Display model's matrix on stdout
 *
 * @param matrix Matrix object
 */
void matrixCheckModel(LEDMATRIX* matrix);

/**
 * Display infos of matrix on stdout
 *
 * @param matrix Matrix object
 */
void matrixDebug(LEDMATRIX* matrix);


#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

#ifndef __LEDMATRIX_H
#define __LEDMATRIX_H

#include "font.h"

typedef struct {
	unsigned int viewportWidth;
	unsigned int viewportHeight;
	unsigned int modelWidth;
	unsigned int modelHeight;
	unsigned char* model;
	unsigned char* viewport;
	FONT* font;
} LEDMATRIX;


/**
 * Create a new object that hold all the data to print into the led matrix
 * 
 * @param unsigned int width :  matrix width (number of leds)
 * @param unsigned int height : matrix height (number of leds)
 **/
LEDMATRIX* openLedMatrix(unsigned int width, unsigned int height);

/**
 * Close and destroy the object that hold the matrix
 * 
 * @param LEDMATRIX* matrix : matrix to close
 **/ 
void closeLedMatrix(LEDMATRIX* matrix);

/**
 *  Print a message in the model of the led matrix
 * 
 * @param LEDMATRIX* matrix :  matrix object
 * @param const char* format : format (printf like)
 * @param list of variables
 **/
void matrixModelPrint(LEDMATRIX *matrix, const char *format, ...);

/**
 * Push a letter to the model of the led matrix 
 * 
 * @param LEDMATRIX* matrix : matrix object
 * @param LETTER letter:      letter to push
 **/
void matrixPushLetter(LEDMATRIX* matrix, LETTER letter);

/**
 * Push a string to the model of the led matrix 
 * 
 * @param LEDMATRIX* matrix : matrix object
 * @param char* string :      string to push
 **/
void matrixPushString(LEDMATRIX* matrix, char* string);

/**
 * Set a font to the matrix
 * 
 * @param LEDMATRIX* matrix : matrix object
 * @param FONT* font:         font to set
 **/
void matrixSetFont(LEDMATRIX* matrix, FONT* font);

/**
 * Erase the model of a matrix
 * 
 * @param LEDMATRIX* matrix : matrix object
 **/
void matrixCleanModel(LEDMATRIX* matrix);

/**
 * send the viewport data to the output
 * 
 * @param LEDMATRIX* matrix : matrix object
 **/
void matrixSendViewport(LEDMATRIX* matrix);

/**
 * Erase the viewport of a matrix
 * 
 * @param LEDMATRIX* matrix : matrix object
 **/
void matrixClearViewport(LEDMATRIX* matrix);


void matrixCheckModel(LEDMATRIX* matrix);
void matrixDebug(LEDMATRIX* matrix);


#endif

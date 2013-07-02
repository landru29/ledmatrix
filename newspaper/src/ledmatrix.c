/**
 * Bibliothèque de fonctions de manipulation de la matrice
 *
 * @file    ledmatrix.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#include "ledmatrix.h"
#include <malloc.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef __arm__
    #include "display.h"
#else
    void writeMatrix(uint8_t *viewport, uint8_t nbMatrix, uint8_t width, uint8_t height){}
    int8_t initDisplay(uint8_t nbMatrix, uint8_t width, uint8_t height){return 0;}
#endif

void columnDebug(unsigned char n, unsigned int column);

/**
 * Create a new object that hold all the data to print into the led matrix
 *
 * @param width  Matrix width (number of leds)
 * @param height Matrix height (number of leds)
 *
 * @return Matrix struct
 */
LEDMATRIX* openLedMatrix(unsigned int width, unsigned int height)
{
	LEDMATRIX* matrix = (LEDMATRIX*) malloc(sizeof(LEDMATRIX));
	matrix->model = (unsigned char*) malloc(1);
	matrix->viewportHeight = height/8;
	matrix->viewportWidth = width;
    matrix->nbMatrix = width / 32;
	matrix->viewport = (unsigned char*) malloc(matrix->viewportHeight * matrix->viewportWidth);
	matrixClearViewport(matrix);
	matrix->debugMode=0;
	#ifdef __arm__
	if (initDisplay(matrix->nbMatrix, 32, height) < 0) {
		printf("Erreur d'initialisation des matrices\n");
	}
	#endif
	return matrix;
}

/**
 * Close and destroy the object that hold the matrix
 *
 * @param matrix Matrix to close
 */
void closeLedMatrix(LEDMATRIX* matrix)
{
	if (!matrix) return;
	if (matrix->model)
		free(matrix->model);
	if (matrix->viewport)
		free(matrix->viewport);
	free(matrix);
}

/**
 * Push a letter to the model of the led matrix
 *
 * @param matrix Matrix object
 * @param letter Letter to push
 */
void matrixPushLetter(LEDMATRIX* matrix, LETTER letter)
{
	unsigned int modelSize = matrix->modelHeight * matrix->modelWidth;
	if (!letter.length) return;
	/* Enlarging the model */
	matrix->model = (unsigned char*) realloc((unsigned char*) matrix->model, modelSize + letter.length + letter.height*letter.spacing);
	/* Copying the letter */
	memcpy(&matrix->model[modelSize], letter.data, letter.length);
	/* Adding a whitespace */
	memset(&matrix->model[modelSize+letter.length], 0, letter.height*letter.spacing);
	/* updating dimensions */
	matrix->modelWidth += (letter.length+letter.spacing) / matrix->modelHeight;
}


/**
 * Push a letter to the model of the led matrix
 *
 * @param matrix Matrix object
 * @param string String to push
 */
void matrixPushString(LEDMATRIX* matrix, char* string)
{
	LETTER letter;
	unsigned int i;
	for(i=0; string[i]; i++) {
		letter = getLetter(string[i], matrix->font);
		matrixPushLetter(matrix, letter);
	}
}


/*void matrixModelPrint(LEDMATRIX *matrix, const char *format, ...)
{
	va_list arg;
	char buffer[1024];

	va_start (arg, format);
	sprintf(buffer, format, arg);
	va_end (arg);

	matrixCleanModel(matrix);


	matrixPushString(matrix, buffer);
}*/

/**
 * Set a font to the matrix
 *
 * @param matrix Matrix object
 * @param font   Font to set
 */
void matrixSetFont(LEDMATRIX* matrix, FONT* font)
{
	matrix->font = font;
	matrix->modelHeight = font->fontHeight;
}

/**
 * Erase the model of a matrix
 *
 * @param matrix Matrix object
 */
void matrixCleanModel(LEDMATRIX* matrix)
{
	/* initialize model */
	matrix->modelWidth = 0;
	matrix->model = (unsigned char*) realloc((unsigned char*) matrix->model, 1);
}

/**
 * send the viewport data to the output
 *
 * @param matrix Matrix object
 */
void matrixSendViewport(LEDMATRIX* matrix)
{
	if (matrix->debugMode) {
		matrixDebugViewport(matrix);
	} else {
#ifdef __arm__
	writeMatrix(matrix->viewport, matrix->nbMatrix, matrix->viewportWidth/matrix->nbMatrix, matrix->viewportHeight*8);
#endif
	}
}

/**
 * Erase the viewport of a matrix
 *
 * @param matrix Matrix object
 */
void matrixClearViewport(LEDMATRIX* matrix) {
	memset(matrix->viewport, 0, matrix->viewportHeight*matrix->viewportWidth);
}

/**
 * send the viewport data to the screen
 *
 * @param matrix Matrix object
 */
void matrixDebugViewport(LEDMATRIX* matrix)
{
	unsigned int i;
	unsigned int column;
	for(i=0; i<matrix->viewportWidth; i++) {
		columnDebug(matrix->viewport[i], i+1);
	}
	column = i;
	for (i=0 ; i<8 ; i++)
	{;
		printf("\033[%d;%dH|\n", i, column);
	}
	printf("\033[%d;%dH-\n", 8, column);
	fflush(stdout);
}

/**
 * Initialize the console for debuging
 */
void matrixDebugInit()
{
	system("clear");
}

/**
 * Send the model on the viewport
 *
 * @param matrix Matrix object
 */
void matrixSendModel(LEDMATRIX* matrix)
{
	memset(matrix->viewport, 0, matrix->viewportHeight*matrix->viewportWidth);
	memcpy(matrix->viewport, matrix->model, matrix->modelWidth);
}

/**
 * Set the debug mode
 *
 * @param matrix Matrix object
 * @param debug  Mode debug = 1, Mode hardware = 0
 */
void matrixSetDebugMode(LEDMATRIX* matrix, unsigned int debug)
{
	matrix->debugMode = debug;
}

/**
 * Debug column
 *
 * @param n      Caractère
 * @param column Column
 */
void columnDebug(unsigned char n, unsigned int column)
{
	unsigned char bit = 0 ;
	unsigned char mask = 0x80 ;
	unsigned int i;
	for (i=0 ; i<8 ; i++)
	{
		bit = (n & mask) >> (7-i) ;
		printf("\033[%d;%dH%c\n", i, column, (bit ? '@':' '));
		mask >>= 1 ;
	}
	printf("\033[%d;%dH-\n", 8, column);
}

/* ################################################################### */
/* ################################################################### */

/**
 * Display model's matrix on stdout
 *
 * @param matrix Matrix object
 */
void matrixCheckModel(LEDMATRIX* matrix)
{
	unsigned int i;
	for(i=0; i< matrix->modelHeight * matrix->modelWidth; i++)
		printf("%02X ", matrix->model[i]);
	printf("\n");
}

/**
 * Display infos of matrix on stdout
 *
 * @param matrix Matrix object
 */
void matrixDebug(LEDMATRIX* matrix)
{
	unsigned int i;
	printf(" === Debugging Matrix ===\n");
	printf("   - viewportWidth: %d\n", matrix->viewportWidth);
	printf("   - viewportHeight: %d\n", matrix->viewportHeight);
	printf("   - modelWidth: %d\n", matrix->modelWidth);
	printf("   - modelHeight: %d\n", matrix->modelHeight);
	for(i=0; i<matrix->modelWidth; i++) {
		printf("   - data %02X ", matrix->model[i]);
		binaryPrint(matrix->model[i]);
		printf("\n");
	}
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

#include "ledmatrix.h"
#include <malloc.h>
#include <string.h>
#include <stdarg.h>

/**
 * Create a new object that hold all the data to print into the led matrix
 * 
 * @param unsigned int width :  matrix width (number of leds)
 * @param unsigned int height : matrix height (number of leds)
 **/
LEDMATRIX* openLedMatrix(unsigned int width, unsigned int height)
{
	LEDMATRIX* matrix = (LEDMATRIX*) malloc(sizeof(LEDMATRIX));
	matrix->model = (unsigned char*) malloc(1);
	matrix->viewport = (unsigned char*) malloc(1);
	matrix->viewportHeight = height/8;
	matrix->viewportWidth = width;
	return matrix;
}

/**
 * Close and destroy the object that hold the matrix
 * 
 * @param LEDMATRIX* matrix : matrix to close
 **/ 
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
 * @param LEDMATRIX* matrix : matrix object
 * @param LETTER letter:      letter to push
 **/
void matrixPushLetter(LEDMATRIX* matrix, LETTER letter)
{
	unsigned int modelSize = matrix->modelHeight * matrix->modelWidth;
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
 * @param LEDMATRIX* matrix : matrix object
 * @param char* string :      string to push
 **/
void matrixPushString(LEDMATRIX* matrix, char* string)
{
	LETTER letter;
	unsigned int i;
	for(i=0; string[i]; i++) {
		letter = getLetter(string[i], matrix->font);
		matrixPushLetter(matrix, letter);
	}
}


void matrixModelPrint(LEDMATRIX *matrix, const char *format, ...)
{
	va_list arg;
	char buffer[1024];

	va_start (arg, format);
	sprintf(buffer, format, arg);
	va_end (arg);
	
	/*printf("#### %s ###\n", buffer);*/
	
	/* initialize model */
	matrixCleanModel(matrix);
	
	/* push letters to the model */
	matrixPushString(matrix, buffer);
}

/**
 * Set a font to the matrix
 * 
 * @param LEDMATRIX* matrix : matrix object
 * @param FONT* font:         font to set
 **/
void matrixSetFont(LEDMATRIX* matrix, FONT* font)
{
	matrix->font = font;
	matrix->modelHeight = font->fontHeight;
}

/**
 * Erase the model of a matrix
 * 
 * @param LEDMATRIX* matrix : matrix object
 **/
void matrixCleanModel(LEDMATRIX* matrix)
{
	/* initialize model */
	matrix->modelWidth = 0;
	matrix->model = (unsigned char*) realloc((unsigned char*) matrix->model, 1);
}

/**
 * send the viewport data to the output
 * 
 * @param LEDMATRIX* matrix : matrix object
 **/
void matrixSendViewport(LEDMATRIX* matrix)
{
	/* sendData(matrix->viewport, matrix->viewportWidth) */
}

/**
 * Erase the viewport of a matrix
 * 
 * @param LEDMATRIX* matrix : matrix object
 **/
void matrixClearViewport(LEDMATRIX* matrix) {
	memset(matrix->viewport, 0, matrix->viewportHeight*matrix->viewportWidth);
}

/* ################################################################### */
/* ################################################################### */

void matrixCheckModel(LEDMATRIX* matrix)
{
	unsigned int i;
	for(i=0; i< matrix->modelHeight * matrix->modelWidth; i++)
		printf("%02X ", matrix->model[i]);
	printf("\n");
}

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

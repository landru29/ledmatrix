/**
 * Bibliothèque de fonctions d'animations
 *
 * @file    animationgif.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#include <stdio.h>
#ifdef HAS_GIF_LIB
#include <gif_lib.h>
#endif
#include <string.h>
#include <malloc.h>
#include "animationgif.h"
#include "ledmatrix.h"

#ifdef HAS_GIF_LIB
int extractRectangle(GifFileType* gif, unsigned int frameNum, unsigned int top, unsigned int left, unsigned int height, unsigned int width, unsigned char* data);
int rectangle2Matrix(unsigned char* matrixData, unsigned char* data, unsigned int height, unsigned int width, unsigned char bgColor);
void printRectangle(unsigned char* data, unsigned int height, unsigned int width);
#endif

/**
 * Animation that play an animated gif
 *
 * @param  matrix      La matrice
 * @param  frameNumber Le numéro de la frame courante
 * @param  userData    Données spécifiques
 *
 * @return Animation's status
 */
int gifAnimation(LEDMATRIX* matrix, int frameNumber, void* userData)
{
#ifdef HAS_GIF_LIB
	GIFANIMATION* gif = (GIFANIMATION*)userData;
	unsigned char* data;
	
	data = (unsigned char*) malloc(matrix->viewportWidth * matrix->viewportHeight*8);
	
	extractRectangle(gif->gif, gif->frameCount-frameNumber-1, 0, 0, matrix->viewportHeight*8, matrix->viewportWidth, data);
	rectangle2Matrix(matrix->viewport, data, matrix->viewportHeight*8, matrix->viewportWidth, gif->gif->SBackGroundColor);
	
	/* Send the data to the matrix */
	matrixSendViewport(matrix);
	
	free(data);
#endif
	return ANIMATION_SUCCESS;
}

/**
 * Create a new GIF animation
 *
 * @param  filename   filename of the GIF 
 *
 * @return Animation gif struct
 */
GIFANIMATION* openGifFile(char* filename)
{
	GIFANIMATION* gif;
	gif = (GIFANIMATION*) malloc(sizeof(GIFANIMATION));
	
#ifdef HAS_GIF_LIB
	if ((gif->gif = DGifOpenFileName(filename)) == 0) {
		printf("An error occured while opening\n");
		return 0;
	}
	
	if (DGifSlurp(gif->gif) != GIF_OK) {
		printf("An error occured while reading\n");
		return 0;
	}
	
	gif->frameCount = gif->gif->ImageCount;
#else
	gif->frameCount = 0;
#endif
	return gif;
}

/**
 * Close a GIF animation
 *
 * @param gif animation to close
 */
void closeGifFile(GIFANIMATION* gif)
{
#ifdef HAS_GIF_LIB
	DGifCloseFile(gif->gif);
#endif
	free(gif);
}


#ifdef HAS_GIF_LIB

/**
 * Extract a rectangle from a GIF file
 * 
 * @param gif		gif descriptor
 * @param frameNum  frame number of the image
 * @param top       number of pixels from the top
 * @param left      number of pixels from the left
 * @param height    height of the rectangle in pixels
 * @param width     width of the rectangle in pixels
 * @param data      destination of the rectangle. You must perform the memory allocation before.
 * 
 * @return Gif status
 * 
 **/
int extractRectangle(GifFileType* gif, unsigned int frameNum, unsigned int top, unsigned int left, unsigned int height, unsigned int width, unsigned char* data)
{
	unsigned int realWidth = width;
	unsigned int realHeight = height;
	unsigned int x;
	unsigned int y;
	
	if (frameNum>=gif->ImageCount) return GIF_ERROR;
	memset(data, gif->SBackGroundColor, width*height);
	
	if (width+left > gif->SavedImages[frameNum].ImageDesc.Width) realWidth = gif->SavedImages[frameNum].ImageDesc.Width-left;
	if (height+top > gif->SavedImages[frameNum].ImageDesc.Height) realHeight = gif->SavedImages[frameNum].ImageDesc.Height-top;
	
	for(y=top; y<realHeight+top; y++) {
		for(x=left; x<realWidth+left; x++) {
			data[(y-top) * width + x-left] = gif->SavedImages[frameNum].RasterBits[y*gif->SavedImages[frameNum].ImageDesc.Width+x];
		}
	}
	return GIF_OK;
}

/**
 * convert a rectangle to matrix data
 * 
 * @param matrixData  destination of the data. You must perform the memory allocation before.
 * @param data        image rectangle (from extractRectangle() function).
 * @param height      height of the rectangle in pixels
 * @param width       width of the rectangle in pixels
 * @param bgcolor     index of the background color
 * 
 * @return Gif status
 * 
 * */
int rectangle2Matrix(unsigned char* matrixData, unsigned char* data, unsigned int height, unsigned int width, unsigned char bgColor)
{
	unsigned int x;
	unsigned int y;
	unsigned int byteOffset;
	unsigned char byte;
	
	memset(matrixData, 0, (height/8)*width);
	
	for(x=0; x<width; x++) {
		for(y=0; y<height; y++) {
			byteOffset = (height-y-1)/8;
			byte = (data[(height-y-1)*width + x]==bgColor ? 0 : 1) << y % 8;
			matrixData[(height/8)*x+byteOffset] |= byte;
		}
	}
	return GIF_OK;
}

/**
 * Debug a rectangle to the console
 * 
 * @param data        image rectangle (from extractRectangle() function).
 * @param height      height of the rectangle in pixels
 * @param width       width of the rectangle in pixels
 **/
void printRectangle(unsigned char* data, unsigned int height, unsigned int width)
{
	unsigned int x;
	unsigned int y;
	for(y=0; y<height; y++) {
		for(x=0; x<width; x++) {
			printf("%02X", data[y*width+x]);
		}
		printf("\n");
	}
}

#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

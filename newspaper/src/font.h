#ifndef __FONT_H
#define __FONT_H

#include <stdint.h>


typedef struct {
  unsigned char length;
  uint16_t offset;
} FONT_INFO;


typedef struct {
	char* mapping;
	FONT_INFO* allocationTable;
	unsigned char* data;
	unsigned char fontHeight;
	unsigned char letterSpacing;
} FONT;

typedef struct {
	unsigned char length;
	unsigned char height;
	unsigned char* data;
	unsigned char spacing;
} LETTER;


/**
 * Create a new font
 *
 * @param unsigned char* data
 * @param FONT_INFO* allocationTable
 * @param char* mapping
 **/
FONT* createFont(unsigned char* data, FONT_INFO* allocationTable, char* mapping, unsigned char fontHeight);

/**
 * Destroy the font object
 *
 * @param FONT* font : font object to destroy
 **/
void destroyFont(FONT* font);

/**
 * Extract a letter from the font
 *
 * @param char character : character to extract
 * @param FONT* font :     font object
 **/
LETTER getLetter(char character, FONT* font);


void fontDebug(FONT* font);
void binaryPrint(unsigned char n);
void letterDebug(LETTER letter);

#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/
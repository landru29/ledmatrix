#include "font.h"
#include <malloc.h>
#include <string.h>

/**
 * Create a new font
 * 
 * @param unsigned char* data
 * @param FONT_INFO* allocationTable
 * @param char* mapping
 **/
FONT* createFont(unsigned char* data, FONT_INFO* allocationTable, char* mapping, unsigned char fontHeight)
{
	unsigned int totalLetter = strlen(mapping);
	FONT_INFO lastInfo;
	unsigned int dataSize;
	FONT* font = (FONT*)malloc(sizeof(FONT));
	
	/* Copying mapping */
	font->mapping = strdup(mapping);
	
	/* Copying allocation table */
	font->allocationTable = (FONT_INFO*) malloc(sizeof(FONT_INFO) * totalLetter);
	memcpy(font->allocationTable, allocationTable, sizeof(FONT_INFO) * totalLetter);
	
	/* Computing the length of font data */
	lastInfo = font->allocationTable[totalLetter-1];
	dataSize = lastInfo.length + lastInfo.offset;
	
	/* Copying font data */
	font->data = (unsigned char*)malloc(dataSize);
	memcpy(font->data, data, dataSize);
	
	/* Setting height and letter spacing */
	font->fontHeight = fontHeight;
	font->letterSpacing = 1;
	
	return font;
}

/**
 * Destroy the font object
 * 
 * @param FONT* font : font object to destroy
 **/
void destroyFont(FONT* font)
{
	if (!font) return;
	if (font->data)
		free(font->data);
	if (font->mapping)
		free(font->mapping);
	if (font->allocationTable)
		free(font->allocationTable);
	free(font);
}

/**
 * Extract a letter from the font
 * 
 * @param char character : character to extract
 * @param FONT* font :     font object
 **/
LETTER getLetter(char character, FONT* font)
{
	FONT_INFO info;
	LETTER letter;
	unsigned int i;
	letter.length=0;
	if (!font) return letter;
	/* Loocking for the parameter */
	for(i=0; (font->mapping[i]) && (font->mapping[i]!=character); i++);
	/* The character was not found */
	if (!font->mapping[i]) {
		letter.length = 0;
		return letter;
	}
	/* The character was found */
	info = font->allocationTable[i];
	letter.length = info.length;
	letter.data = &font->data[info.offset];
	letter.height = font->fontHeight;
	letter.spacing = font->letterSpacing;
	return letter;
}


	char* mapping;
	FONT_INFO* allocationTable;
	unsigned char* data;

void fontDebug(FONT* font)
{
	unsigned int i;
	printf(" === Debugging Font ===\n");
	printf("   - Mapping: %s\n", font->mapping);
	printf("   - Height: %d\n", font->fontHeight);
	for(i=0; font->mapping[i]; i++) {
		printf("     => %c: %d, %d\n", font->mapping[i], font->allocationTable[i].offset, font->allocationTable[i].length);
	}
}

void binaryPrint(unsigned char n)
{
	unsigned char bit = 0 ;
	unsigned char mask = 0x80 ;
	unsigned int i;
	for (i=0 ; i<8 ; i++)
	{
		bit = (n & mask) >> (7-i) ;
		printf("%c", (bit ? '@':' '));
		mask >>= 1 ;
	}
}

void letterDebug(LETTER letter)
{
	unsigned int i;
	printf(" === Debugging Letter (length: %d) ===\n", letter.length);
	for(i=0; i<letter.length; i++) {
		printf("   - data %02X ", letter.data[i]);
		binaryPrint(letter.data[i]);
		printf("\n");
	}
}

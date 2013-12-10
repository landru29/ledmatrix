/**
 * Bibliothèque de fonctions de manipulation des fontes
 *
 * @file    font.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#include "font.h"
#include <malloc.h>
#include <string.h>
#include <stdio.h>

/**
 * Create a new font
 *
 * @param data            Tableau de la fonte au format binaire
 * @param sizeTable       Tableau d'information de taille des caractères
 * @param mapping         Mapping des caractères ascii et de la fonte
 * @param fontHeight      Hauteur de la fonte
 *
 * @return Font struct
 */
FONT* createFont(unsigned char* data, unsigned int* sizeTable, char* mapping, unsigned char fontHeight)
{
    //data = oteAccents(data);
	unsigned int totalLetter = strlen(mapping);
	FONT_INFO lastInfo;
	unsigned int dataSize;
	FONT* font = (FONT*)malloc(sizeof(FONT));
	unsigned int i;
	unsigned int offset = 0;

	/* Copying mapping */
	font->mapping = strdup(mapping);

	/* Copying allocation table */
	font->allocationTable = (FONT_INFO*) malloc(sizeof(FONT_INFO) * totalLetter);
	memset(font->allocationTable, 0, sizeof(FONT_INFO) * totalLetter);
	for(i=0; mapping[i]; i++) {
		font->allocationTable[i].length = sizeTable[i];
		font->allocationTable[i].offset = offset;
		offset += sizeTable[i];
	}

	/* Computing the length of font data */
	lastInfo = font->allocationTable[totalLetter-1];
	dataSize = lastInfo.length + lastInfo.offset;

	/* Copying font data */
	font->data = (unsigned char*)malloc(dataSize);
	memset(font->data, 0, dataSize);
	memcpy(font->data, data, dataSize);

	/* Setting height and letter spacing */
	font->fontHeight = fontHeight;
	font->letterSpacing = 1;

	return font;
}

/**
 * Load a font from a font file
 *
 * @param filename Font filename
 *
 * @return Font struct
 */
FONT* loadFont(char* filename)
{
    FONT* font;
    FILE* pFile;
    char currentLetter=0;
    char* lineBuffer[200];
    size_t lineLen=0;
    unsigned char byte;
    unsigned int dataSize=0;
    int i;

    pFile = fopen (filename,"r");
    if (pFile==NULL) {
        fprintf(stderr, "Couldnot open font file %s\n", filename);
        return 0;
    }

    font = (FONT*)malloc(sizeof(FONT));
    font->mapping = (char*)malloc(1);
    font->mapping[0] = 0;
    font->data = (unsigned char*)malloc(1);
    font->fontHeight=1;
    font->letterSpacing=1;
    font->allocationTable = (FONT_INFO*)malloc(1);

    while (!feof(pFile)) {
        getline(&lineBuffer, &lineLen, pFile);
        if (lineBuffer[0]<32) {
            currentLetter = 0;
        } else if (!currentLetter) {
            currentLetter = lineBuffer[0];
        } else {
            byte = 0;
            for(i=0; (i<8) && (lineBuffer[i]); i++) {
                byte <<= 1;
                byte |= (lineBuffer[i]==1);
            }
        }
    }

    fclose (pFile);
    return font;
}

/**
 * Destroy the font object
 *
 * @param font Font object to destroy
 */
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
 * @param character Character to extract
 * @param font      Font object
 *
 * @return Letter struct
 */
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

/**
 * Debug font struct
 *
 * @param font Font struct
 */
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

/**
 * Display word to binary
 *
 * @param n word to display
 */
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

/**
 * Debug letter struct
 *
 * @param letter Letter to display
 */
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

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

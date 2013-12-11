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
    LETTER* currentLetter;
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
    font->letters = (LETTER*)malloc(1);
    font->length = 0;
    font->fontHeight=1;

    while (!feof(pFile)) {
        getline(&lineBuffer, &lineLen, pFile);
        if (lineBuffer[0]<32) {
            font->length++;
            font->letters = (LETTER*)realloc(font->letters, sizeof(LETTER)*font->length);
            currentLetter = &font->letters[font->length-1];
            currentLetter->letter = 0;
            currentLetter->data = (unsigned char*)malloc(1);
            currentLetter->height=1;
            currentLetter->length=0;
        } else if (!currentLetter) {
            currentLetter->letter = lineBuffer[0];
        } else {
            byte = 0;
            for(i=0; (i<8) && (lineBuffer[i]); i++) {
                byte <<= 1;
                byte |= (lineBuffer[i]==1);
            }
            currentLetter->length++;
            currentLetter->data = (unsigned char*)realloc(currentLetter->data, sizeof(unsigned char) * currentLetter->length);
            currentLetter->data[currentLetter->length-1] = byte;
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
	unsigned int i;
	if (!font) return;
	for(i=0; i<font->length; i++) {
        free(font->letters[i].data);
	}
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
	LETTER letter;
	unsigned int i;
	letter.length=0;
	if (!font) return letter;
	/* Loocking for the parameter */
	for(i=0; (i<font->length); i++) {
        if (font->letters[i].letter == character) {
            return font->letters[i];
        }
	}
	return letter;
}


/**
 * Debug font struct
 *
 * @param font Font struct
 */
void fontDebug(FONT* font)
{
	unsigned int i;
	printf(" === Debugging Font ===\n");
	printf("   - Length: %s\n", font->length);
	for(i=0; i<font->length; i++) {
		printf("     => %c: %d\n", font->letters[i].letter, font->letters[i].length);
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

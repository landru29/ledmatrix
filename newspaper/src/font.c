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
 * Trim a string (left and right)
 *
 * @param str string to trim (will be altered)
 *
 * @return trimmed string
 */
char* trim(char* str)
{
	unsigned int begin;
	unsigned int end;
	for(begin=0; (str[begin]) && (str[begin]<33); begin++);
	for(end=begin; (str[end]) && (str[end]>32); end++);
	str[end]=0;
	strcpy(str, &str[begin]);
	return str;
}

/**
 * Append an empty  letter to the font
 *
 * @param font Font structure
 *
 * @return the new inserted letter
 */
LETTER* appendLetter(FONT* font)
{
	LETTER* currentLetter;
	font->length++;
        font->letters = (LETTER*)realloc(font->letters, font->length*sizeof(LETTER));
        currentLetter = &font->letters[font->length-1];
        currentLetter->data = (unsigned char*)malloc(1);
	return currentLetter;
}

/**
 * Append a byte the the data array of the letter
 *
 * @param letter Letter structure
 * @param data the byte to add
 */
void appendLetterData(LETTER* letter, unsigned char data)
{
	letter->length++;
	letter->data = (unsigned char*)realloc(letter->data, sizeof(unsigned char)*letter->length);
	letter->data[letter->length-1] = data;
}

/**
 * Load a font from a font file
 *
 * @param filename Font filename
 *
 * @return Font struct
 */
FONT* loadFont(char* filename) {
        FONT* font = (FONT*)malloc(sizeof(FONT));
        LETTER* currentLetter;
        font->length=0;
        font->fontHeight=1;
        char* buffer = (char*)malloc(200);
        size_t len;
	unsigned int i;
	unsigned char byte;
        FILE* f = fopen(filename, "r");
        while (!feof(f)) {
                getline(&buffer, &len, f);
                trim(buffer);
                if (strlen(buffer)>0) {
                        fprintf(stdout, "%s\n", buffer);
                        switch (buffer[0]) {
                                case '[':
                                        currentLetter = appendLetter(font);
                                        currentLetter->letter = buffer[1];
                                        break;
                                case 'b':
					for(i=1; (i<9) && (buffer[i]); i++) {
                				byte <<= 1;
                				byte |= (buffer[i]=='1');
            				}
					appendLetterData(currentLetter, byte);
                                        break;
                                default: break;
                        }
                }
        }
        fclose(f);
        free(buffer);
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

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


void fontDebug(FONT* font);
void letterDebug(LETTER letter);
unsigned char fromBinary(char* binary);

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
	for(end=strlen(str); (str[end]) && (end>0); end--);
	str[end+1]=0;
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
	if (font->length>1) {
        font->letters = (LETTER*)realloc(font->letters, font->length*sizeof(LETTER));
	} else {
        font->letters = (LETTER*)malloc(font->length*sizeof(LETTER));
    }
    currentLetter = &font->letters[font->length-1];
    currentLetter->data = (unsigned char*)malloc(1);
    currentLetter->length=0;
    currentLetter->spacing=1;
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
    LETTER* currentLetter=0;
    font->length=0;
    font->fontHeight=1;
    char* buffer = (char*)malloc(200);
    size_t len;
	unsigned int i;
    FILE* f = fopen(filename, "r");
	if (!f) {
		fprintf(stderr, "  * Could not open font %s\n", filename);
		return 0;
	}
	fprintf(stdout, "  * Opening font %s", filename);
	fflush(stdout);
        while (!feof(f)) {
                getline(&buffer, &len, f);
                trim(buffer);
                if (strlen(buffer)>0) {
                        switch (buffer[0]) {
                                case '[':
                                    currentLetter = appendLetter(font);
                                    currentLetter->letter = buffer[1];
                                    break;
                                case 'b':
                                    appendLetterData(currentLetter, fromBinary(&buffer[1]));
                                    break;
                                default: break;
                        }
                }
        }
        fclose(f);
        free(buffer);
	fprintf(stdout, " => %d characteres loaded\n", font->length);
        //fontDebug(font);
        //LETTER letterTest = getLetter('c', font);
        //letterDebug(letterTest);
	return font;
}

/**
 * Convert a binary writing in a byte
 *
 * @param binary binary representation
 *
 * @return byte
 */
unsigned char fromBinary(char* binary)
{
    unsigned char byte = 0;
    unsigned char i;
    for(i=0; (i<8) && (binary[i]!=0); i++) {
        byte <<= 1;
        byte |= (binary[i]=='1');
    }
    return byte;
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
            font->letters[i].height = font->fontHeight;
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
    unsigned int i, j;
    printf(" === Debugging Font ===\n");
    printf("   - Length: %d\n", font->length);
    printf("   - height: %d\n", font->fontHeight);
    for(i=0; i<font->length; i++) {
        letterDebug(font->letters[i]);
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
    printf("     => %c: %d [sp:%d]--> ", letter.letter, letter.length, letter.spacing);
    for(i=0; i<letter.length; i++)
        printf("%02X ", letter.data[i]);
    printf("\n");
}

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

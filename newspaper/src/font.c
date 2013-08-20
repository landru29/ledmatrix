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
#include <wchar.h>
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
FONT* createFont(char* data, unsigned int* sizeTable, wchar_t* mapping, unsigned char fontHeight)
{
	mapping = oteAccents(mapping);
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

/* sansAccent : transforme un caractère accentué en la même lettre mais
 * sans accent (é -> e, ç -> c, À -> A, ...)
 * Entrées : le caractère
 * Sorties : le même sans accent, ou le même s'il n'avait pas d'accent */
wchar_t sansAccent(wchar_t c)
{
  switch (c)
  {
      case L'à': case L'á': case L'â': case L'ã': case L'ä': case L'å':
          return L'a';
      case L'À': case L'Á': case L'Â': case L'Ã': case L'Ä': case L'Å':
          return L'A';
      case L'ç':
          return L'c';
      case L'Ç':
          return L'C';
      case L'é': case L'è': case L'ê': case L'ë':
          return L'e';
      case L'É': case L'È': case L'Ê': case L'Ë':
          return L'E';
      case L'ì': case L'í': case L'î': case L'ï':
          return L'i';
      case L'Ì': case L'Í': case L'Î': case L'Ï':
          return L'I';
      case L'ñ':
          return L'n';
      case L'Ñ':
          return L'N';
      case L'ò': case L'ó': case L'ô': case L'õ': case L'ö':
          return L'o';
      case L'Ò': case L'Ó': case L'Ô': case L'Õ': case L'Ö':
          return L'O';
      case L'ù': case L'ú': case L'û': case L'ü':
          return L'u';
      case L'Ù': case L'Ú': case L'Û': case L'Ü':
          return L'U';
      case L'ý': case L'ÿ':
          return L'y';
      case L'Ý':
          return L'Y';
      default:
          return c;
    }
}

/* On enlèveles accent dans la chaine */
void oteAccents(wchar_t* j)
{
    int i;
    i = 1 ;
    while ( j[i] != '\0' ) {
        j[i] = sansAccent(j[i]);
        i = i + 1;
    }
    j[i-1] = '\0';
    return j;
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

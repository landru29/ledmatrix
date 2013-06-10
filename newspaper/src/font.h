/**
 * Bibliothèque de fonctions de manipulation des fontes
 *
 * @file    font.h
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#ifndef __FONT_H
#define __FONT_H

#include <stdint.h>

/**
 * Structure d'information d'un caractère de la fonte
 */
typedef struct {
  unsigned char length;
  uint16_t offset;
} FONT_INFO;

/**
 * Structure de la fonte
 */
typedef struct {
	char* mapping;
	FONT_INFO* allocationTable;
	unsigned char* data;
	unsigned char fontHeight;
	unsigned char letterSpacing;
} FONT;

/**
 * Structure d'un caractère de la fonte
 */
typedef struct {
	unsigned char length;
	unsigned char height;
	unsigned char* data;
	unsigned char spacing;
} LETTER;


/**
 * Create a new font
 *
 * @param data            Tableau de la fonte au format binaire
 * @param allocationTable Tableau d'information de positionnement et de taille des caractères
 * @param mapping         Mapping des caractères ascii et de la fonte
 * @param fontHeight      Hauteur de la fonte
 *
 * @return Font struct
 */
FONT* createFont(unsigned char* data, unsigned int* sizeTable, char* mapping, unsigned char fontHeight);

/**
 * Destroy the font object
 *
 * @param font Font object to destroy
 */
void destroyFont(FONT* font);

/**
 * Extract a letter from the font
 *
 * @param character Character to extract
 * @param font      Font object
 *
 * @return Letter struct
 */
LETTER getLetter(char character, FONT* font);

/**
 * Debug font struct
 *
 * @param font Font struct
 */
void fontDebug(FONT* font);

/**
 * Display word to binary
 *
 * @param n word to display
 */
void binaryPrint(unsigned char n);

/**
 * Debug letter struct
 *
 * @param letter Letter to display
 */
void letterDebug(LETTER letter);

#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

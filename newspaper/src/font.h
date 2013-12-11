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
#include "../config.h"

/**
 * Structure d'un caractère de la fonte
 */
typedef struct {
    char letter;
	unsigned char length;
	unsigned char height;
	unsigned char* data;
	unsigned char spacing;
} LETTER;

/**
 * Structure de la fonte
 */
typedef struct {
    LETTER* letters;
    unsigned int length;
    unsigned char fontHeight;
} FONT;

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
 * Load a font from a font file
 *
 * @param filename Font filename
 *
 * @return Font struct
 */
FONT* loadFont(char* filename);

#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

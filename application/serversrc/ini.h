/**
 * Bibliothèque de fonctions de lecture de fichier ini
 *
 * @file    ini.h
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */
#ifndef __INI_H
#define __INI_H

typedef struct {
    char name[80];
    char value[200];
} INI_LINE;


/**
 * Get the value of a key
 *
 * @param data data holding the configuration
 * @param keyName name of the configuration key
 *
 * @return value of the key
 */
char* iniGet(INI_LINE* data, char* keyName);

/**
 * Get the size of the ini
 *
 * @param data data holding the configuration
 *
 * @return size of the configuration
 */
unsigned int iniSize(INI_LINE* data);

/**
 * Get the key at position n
 *
 * @param data data holding the configuration
 * @param n index of the key to retrieve
 *
 * @return name of the key
 */
char* iniKey(INI_LINE* data, unsigned int n);

/**
 * Check if a key exists
 *
 * @param data data holding the configuration
 * @param keyName name of the configuration key
 *
 * @return boolean whether then key was found
 */
int iniHasKey(INI_LINE* data, char* keyName);

/*
 * Parse a ini file
 *
 * @param filename ini file
 *
 * @return data holding the configuration
 */
INI_LINE* iniParse(char* filename);

/*
 * Release the memory
 *
 * @param data data holding the configuration
 *
 * @return void
 */
void iniDestroy(INI_LINE* data);


#endif

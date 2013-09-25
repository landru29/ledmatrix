/**
 * Bibliothèque de fonctions de lecture de fichier ini
 *
 * @file    ini.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */


#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "ini.h"

/**
 * Get the value of a key
 *
 * @param data data holding the configuration
 * @param keyName name of the configuration key
 *
 * @return value of the key
 */
char* iniGet(INI_LINE* data, char* keyName)
{
    unsigned int i;
    if ((!keyName) || (!data))
        return 0;
    for(i=0; data[i].name[0]; i++)
        if (strcmp(keyName, data[i].name) == 0)
            return data[i].value;
    return 0;
}

/**
 * Get the size of the ini
 *
 * @param data data holding the configuration
 *
 * @return size of the configuration
 */
unsigned int iniSize(INI_LINE* data)
{
    unsigned int i;
    if (!data) return 0;
    for(i=0; data[i].name[0]; i++);
    return i;
}

/**
 * Get the key at position n
 *
 * @param data data holding the configuration
 * @param n index of the key to retrieve
 *
 * @return name of the key
 */
char* iniKey(INI_LINE* data, unsigned int n)
{
    unsigned int len = iniSize(data);
    if (n<len)
        return data[n].name;
    return 0;
}

/**
 * Check if a key exists
 *
 * @param data data holding the configuration
 * @param keyName name of the configuration key
 *
 * @return boolean whether then key was found
 */
int iniHasKey(INI_LINE* data, char* keyName)
{
    unsigned int i;
    unsigned int len = iniSize(data);
    for (i=0; i<len; i++)
        if (strcmp(data[i].name, keyName)==0)
            return 1;
    return 0;
}

/*
 * Parse a ini file
 *
 * @param filename ini file
 *
 * @return data holding the configuration
 */
INI_LINE* iniParse(char* filename)
{
    FILE* f;
    INI_LINE* buffer;
    unsigned int count = 0;
    int i, j;
    char line[500];
    char* dataLine;
    unsigned int dataSize;
    f = fopen(filename, "r");
    if (f==0) return 0;
    buffer = (INI_LINE*)malloc(0);
    while (!feof(f)) {
        /* read line from file */
        if (fgets(line, 500, f)) {
            /* trim the end of the line */
            for(i=strlen(line); (i>=0) && (line[i]<=32); i--)
                line[i]=0;
            /* trim the beginning of the file */
            if (line[0]<=32)
                for(dataLine = line; (*dataLine) && (*dataLine>32); dataLine++) ;
            else
                dataLine = line;
             /* check if the line is a comment */
            if ((dataLine[0] == ';') || (dataLine[0] == '#') || (dataLine[0]==0))
                continue;
            /* extend the result */
            buffer = (INI_LINE*) realloc(buffer, sizeof(INI_LINE) * (++count));
            /* detect the name */
            dataSize = strlen(dataLine);
            for(i=0; (dataLine[i]) && (dataLine[i] != '='); i++);
            dataLine[i]=0;
            /* trim the name */
            for(j=strlen(dataLine); j>=0; j--)
                if (dataLine[j]<=32) dataLine[j]=0;
            strcpy(buffer[count-1].name, dataLine);
            /* detect and trim the value */
            for(i++;(i<dataSize) && (dataLine[i]<=32); i++);
            strcpy(buffer[count-1].value, &dataLine[i]);
        }
    }
    fclose(f);
    /* close the databuffer with a zero */
    buffer = (INI_LINE*) realloc(buffer, sizeof(INI_LINE) * (++count));
    buffer[count-1].name[0] = 0;
    return buffer;
}

/*
 * Release the memory
 *
 * @param data data holding the configuration
 *
 * @return void
 */
void iniDestroy(INI_LINE* data)
{
    free(data);
}

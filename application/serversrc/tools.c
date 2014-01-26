/**
 * Bibliothèque de fonctions générales
 *
 * @file    tools.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#include <stdio.h>
#include <string.h>


/**
 * Donne l'extention d'un fichier
 *
 * @param  filename      nom du fichier
 *
 * @return pointer sur l'extension du fichier
 **/
char* getExtension(char* filename)
{
	unsigned int i;
	for(i=strlen(filename); (i>0) && (filename[i-1] != '.'); i--) ;
	return &filename[i];
}

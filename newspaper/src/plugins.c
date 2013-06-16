/**
 * Bibliothèque de chargement des plugins
 *
 * @file    plugins.c
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <dirent.H>
#include <sys/types.h>
#include "plugins.h"

/**
 * Donne l'extention d'un fichier
 *
 * @param  filename      nom du fichier
 *
 * @return pointer sur l'extension du fichier
 */
char* getExtension(char* filename) {
	unsigned int i;
	for(i=strlen(filename)-1; (i>=0) && (filename[i] != '.'; i--) ;
	return &filename[i+1];
}



/**
 * Charge tous les plugins d'animation
 *
 * @param  path      chemin du répertoire contenant les plugins
 *
 * @return tableau de plugins (terminé par un nom null)
 */
ANIMATIONPLUGIN* loadPlugins(char* path)
{
	ANIMATIONPLUGIN* temp;
	DIR* dir;
	struct dirent* entrees;
	/* initialize result */
	temp = (ANIMATIONPLUGIN*)malloc(sizeof(ANIMATIONPLUGIN));
	temp->name = 0;
	/* Ppening plugin folder */
	if ((dir = opendir(path)) == 0) {
		fprintf(stderr, "Could not find plugin path %s\n", path);
		return temp;
	}
	/* Scan folder */
	while ((entree = readdir(dir)) && (entree->d_type==0x08)) {
		if (strcpm(getExtension(entree->d_name), "so") == 0) {
			printf("Loading %s\n", entree->d_name);
		}
	}
	return temp;
}



/* vim: set expandtab ai ts=4 sw=4 nu:
*/

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
#include <dirent.h>
#include <sys/types.h>
#include <dlfcn.h>
#include "plugins.h"
#ifdef __arm__
#include "display.h"
#endif
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
	for(i=strlen(filename)-1; (i>=0) && (filename[i] != '.'); i--) ;
	return &filename[i+1];
}

/**
 * Définit les functions à passer au plugin
 *
 * @return tableau de fonctions (terminé par null)
 **/
SHAREDFUNCTION** getHostFunctions()
{
	SHAREDFUNCTIONLIST temp = initializeFunctionList();
	temp = appendFunction(temp, "matrixSendViewport", (shared_function)matrixSendViewport);
	temp = appendFunction(temp, "matrixClearViewport", (shared_function)matrixClearViewport);
#ifdef __arm__
	temp = appendFunction(temp, "displayBlink", (shared_function)displayBlink);
#endif
	return temp;
}

/**
 * Charge tous les plugins d'animation
 *
 * @param  path      chemin du répertoire contenant les plugins
 *
 * @return tableau de plugins (terminé par null)
 **/
ANIMATIONPLUGIN** loadPlugins(char* path)
{
	ANIMATIONPLUGIN** temp;
	DIR* dir;
	struct dirent* entrees;
	int isPathClean = 1;
	char pluginPath[2000];
	void *plugin;
	init_function initFunction;
	unsigned int nbPlugins = 0;
	SHAREDFUNCTION** sharedFunctions = getHostFunctions();
	
	/* initialize result */
	temp = (ANIMATIONPLUGIN**)malloc(sizeof(ANIMATIONPLUGIN*));
	temp = 0;
	/* Opening plugin folder */
	if (path[strlen(path)-1] == '/') isPathClean=0;
	printf("Loading plugins in %s\n", path);
	if ((dir = opendir(path)) == 0) {
		fprintf(stderr, "Could not find plugin path %s\n", path);
		return temp;
	}
	/* Scan folder */
	while (entrees = readdir(dir)) {
		/* Check if this file is interesting */
		if (strcmp(getExtension(entrees->d_name), "so") != 0) continue;
		/* Now, we have a plugin filename */
		sprintf(pluginPath, (isPathClean ? "%s/%s" : "%s%s"), path, entrees->d_name);
		printf("  * Found %s", entrees->d_name);
		fflush(stdout);
		/* Loading the plugin */
		plugin = dlopen(pluginPath, RTLD_NOW);
		if (plugin) {
			printf(" => loaded\n");
			/* handle the init function */
			initFunction = (init_function)dlsym (plugin, "init");
			/* resize the table */
			temp = (ANIMATIONPLUGIN**)realloc(temp, sizeof(ANIMATIONPLUGIN*)*(nbPlugins+2));
			/* Add the new plugin */
			temp[nbPlugins] = (ANIMATIONPLUGIN*)initFunction(sharedFunctions);
			/* register the plugin */
			temp[nbPlugins]->_pluginHdl = plugin;
			/* terminate the table with a 0 */
			temp[nbPlugins+1] = 0;
			nbPlugins++;
		} else {
			fprintf(stderr, " => Error: %s\n", dlerror());
		}
	}
	return temp;
}

/**
 * Décharge les plugins et libère la mémoire
 *
 * @param  plugins      tableau de plugins (terminé par null)
 *
 **/
void closePlugins(ANIMATIONPLUGIN** plugins)
{
	unsigned int i;
	for(i=0; plugins[i]; i++) {
		dlclose(plugins[i]->_pluginHdl);
		free(plugins[i]->name);
		free(plugins[i]);
	}
	free(plugins);
}

/**
 * Récupère un plugin par son nom
 *
 * @param  plugins   tableau de plugins (terminé par null)
 * @param  name      nom du plugin
 *
 * @return plugin d'animation
 */
ANIMATIONPLUGIN* getPluginAnimation(ANIMATIONPLUGIN** plugins, char* name)
{
	unsigned int i;
	for(i=0; plugins[i]; i++) {
		if (strcmp(plugins[i]->name, name) == 0)
			return plugins[i];
	}
	return 0;
}


/* vim: set expandtab ai ts=4 sw=4 nu:
*/

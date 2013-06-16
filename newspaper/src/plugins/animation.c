#include "animation.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>


/**
 * recherche une fonction hôte
 *
 * @param  hostFunctions  table des fonctions hôtes
 * @param  name           nom de la fonction
 *
 * @return fonction
 */
void* getHostFunction(HOSTFUNCTION** hostFunctions, char* name)
{
	unsigned int i;
	for(i=0; hostFunctions[i]; i++)
		if (strcmp(name, hostFunctions[i]->name) == 0)
			return hostFunctions[i]->runtime;
	fprintf(stderr, "Could not find function %s\n", name);
	return 0;
}

/**
 * Crée une liste vide de fonctions
 *
 * @return liste de fonction
 */
HOSTFUNCTIONLIST initializeFunctionList()
{
	HOSTFUNCTIONLIST temp;
	temp = (HOSTFUNCTIONLIST)malloc(sizeof(HOSTFUNCTION*));
	temp[0] = 0;
	return temp;
}


/**
 * Crée une liste vide de fonctions
 *
 * @param plugin    plugin
 * @param name      nom de la fonction
 * @param function  fonction à ajouter
 * 
 */
void pluginAppendFunction(ANIMATIONPLUGIN* plugin, char* name, host_function function)
{
	plugin->functions = appendFunction(plugin->functions, name, function);
}

/**
 * Ajoute une fonction à la liste
 *
 * @param liste     liste de fonctions
 * @param name      nom de la fonction
 * @param function  fonction à ajouter
 * 
 */
HOSTFUNCTIONLIST appendFunction(HOSTFUNCTIONLIST list, char* name, host_function function)
{
	unsigned int len;
	HOSTFUNCTION* temp;
	HOSTFUNCTIONLIST newList;
	/* get the length of the list */
	for(len=0; list[len]; len++);
	/* enlarge the table */
	newList = (HOSTFUNCTION**)realloc(list, sizeof(HOSTFUNCTION*) * (len+2));
	newList[len+1]=0;
	/* Add the new function */
	temp = (HOSTFUNCTION*)malloc(sizeof(HOSTFUNCTION));
	newList[len]=temp;
	temp->name = strdup(name);
	temp->runtime = function;
	return newList;
}



/**
 * Initialise un plugin
 * 
 * @param name        nom du plugin
 * @param animation   fonction principale de l'animation
 * @param createUser  fonction d'initialisation de données utilisables dans l'animation
 * @param destroyUser fonction de destruction de données utilisables dans l'animation
 **/
ANIMATIONPLUGIN* createAnimationPlugin(char* name, animation_function animation, userdata_function createUser, userdata_function destroyUser)
{
	ANIMATIONPLUGIN* temp = (ANIMATIONPLUGIN*)malloc(sizeof(ANIMATIONPLUGIN));
	temp->name = strdup(name);
	temp->runtime = animation;
	temp->creation = createUser;
	temp->destruction = destroyUser;
	temp->functions = initializeFunctionList();
	return temp;
}

/**
 * recherche une fonction du plugin
 *
 * @param  plugin         plugin
 * @param  name           nom de la fonction
 *
 * @return fonction
 */
void* getPluginFunction(ANIMATIONPLUGIN* plugin, char* name)
{
	return getHostFunction(plugin->functions, name);
}

/**
 * liste les fonctions spécifiques du plugin
 *
 * @param plugin    plugin
 * 
 */
void pluginListFunctions(ANIMATIONPLUGIN* plugin)
{
	listFunctions(plugin->functions);
}

/**
 * liste les fonctions spécifiques du plugin
 *
 * @param liste     liste de fonctions
 * 
 */
void listFunctions(HOSTFUNCTIONLIST list)
{
	unsigned int len;
	for(len=0; list[len]; len++) {
		fprintf(stdout, "%s(...);\n", list[len]->name);
	}
}

/**
 * plugin d'animations
 *
 * @file    animation.h
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#ifndef __ANIMATION_PLUGIN_H_
#define __ANIMATION_PLUGIN_H_

#include "../animate.h"
#include "../../config.h"
#include "../ledmatrix.h"


typedef int (*host_function) ();

typedef int (*animation_function) ();

typedef int (*plugin_function) ();

typedef void* (*userdata_function) ();


typedef struct {
	char* name;
	host_function runtime;
} HOSTFUNCTION;

typedef HOSTFUNCTION** HOSTFUNCTIONLIST;

typedef struct {
	char* name;
	animation_function runtime;
	userdata_function creation;
	userdata_function destruction;
	HOSTFUNCTIONLIST functions;
	void* _pluginHdl;
} ANIMATIONPLUGIN;

typedef ANIMATIONPLUGIN* (*init_function) ();



/**
 * recherche une fonction hôte
 *
 * @param  hostFunctions  table des fonctions hôtes
 * @param  name           nom de la fonction
 *
 * @return fonction
 */
void* getHostFunction(HOSTFUNCTION** hostFunctions, char* name);

/**
 * recherche une fonction du plugin
 *
 * @param  hostFunctions  table des fonctions hôtes
 * @param  name           nom de la fonction
 *
 * @return fonction
 */
void* getPluginFunction(ANIMATIONPLUGIN* plugin, char* name);

/**
 * Crée une liste vide de fonctions
 *
 * @return liste de fonction
 */
HOSTFUNCTIONLIST initializeFunctionList();

/**
 * Ajoute une fonction au plugin
 *
 * @param plugin    plugin
 * @param name      nom de la fonction
 * @param function  fonction à ajouter
 * 
 */
void pluginAppendFunction(ANIMATIONPLUGIN* plugin, char* name, host_function function);

/**
 * liste les fonctions spécifiques du plugin
 *
 * @param plugin    plugin
 * 
 */
void pluginListFunctions(ANIMATIONPLUGIN* plugin);

/**
 * liste les fonctions spécifiques du plugin
 *
 * @param liste     liste de fonctions
 * 
 */
void listFunctions(HOSTFUNCTIONLIST list);

/**
 * Ajoute une fonction à la liste
 *
 * @param liste     liste de fonctions
 * @param name      nom de la fonction
 * @param function  fonction à ajouter
 * 
 */
HOSTFUNCTIONLIST appendFunction(HOSTFUNCTIONLIST list, char* name, host_function function);

/**
 * Initialise un plugin
 * 
 * @param name        nom du plugin
 * @param animation   fonction principale de l'animation
 * @param createUser  fonction d'initialisation de données utilisables dans l'animation
 * @param destroyUser fonction de destruction de données utilisables dans l'animation
 **/
ANIMATIONPLUGIN* createAnimationPlugin(char* name, animation_function animation, userdata_function createUser, userdata_function destroyUser);

#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

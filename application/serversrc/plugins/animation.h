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

#define ANIMATION_FAILURE -1
#define ANIMATION_SUCCESS 0
#define ANIMATION_END 1

//#include "../animate.h"
#include "../../config.h"
#include "../ledmatrix.h"

/**
 * Fonction partagée
 **/
typedef void* (*shared_function) ();

/**
 * Description d'une fonction d'échange
 **/
typedef struct {
	char* name;
	shared_function runtime;
} SHAREDFUNCTION;

/**
 * Liste de fonctions d'échange
 **/
typedef SHAREDFUNCTION** SHAREDFUNCTIONLIST;

/**
 * Plugin
 **/
typedef struct {
	char* name;
	shared_function runtime;
	shared_function creation;
	shared_function destruction;
	SHAREDFUNCTIONLIST functions;
	void* _pluginHdl;
} ANIMATIONPLUGIN;


/**
 * Fonction d'initialisation de plugin
 **/
typedef ANIMATIONPLUGIN* (*init_function) ();



/**
 * recherche une fonction hôte
 *
 * @param  sharedFunctions  table des fonctions hôtes
 * @param  name           nom de la fonction
 *
 * @return fonction
 */
void* getHostFunction(SHAREDFUNCTION** sharedFunctions, char* name);

/**
 * recherche une fonction du plugin
 *
 * @param  sharedFunctions  table des fonctions hôtes
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
SHAREDFUNCTIONLIST initializeFunctionList();

/**
 * Ajoute une fonction au plugin
 *
 * @param plugin    plugin
 * @param name      nom de la fonction
 * @param function  fonction à ajouter
 *
 */
void pluginAppendFunction(ANIMATIONPLUGIN* plugin, char* name, shared_function function);

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
 * @param list      liste de fonctions
 *
 */
void listFunctions(SHAREDFUNCTIONLIST list);

/**
 * Ajoute une fonction à la liste
 *
 * @param list      liste de fonctions
 * @param name      nom de la fonction
 * @param function  fonction à ajouter
 *
 */
SHAREDFUNCTIONLIST appendFunction(SHAREDFUNCTIONLIST list, char* name, shared_function function);

/**
 * Initialise un plugin
 *
 * @param name        nom du plugin
 * @param animation   fonction principale de l'animation
 * @param createUser  fonction d'initialisation de données utilisables dans l'animation
 * @param destroyUser fonction de destruction de données utilisables dans l'animation
 **/
ANIMATIONPLUGIN* createAnimationPlugin(char* name, shared_function animation, shared_function createUser, shared_function destroyUser);

#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

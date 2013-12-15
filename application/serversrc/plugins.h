/**
 * Bibliothèque de fonctions d'animations
 *
 * @file    animationpool.h
 * @author  Cyrille Meichel <cmeichel@digitaleo.com>
 * @author  Manuel Hervo <mhervo@digitaleo.com>
 * @version 0.1
 */

#ifndef __PLUGINS_H_
#define __PLUGINS_H_
#include "../config.h"
#include "plugins/animation.h"

int exitCondition;

/**
 * Charge tous les plugins d'animation
 *
 * @param  path      chemin du répertoire contenant les plugins
 *
 * @return tableau de plugins (terminé par un nom null)
 */
ANIMATIONPLUGIN** loadPlugins(char* path);

/**
 * Décharge les plugins et libère la mémoire
 *
 * @param  plugins      tableau de plugins (terminé par null)
 *
 **/
void closePlugins(ANIMATIONPLUGIN** plugins);

/**
 * Récupère un plugin par son nom
 *
 * @param  name      nom du plugin
 *
 * @return plugin d'animation
 */
ANIMATIONPLUGIN* getPluginAnimation(ANIMATIONPLUGIN** plugins, char* name);

#endif

/* vim: set expandtab ai ts=4 sw=4 nu:
*/

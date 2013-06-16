#include "animation.h"


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
	return 0;
}

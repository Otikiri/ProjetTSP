#ifndef P2_2OPT_H
#define P2_2OPT_H

#include <stdbool.h>
#include "struct.h"

/**
 * @brief améliore une tournee déjà calculé
 * @param tournee tournee déjà calculé (généralement avec randomwalk ou nearestneighbour)
 * @param tourneeLen Pointeur sur double ou l'on veut stocker la longueur de la meilleure tournee
 * @param f fonction qui seras utilise pour le calcul de distance (att,geu ou euc_2d)
 * @return int 0 si tout se passe bien -1 en cas d'erreur
 */
int tsp_2opt(Tournee* tournee, double* tourneeLen, DistanceFun f);


#endif
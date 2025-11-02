#ifndef P2_HEURISTIQUES_H
#define P2_HEURISTIQUES_H

#include "struct.h"

/**
 * @brief calcule une tournee en utilisant l'heuristique du randomWalk
 * @param graphe graphe d'un fichier tsp
 * @param f fonction qui seras utilise pour le calcul de distance (att,geu ou euc_2d)
 * @param outTournee Pointeur sur une tournee ou l'on veut stocker la meilleure tournee
 * @param outLen Pointeur sur double ou l'on veut stocker la longueur de la meilleure tournee
 * @return int 0 si tout se passe bien -1 en cas d'erreur
 */
int tsp_randomWalk(const Graphe* graphe, DistanceFun f, Tournee* outTournee, double* outLen);

/**
 * @brief calcule une tournee en utilisant l'heuristique du nearest neighbour
 * @param graphe graphe d'un fichier tsp
 * @param f fonction qui seras utilise pour le calcul de distance (att,geu ou euc_2d)
 * @param outTournee Pointeur sur une tournee ou l'on veut stocker la meilleure tournee
 * @param outLen Pointeur sur double ou l'on veut stocker la longueur de la meilleure tournee
 * @return int 0 si tout se passe bien -1 en cas d'erreur
 */

int tsp_nearestNeighbour(const Graphe* graphe, DistanceFun f, Tournee* outTournee, double* outLen);




#endif
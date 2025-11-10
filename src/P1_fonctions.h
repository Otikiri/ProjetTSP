#ifndef P1_FONCTIONS_H
#define P1_FONCTIONS_H

#include <stdbool.h>
#include "struct.h"
#include "matrice.h"

#define WITH_MATRICE 1
#define WITHOUT_MATRICE 0


/**
 * @brief Fonction appliquant l'algorithme brute force au graphe passee en parametre et retournant la meilleure/pire tournee et sa longueur
 * @param g Graphe sur lequel appliquer l'algorithme
 * @param f fonction de calcul de distance que l'on veut utiliser
 * @param faire_matrice_distance Booleen dictant si l'on veut creer une matrice de distance
 * @param outBest Pointeur sur une tournee ou l'on veut stocker la meilleure tournee
 * @param outBestLen Pointeur sur double ou l'on veut stocker la longueur de la meilleure tournee
 * @param outWorst Pointeur sur une tournee ou l'on veut stocker la pire tournee
 * @param outWorstLen Pointeur sur double ou l'on veut stocker la longueur de la pire tournee
 * @return int: -1 si erreur, 0 sinon
 */
int tsp_bruteforce(const Graphe* g, DistanceFun f, bool faire_matrice_distance,
                    Tournee* outBest, double* outBestLen, Tournee* outWorst, double* outWorstLen);


typedef double (*DistanceCalc)(const Graphe* g, DistanceFun f, int i, int j, Matrice m);

int fill_tournee_from_perm(const Graphe* g, const int* perm, int n, Tournee* out);

#endif
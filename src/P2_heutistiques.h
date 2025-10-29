#ifndef P2_HEURISTIQUES_H
#define P2_HEURISTIQUES_H

#include "struct.h"

int tsp_randomWalk(const Graphe* graphe, DistanceFun f, Tournee* outTournee, double* outLen);

int tsp_nearestNeighbour(const Graphe* graphe, DistanceFun f, Tournee* outTournee, double* outLen);




#endif
#include "P2_heutistiques.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "calc_distance.h"

int tsp_randomWalk(const Graphe* graphe, DistanceFun f, Tournee* outTournee, double* outLen){
    if (!graphe || graphe->dimension <= 0 || !outTournee || !outLen) return -1;
    // Initialisation du randomizer en utilisant le moment actuel en tant que seed
    srand(time(NULL));

    // Tableau stockant les entiers deja tires au sort
    int* tabOldIndice = malloc(sizeof(int) * graphe->dimension);
    int nbOldIndice = 0;
    int randomIndice;

    // Rempli outTournee aleatoirement
    for (int i = 0; i < graphe->dimension; i++){
        do{
            randomIndice = rand() % graphe->dimension; // Entier random appartenant a [0, graphe->dimension[
        } while (!isIn(randomIndice, tabOldIndice, nbOldIndice)); // Pour avoir un point qui n'a pas deja ete tire au sort

        // Actualise le tableau des indices deja tires au sort, et incremente sa taille
        tabOldIndice[i] = randomIndice;
        nbOldIndice++;
        // Ajoute le point choisi a outTournee
        outTournee->ord_point_vis[i] = graphe->point[randomIndice];
    }

    *outLen = calcul_poids_tournee(outTournee, f);
    free(tabOldIndice);
    return 0;
}

bool isIn(int n, int* tab, int tabLen){
    for (int i = 0; i < tabLen; i++){
        if (n == tab[i]) return true;
    }
    return false;
}
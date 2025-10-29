#include "P2_heuristiques.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "calc_distance.h"

bool isIn(int n, int* tab, int tabLen);

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

int tsp_nearestNeighbour(const Graphe* graphe, DistanceFun f, Tournee* outTournee, double* outLen) {
    if (!graphe || graphe->dimension <= 0 || !outTournee || !outLen) return -1;
    outTournee->ord_point_vis[0]=graphe->point[0];

    int* tabOldIndice = malloc(sizeof(int) * graphe->dimension);
    tabOldIndice[0] = 0;
    int nbOldIndice=1;
    int lastIndice =0;
    double distance_min=-1;
    int fin=-1;

    for (int j=1;j<graphe->dimension;j++) {
        for (int i =1;i<graphe->dimension;i++) {
            if (!isIn(i, tabOldIndice, nbOldIndice)){
                double distance_temp=f(graphe->point[lastIndice],graphe->point[i]);
                if (distance_min==-1||distance_temp<distance_min) {
                    distance_min=distance_temp;
                    fin=i;
                }
            }
        }
        tabOldIndice[nbOldIndice]=fin;
        outTournee->ord_point_vis[nbOldIndice]=graphe->point[fin];
        nbOldIndice++;
        lastIndice=fin;
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
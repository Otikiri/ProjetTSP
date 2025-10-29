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
        } while (isIn(randomIndice, tabOldIndice, nbOldIndice)); // Pour avoir un point qui n'a pas deja ete tire au sort

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

    //Choix arbitraire d'un point de départ (ici 0 )
    outTournee->ord_point_vis[0]=graphe->point[0];
    int lastIndice =0;// dernier point de la tournee

    // Tableau stockant les entiers deja utilisé dont 0
    int* tabOldIndice = malloc(sizeof(int) * graphe->dimension);
    tabOldIndice[0] = 0; //rajout de 0
    int nbOldIndice=1; //nbr d'élément dans le tableau

    //Traitement principal
    for (int j=1;j<graphe->dimension;j++) {//j=1 car on a n-1 calcul à faire

        int fin=-1; //init deindice de l'élément qui seras choisis
        double distance_min=-1; //init de la plus petite distance trouvé 

        //Trouve le point qui a la plus petite distance avec lastIndice et le met dans fin
        for (int i =1;i<graphe->dimension;i++) {// j=1 car on a choisi arbitrairement le point 0 en premier

            if (!isIn(i, tabOldIndice, nbOldIndice)){//verifie que i n'est pas un point déja rajouté à la tournee

                double distance_temp=f(graphe->point[lastIndice],graphe->point[i]);
                if (distance_min==-1||distance_temp<distance_min) {//verifie si on a pas encore de distance min ou si actuel<min
                    distance_min=distance_temp;
                    fin=i;
                }
            }
        }

        //traitement du point trouvé
        tabOldIndice[nbOldIndice]=fin;
        outTournee->ord_point_vis[nbOldIndice]=graphe->point[fin];
        nbOldIndice++;
        lastIndice=fin;
    }
    //calcul de outlen
    *outLen = calcul_poids_tournee(outTournee, f);
    //libère la mémoire
    free(tabOldIndice);
    return 0;
}

/**
 * @brief vérifie si l'entier n et dans le tableau tab
 * @param n indice d'un point
 * @param tab tableau d'indice
 * @param tabLen longueur du tableau 
 * @return true si n est dans tab et false sinon
 */
bool isIn(int n, int* tab, int tabLen){
    //Parcours du tableau
    for (int i = 0; i < tabLen; i++){
        if (n == tab[i]) return true;
    }
    return false;
}
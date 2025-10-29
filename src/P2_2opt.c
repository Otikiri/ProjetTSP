#include "P2_2opt.h"
#include "calc_distance.h"

bool verifyTriangleInequality(Point xi1, Point xi2, Point xj1, Point xj2, DistanceFun f){
    double dist1 = f(xi1, xi2) + f(xj1, xj2);
    double dist2 = f(xi1, xj1) + f(xi2, xj2);
    return dist1 > dist2;
}

int switchEdges(Tournee* tournee, int i, int j){
    // Verification des indices
    if ((i+1 >= tournee->size) || (j+1 >= tournee->size)) return -1;

    // Principe: echange de position des points aux indices i+1 et j dans la tournee;
    Point oldPt = tournee->ord_point_vis[i+1];
    tournee->ord_point_vis[i+1] = tournee->ord_point_vis[j];
    tournee->ord_point_vis[j] = oldPt;
    return 0;
}


// Algorithme recupere sur le wikipedia de la 2opt
// https://fr.wikipedia.org/wiki/2-opt
int tsp_2opt(Tournee* tournee, double* tourneeLen, DistanceFun f){
    bool amelioration = true;

    while (amelioration){
        amelioration = false;

        for (int i = 0; i < tournee->size; i++){
            for (int j = 0; j < tournee->size; j++){
                if (j != i && !((i+1 >= tournee->size) || (j+1 >= tournee->size))){ // Verification des indices
                    if (verifyTriangleInequality(tournee->ord_point_vis[i], tournee->ord_point_vis[i+1], tournee->ord_point_vis[j], tournee->ord_point_vis[j+1], f)){
                        switchEdges(tournee, i, j);
                        amelioration = true;
                    }
                }
            }
        }
    }
    *tourneeLen = calcul_poids_tournee(tournee, f);
    return 0;
}
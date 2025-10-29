#include "main.h"
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 
#include <signal.h>
#include "ctrl_c.h"
#include "P1_fonctions.h"
#include "P2_heuristiques.h"

/*
Comment utiliser:
    make -f p2tempMakefile
    make -f p2tempMakefile clean
    bin/tsp_p2temp chemin_de_fichier
*/

int main(int argc, char *argv[]) {
    printf("main de test p2\n");
    if (argc!=2) {
        fprintf(stderr,"tu troll mon gars donne un nom de fichier\n");
        return 1;
    }
    char * file_name = argv[1];
    //CREATION DE L'INSTANCE LUE
    Graphe* gr = read_TSPLIB(file_name);
    if (!gr){
        return 2;
    }
      
    // INIT CALCUL DE FONCTION DE CALCUL DE BASE 
    DistanceFun calc_dist = calc_dist_att;
    if (strcmp(gr->type_edge,"GEO")==0)
    {
        calc_dist = calc_dist_geo;
    }else if(strcmp(gr->type_edge,"EUC_2D")==0){ 
        calc_dist = calc_dist_eucl2d;
    }
    Tournee* best = createTourneCanonique(gr);
    double bestL;
    clock_t begin = clock();
    printf("bah debut\n");
    int result = tsp_nearestNeighbour(gr, calc_dist, best,&bestL) ;
    
    printf("bah fin2\n");
    clock_t end = clock();
    if (result!=0) {
        printf("bah merde\n");
    }
    double timeCPU = (double)(end-begin);
    char * bestString = toStringArray(*best);
    char * chread="lol";
    printf("bah fin\n");
    affichageResultat(gr,bestString,bestLen,timeCPU,chread);
    return 0;
}
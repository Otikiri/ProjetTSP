#include "main.h"
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 
#include <signal.h>
#include "ctrl_c.h"


typedef enum{ 
    CANONICAL,BRUTEFORCE,NEAREST_NEIGHBOUR,RANDOM_WALK,OPT_NN,OPT_RW,UNKNOWN
}Algo_type;


int test_entree(int argc, char *argv[]){
    if (argc <2)
    {
        affichage_erreur(); 
        return 1;
    }
    else if (argc ==2)
    {
        if (strcmp(argv[1],"-h")==0)
        {  
            affichage_help();
            return 1;
        }else{
            affichage_erreur();
            return 1; 
        }
    }else if (argc == 4 && strcmp(argv[3],"-c")!=0)
    {
        affichage_erreur();
        return 1;
    }else if (argc == 5 && 
        (strcmp(argv[3], "-m") != 0 
        &&( strcmp(argv[4], "bf") != 0
        || strcmp(argv[4], "nn") != 0
        || strcmp(argv[4], "rw") != 0 ))) {
        affichage_erreur();
        return 1;
    }
    return 0; 
}


Algo_type returnAlgoType(char * chRead){
    if (strcmp(chRead,"-c")==0) return CANONICAL;
    if (strcmp(chRead,"bf")==0) return BRUTEFORCE;
    if (strcmp(chRead,"nn")==0) return NEAREST_NEIGHBOUR; 
    if (strcmp(chRead,"rw")==0) return RANDOM_WALK;
    if (strcmp(chRead,"2optnn")==0) return OPT_NN;
    if (strcmp(chRead,"2optrw")==0) return OPT_RW;
    return UNKNOWN;
}

void canon_wrapper(Graphe * gr,char * chRead,DistanceFun calc_dist){
    Tournee * t = createTourneCanonique(gr);
    double resCanonicalTurn = calcul_poids_tournee(t,calc_dist);
    char * turnString = toStringArray(*t);
        
    affichageResultat(gr,turnString,resCanonicalTurn,0.0,chRead);
    free(turnString);
    detruireTournee(t);
    //exit(EXIT_SUCCESS);
}

void bf_wrapper(Graphe * gr,char * chRead,DistanceFun calc_dist){
        Tournee* best = createTourneCanonique(gr);
        Tournee* worst = createTourneCanonique(gr);
        double bestL;
        double worstL;

        // Allocation memoire de la chaine de caractere permActuelle et bestPermString affichee dans ctrl_c
        bestPermString = malloc(512);
        permActuelle = malloc(512);
        
        void (*oldINT)(int) = signal(SIGINT, INThandler);

        if (oldINT == SIG_ERR){
            perror("Erreur assignement signal SIGINT\n");
            exit(EXIT_FAILURE);
        }
        clock_t begin = clock();
        int returnBruteForce = tsp_bruteforce(gr,calc_dist,WITH_MATRICE,best,&bestL,worst,&worstL);
        clock_t end = clock();
        if (returnBruteForce == -1){
            free_graphe(gr);
            detruireTournee(best);
            detruireTournee(worst);
            free(permActuelle);
            free(bestPermString);
            exit(0);
        }
        signal(SIGINT, oldINT);

        free(permActuelle);
        free(bestPermString);

        double timeCPU = (double)(end-begin);
        char * bestString = toStringArray(*best);
        char * worstString = toStringArray(*worst);

        affichageResultat(gr,bestString,bestLen,timeCPU,chRead);
        //affichageResultat(gr,bestString,bestLen,timeCPU,chRead);

        free(bestString);
        free(worstString);
        detruireTournee(best);
        detruireTournee(worst);
}

void nn_wrapper(Graphe * gr,char * chRead,DistanceFun calc_dist){
    Tournee * best = createTourneCanonique(gr);
    double bestL;   
    clock_t begin = clock();
    tsp_nearestNeighbour(gr,calc_dist,best,&bestL);
    clock_t end = clock();
    char * bestString = toStringArray(*best);
    affichageResultat(gr,bestString,bestL,(double)(end-begin),chRead);
    free(bestString);
    detruireTournee(best);
    //exit(EXIT_SUCCESS);
}

void rw_wrapper(Graphe * gr,char * chRead,DistanceFun calc_dist){
    Tournee * best = createTourneCanonique(gr);
    double bestL;   
    clock_t begin = clock();
    tsp_randomWalk(gr,calc_dist,best,&bestL);
    clock_t end = clock();
    char * bestString = toStringArray(*best);
    affichageResultat(gr,bestString,bestL,(double)(end-begin),chRead);
    free(bestString);
    detruireTournee(best);
    //exit(EXIT_SUCCESS);
}

void optnn_wrapper(Graphe * gr,char * chRead,DistanceFun calc_dist){
    Tournee * best = createTourneCanonique(gr);
    double bestL;   
    tsp_nearestNeighbour(gr,calc_dist,best,&bestL);

    clock_t begin = clock();
    tsp_2opt(best,&bestL,calc_dist);
    clock_t end = clock();

    char * bestString = toStringArray(*best);
    affichageResultat(gr,bestString,bestL,(double)(end-begin),chRead);
    free(bestString);
    detruireTournee(best);
    //exit(EXIT_SUCCESS);
}

void optrw_wrapper(Graphe * gr,char * chRead,DistanceFun calc_dist){
    Tournee * best = createTourneCanonique(gr);
    double bestL;   
    tsp_randomWalk(gr,calc_dist,best,&bestL);

    clock_t begin = clock();
    tsp_2opt(best,&bestL,calc_dist);
    clock_t end = clock();
    
    char * bestString = toStringArray(*best);
    affichageResultat(gr,bestString,bestL,(double)(end-begin),chRead);
    free(bestString);
    detruireTournee(best);
    //exit(EXIT_SUCCESS);
}

int main(int argc,char *argv[]){
    
    if (test_entree(argc,argv)==1)
    {
        return 1;
    }
    
    char * file_name = argv[2];

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
    
    char * chRead;
    if(argc==4){ chRead = argv[3];}
    else{ chRead = argv[4];}

    //printf("chRead : %s\n",chRead);
    Algo_type t = returnAlgoType(chRead);

    switch (t)
    {
    case CANONICAL:
        canon_wrapper(gr,chRead,calc_dist);
        break;
    case BRUTEFORCE: 
        bf_wrapper(gr,chRead,calc_dist);
        break;
    case NEAREST_NEIGHBOUR:
        nn_wrapper(gr,chRead,calc_dist);
        break; 
    case RANDOM_WALK: 
        rw_wrapper(gr,chRead,calc_dist);
        break; 
    case OPT_NN: 
        optnn_wrapper(gr,chRead,calc_dist);
        break; 
    case OPT_RW:
        optrw_wrapper(gr,chRead,calc_dist); 
        break;
    case UNKNOWN: 
        exit(EXIT_FAILURE);
    }
    free_graphe(gr);
    
    
    return 0;
}
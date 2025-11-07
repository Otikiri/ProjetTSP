#include "main.h"
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> 
#include <signal.h>


typedef enum{
    CANONIQUE,BRUTEFORCE,NEAREST_NEIGHBOUR,RANDOM_WALK,OPT_NN,OPT_RW,INCONNU
}AlgoType;

/*                  FONCTIONS POUR MAIN                     */

/* FONCTION POUR VERIFIER QUE LES ENTREES UTILISATEURS SONT CORRECTES */
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
    }else if (argc == 5){
        char * modeRead = argv[3];
        char * algoRead = argv[4];

        bool modeInvalid = strcmp(modeRead,"-m")!=0; 
        bool algoInvalid = (strcmp(algoRead,"bf") != 0 && strcmp(algoRead,"nn") !=0
                        && strcmp(algoRead,"rw")!= 0 && strcmp(algoRead,"2optnn")!=0
                        && strcmp(algoRead,"2optrw")!=0); 

        if (modeInvalid || algoInvalid){
            affichage_erreur();
            return 1;
        }
    }
    return 0; 
}


AlgoType returnAlgoType(char * chRead){
    if (strcmp(chRead,"-c")==0) return CANONIQUE;
    if (strcmp(chRead,"bf")==0) return BRUTEFORCE;
    if (strcmp(chRead,"nn")==0) return NEAREST_NEIGHBOUR;
    if (strcmp(chRead,"rw")==0) return RANDOM_WALK;
    if (strcmp(chRead,"2optnn")==0) return OPT_NN;
    if (strcmp(chRead,"2optrw")==0) return OPT_RW;
    return INCONNU;
}

/* FONCTION WRAPPER POUR CANONIQUE */

void canonical(Graphe * g,DistanceFun calc_dist,char * chRead){
    Tournee * t = createTourneCanonique(g);

    double resCanonicalTurn = calcul_poids_tournee(t,calc_dist);
    char * turnString = toStringArray(*t);
        
    affichageResultat(g,turnString,resCanonicalTurn,0.0,chRead);
    detruireTournee(t);
    free(turnString);
}

/* FONCTION WRAPPER POUR BRUTE FORCE */

void bruteForce(Graphe *gr,DistanceFun calc_dist,char * chRead){
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
        exit(EXIT_SUCCESS);
    }
    signal(SIGINT, oldINT);

    free(permActuelle);
    free(bestPermString);

    double timeCPU = (double)(end-begin);
    char * bestString = toStringArray(*best);
    char * worstString = toStringArray(*worst);

    affichageResultat(gr,bestString,bestLen,timeCPU,chRead);

    free(bestString);
    free(worstString);
    detruireTournee(best);
    detruireTournee(worst);   
}

/* FONCTION WRAPPER POUR NEAREST NEIGHBOUR */

void nearestNeighbour(Graphe * gr,DistanceFun calc_dist,char * chRead){
    Tournee * t = createTourneCanonique(gr);
    double bestLen;

    clock_t begin = clock();
    tsp_nearestNeighbour(gr,calc_dist,t,&bestLen);
    clock_t end = clock();

    double timeCPU = (double)(end-begin);
    char * tString = toStringArray(*t);
    affichageResultat(gr,tString,bestLen,timeCPU,chRead);
    free(tString);
    detruireTournee(t);
}

/* FONCTION WRAPPER POUR RANDOM WALK */

void randomWalk(Graphe *gr,DistanceFun calc_dist,char * chRead){
    Tournee * t = createTourneCanonique(gr);
    double bestLen;

    clock_t begin = clock();
    tsp_randomWalk(gr,calc_dist,t,&bestLen);
    clock_t end = clock();

    double timeCPU = (double)(end-begin);
    char * tString = toStringArray(*t);
    affichageResultat(gr,tString,bestLen,timeCPU,chRead);
    free(tString);
    detruireTournee(t);
}

void optNN(Graphe *gr,DistanceFun calc_dist,char * chRead){
    Tournee * t = createTourneCanonique(gr);
    double bestLen;

    tsp_nearestNeighbour(gr,calc_dist,t,&bestLen);
    tsp_2opt(t,&bestLen,calc_dist);

    char * tString = toStringArray(*t);
    affichageResultat(gr,tString,bestLen,0.0,chRead);
    free(tString);
    detruireTournee(t);
}

void optRW(Graphe *gr,DistanceFun calc_dist,char * chRead){
    Tournee * t = createTourneCanonique(gr);
    double bestLen;

    tsp_randomWalk(gr,calc_dist,t,&bestLen);
    tsp_2opt(t,&bestLen,calc_dist);
    
    char * tString = toStringArray(*t);
    affichageResultat(gr,tString,bestLen,0.0,chRead);
    free(tString);
    detruireTournee(t);
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
    switch (returnAlgoType(chRead))
    {
    case CANONIQUE:
        canonical(gr,calc_dist,chRead);
        break;
    case BRUTEFORCE: 
        bruteForce(gr,calc_dist,chRead);
        break;
    case NEAREST_NEIGHBOUR: 
        nearestNeighbour(gr,calc_dist,chRead);
        break;
    case RANDOM_WALK: 
        randomWalk(gr,calc_dist,chRead);
        break;
    case OPT_NN: 
        optNN(gr,calc_dist,chRead);
        break;
    case OPT_RW:
        optRW(gr,calc_dist,chRead);
        break;
    case INCONNU: 
        free_graphe(gr);
        return EXIT_FAILURE;
    }
    
    if (gr!=NULL) free_graphe(gr);
    
    return 0;
}
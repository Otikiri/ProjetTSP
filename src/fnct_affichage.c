#include "fnct_affichage.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void affichage_help(){
    printf("Utilisation :\n");
    printf("> %s./bin/tsp -h %s: pour savoir comment utiliser le code\n",GREEN,NORMAL);
    printf("> %s./bin/tsp -f test/FICHIER -c %s: pour calculer la distance d'une tournee cannonique\n",GREEN,NORMAL);
    printf("> %s./bin/tsp -f test/FICHIER -m [bf] %s: pour calculer la distance d'une tournee avec des algorithmes differents\n",GREEN,NORMAL);
    printf("> Les fichiers dans test sont : %satt5.tsp , att10.tsp , att15.tsp , berlin5.tsp , berlin52.tsp , burma14.tsp et burma5.tsp%s\n",GREEN,NORMAL);
}

void affichage_erreur(){
    printf("Utilisation :\n");
    printf("> %s./bin/tsp -h %s: pour savoir comment utiliser le code\n",RED,NORMAL);
    printf("> %s./bin/tsp -f test/FICHIER -c %s: pour calculer la distance d'une tournee cannonique\n",RED,NORMAL);
    printf("> %s./bin/tsp -f test/FICHIER -m [bf] %s: pour calculer la distance d'une tournee avec des algorithmes differents\n",RED,NORMAL);
    printf("> Les fichiers dans test sont : %satt5.tsp , att10.tsp , att15.tsp , berlin5.tsp , berlin52.tsp , burma14.tsp et burma5.tsp%s\n",GREEN,NORMAL);
}

void affichageResultat(Graphe * g,char * tourneString,double resultat_calc,double temps,char * methode_calc){
    printf("Instance ; Méthode ; Temps CPU (sec) ; Longueur ; Tour\n");
    printf("%s ; %s ; %f ; %f ; %s\n", g->nom, methode_calc, temps/CLOCKS_PER_SEC, resultat_calc, tourneString); 
}

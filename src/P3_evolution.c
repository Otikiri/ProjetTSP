#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "calc_distance.h"


#include <string.h>
#include "P3_evolution.h"
#include <float.h>
#include "P1_fonctions.h"
#include "P2_heuristiques.h"
#include "P2_2opt.h"
#include "lectureTSPLIB.h"
/* --- UTILS --- */
/**
 * @brief calcule la longueur de l'individu passe en parametre
 * @param indiv l'individu qu'on veut calculer sa longueur
 * @param n longueur de l'individu , nb de points qu'il y a dans le graphe
 * @param dist fonction de distance utilise pour le calcule de la longueur du traject
 * @param graphe graphe du fichier
 * @return double qui correspond a la longueur de l'individu
 */
static double fitness(int* indiv, int n, DistanceFun dist, const Graphe* graphe) {
    double sum = 0.0;
    for(int i = 0; i < n - 1; i++)
        sum += dist(graphe->point[indiv[i]], graphe->point[indiv[i+1]]);
    sum += dist(graphe->point[indiv[n-1]], graphe->point[indiv[0]]);
    return sum;
}

/**
 * @brief melange un individue de maniere aleatoire
 * @param arr l'individue qu'on veut melanger
 * @param n taille de l'individue
 * @return void
 */
static void shuffle(int* arr, int n) {
    for(int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
    }
}

/* Mutation: swap mutation */
/**
 * @brief possibilite d'echanger deux points du tableau 
 * @param indiv l'individu qu'on veut echanger las points
 * @param n taille de l'individue
 * @param rate la possibilite d'echanger 1 point du tableau avec un autre
 * @return void 
 */
static void swapMutation(int* indiv, int n, double rate)
{
    for(int i = 0; i < n; i++) {
        double r = (double)rand() / (double)RAND_MAX;
        if(r < rate) {
            int j = rand() % n;
            int tmp = indiv[i];
            indiv[i] = indiv[j];
            indiv[j] = tmp;
        }
    }
}


/* Tournoi */
/**
 * @brief Le tournoi pour selectiones les individues avec une distance minimale
 * @param pop population participant au tournoi
 * @param fit le fitness de chaque individue de la population
 * @param sel la population selectione
 * @param pop_size la taille de la population
 * @param n la taille d'un individue
 * @param tour_size la taille du tournoi
 * @return void
 */
static void tournamentSelection(
    int** pop, double* fit, int** sel,
    int pop_size, int n, int tour_size
) {
    for(int i = 0; i < pop_size; i++) {
        int best = rand() % pop_size;
        for(int k = 1; k < tour_size; k++) {
            int c = rand() % pop_size;
            if(fit[c] < fit[best]) best = c;
        }
        memcpy(sel[i], pop[best], n * sizeof(int));
    }
}


/* 
 * ordered_crossover
 * parent1, parent2 : tableaux de taille n (permutations de villes)
 * child : tableau de sortie (taille n)
 */
void orderedCrossover(int* parent1, int* parent2, int* child, int n,DistanceFun dist, const Graphe* graphe)
{
    int start = rand() % n;
    int end   = rand() % n;

    if(start > end) {
        int tmp = start;
        start = end;
        end = tmp;
    }

    /* Initialise child avec -1 = vide */
    for(int i = 0; i < n; i++)
        child[i] = -1;

    /* Copie du segment de parent1 */
    for(int i = start; i <= end; i++)
        child[i] = parent1[i];

    /* Remplir avec parent2 dans l’ordre */
    int insert_pos = 0;
    for(int i = 0; i < n; i++) {
        int gene = parent2[i];

        /* vérifier si gene est déjà dans child */
        int present = 0;
        for(int k = start; k <= end; k++) {
            if(child[k] == gene) {
                present = 1;
                break;
            }
        }
        if(present) continue;

        /* trouver le premier emplacement libre (-1) */
        while(child[insert_pos] != -1)
            insert_pos++;

        child[insert_pos] = gene;
    }
}









/**
 * @brief calcule un tableau qui contient les connexions deja coupes de la forme suivante:
 *  next[a]=b montre la connexion (a,b)
 * @param p1 un des parents pour faire le croissement
 * @param p2 un des parentrs pour faire le croissement
 * @param n taille des individues
 * @param next tableau ou on stocke les connexions coupes
 * @return void
 */
static void build_initial_fragments(const int* p1, const int* p2, int n, int* next) {

    int* pos = malloc(n * sizeof(int));
    if(!pos) exit(1);

    for(int i = 0; i < n; i++)
        pos[p2[i]] = i;

    // next[i] = successeur dans P1
    for(int i = 0; i < n; i++) {
        int a = p1[i];
        int b = p1[(i+1) % n];
        next[a] = b;
    }

    // Couper les arêtes non présentes dans P2
    for(int i = 0; i < n; i++) {
        int a = p1[i];
        int b = p1[(i+1) % n];
        if(p2[(pos[a] + 1) % n] != b)
            next[a] = -1;
    }

    free(pos);
}



/**
 * @brief Trouve les 2 extremites les plus proche a partire des connexions donnes
 * @param next tableau de connexions
 * @param n taille des individues (meme taille pour le nb de connexions)
 * @param dist la fonction de distance utilisee
 * @param graphe graphe d'un fichier tsp
 * @param best_i une des extremites trouves
 * @param best_j une des extremites trouves
 * @return void 
 */
static void find_closest_ends(const int* next, int n, DistanceFun dist, const Graphe* graphe,
                              int* best_i, int* best_j) {

    int* ends = malloc(n * sizeof(int));
    if(!ends) exit(1);

    int count = 0;
    for(int i = 0; i < n; i++)
        if(next[i] == -1)
            ends[count++] = i;

    *best_i = -1;
    *best_j = -1;

    // Moins de deux extrémités -> un seul fragment -> fin
    if(count <= 1) {
        free(ends);
        return;
    }

    double best = DBL_MAX;

    for(int a = 0; a < count; a++) {
        for(int b = a+1; b < count; b++) {
            double d = dist(graphe->point[ends[a]], graphe->point[ends[b]]);
            if(d < best) {
                best = d;
                *best_i = ends[a];
                *best_j = ends[b];
            }
        }
    }

    free(ends);
}



/**
 * @brief Reconnecte les deux extremites les plus proches
 * @param next tableau de connexion
 * @param n taille des individues (meme taille pour le nb de connexions)
 * @param dist fonction de distance utilise
 * @param graphe graphe d'un fichier tsp
 * @return void
 */
static void reconnect_fragments(int* next, int n, DistanceFun dist,const Graphe* graphe) {

    for(int step = 0; step < n; step++) {

        // 1) Compter les extrémités
        int ends_count = 0;
        for(int i = 0; i < n; i++)
            if(next[i] == -1)
                ends_count++;

        // 2) S'il reste ≤ 1 extrémité → un seul fragment → terminé
        if(ends_count <= 1)
            return;

        // 3) Trouver la meilleure pair d'extrémités
        int i, j;
        find_closest_ends(next, n, dist, graphe, &i, &j);

        // 4) Si aucune connexion possible → fini
        if(i == -1 || j == -1)
            return;

        // 5) Connexion
        next[i] = j;
    }
}




/**
 * @brief Reconstruction de l'enfant a partire d'un tableau de connexion
 * @param child enfant qu'on donne apres la reconstruction
 * @param next tableau de connexion
 * @param n taille des individues (meme taille pour le nb de connexions)
 * @return void
 */
static void rebuild_child_from_next(int* child, const int* next, int n) {

    // Trouver un sommet qui a un successeur valide
    int start = -1;
    for(int i = 0; i < n; i++) {
        if(next[i] != -1) { start = i; break; }
    }

    // Cas pathologique (ne devrait jamais arriver mais sécurité)
    if(start == -1) {
        for(int i = 0; i < n; i++)
            child[i] = i;
        return;
    }

    int cur = start;
    for(int i = 0; i < n; i++) {
        child[i] = cur;
        cur = next[cur];
        if(cur == -1) break;
    }
}




/**
 * @brief Realise le croissemente DPX de deux individues
 * @param p1 un des parent pour le coisement
 * @param p2 un des parent pour le coisement
 * @param child resultat du croissement DPX
 * @param n taille des individues (meme taille pour le nb de connexions)
 * @param dist fonction de distance utilise
 * @param graphe graphe d'un fichier tsp
 * @return int 0 si tout se passe bien -1 en cas d'erreur
 */
void dpx_crossover(const int* p1, const int* p2, int* child, int n, DistanceFun dist, const Graphe* graphe) {

    // 1. Initialiser l'enfant = copie de parent1
    memcpy(child, p1, n * sizeof(int));

    // 2. Construire fragments
    int* next = malloc(n * sizeof(int));
    if(!next) exit(1);

    build_initial_fragments(p1, p2, n, next);

    // 3. Reconnecter par plus proche extrémité
    reconnect_fragments(next, n, dist, graphe);

    // 4. Reconstruire tournée complète
    rebuild_child_from_next(child, next, n);

    free(next);
}





/* --- TSP EVOLUTION --- */
/**
 * @brief Algorithe d'evolution qui donne la meilleur trounee trouve
 * @param population_size taille de la population genere
 * @param generations nb de fois l'algo evolutif est utilise
 * @param mutation_rate probabilite qu'une mutation apparait dans un individue
 * @param trournament_size taille du tournoi effectue
 * @param distance fonction de distance utilise
 * @param graphe graphe d'un fichier tsp
 * @param crossover le croissement effectue par l'algorithme
 * 
 * @param outBest Pointeur sur la tournee ou l'on veut stocker la meilleur tournee
 * @param outBestLen Pointeur sure le double ou l'on veut stocker la longueur de la meilleur tournee
 * @return int 0 si tout se passe bien -1 en cas d'erreur
 */
int tsp_evolution(
    int population_size,
    int generations,
    double mutation_rate,
    int tournament_size,
    DistanceFun distance,
    Graphe* graphe,
    crossoverFun crossover,

    Tournee* outBest, 
    double* outBestLen
) {
    if (!graphe || graphe->dimension <= 0 || !outBest || !outBestLen) return -1;
    // Initialisation du randomizer en utilisant le moment actuel en tant que seed
    srand(time(NULL));

    const int n = graphe->dimension;

    /* Allocate population */
    int** population = malloc(population_size * sizeof(int*));
    int** offspring   = malloc(population_size * sizeof(int*));
    int** selected    = malloc(population_size * sizeof(int*));

    Tournee * tInit = createTourneCanonique(graphe);
    double buffer;
    tsp_randomWalk(graphe, distance,tInit,&buffer);
    int cities[n];
    for (int i = 0; i < n; i++)
        cities[i]=tInit->ord_point_vis[i].id;
    
    for(int i = 0; i < population_size; i++) {
        population[i] = malloc(n*sizeof(int));
        offspring[i]  = malloc(n*sizeof(int));
        selected[i]   = malloc(n*sizeof(int));

        memcpy(population[i], cities, n*sizeof(int));
        shuffle(population[i], n);
    }


    double* fit = malloc(population_size * sizeof(double));
    int* bestIndiv = malloc(n*sizeof(int));
    memcpy(bestIndiv, population[0], n*sizeof(int));

    for(int g = 0; g < generations; g++) {

        for(int i = 0; i < population_size; i++)
            fit[i] = fitness(population[i], n, distance, graphe);

        tournamentSelection(population, fit, selected,
                             population_size, n, tournament_size);

        for(int i = 0; i < population_size; i+=2) {
            crossover(selected[i], selected[i+1], offspring[i],   n, distance, graphe);
            crossover(selected[i+1], selected[i], offspring[i+1], n, distance, graphe);
        }

        for(int i = 0; i < population_size; i++)
            swapMutation(offspring[i], n, mutation_rate);

        for(int i = 0; i < population_size; i++)
            fit[i] = fitness(offspring[i], n, distance, graphe);

        /* find worst */
        int worst = 0;
        for(int i = 1; i < population_size; i++)
            if(fit[i] > fit[worst]) worst = i;

        shuffle(offspring[worst], n);

        /* elitism */
        int best_gen = 0;
        for(int i = 1; i < population_size; i++)
            if(fit[i] < fit[best_gen]) best_gen = i;

        if(fitness(offspring[best_gen], n, distance, graphe)
            < fitness(bestIndiv, n, distance, graphe)) {
            memcpy(bestIndiv, offspring[best_gen], n*sizeof(int));
        }
        memcpy(offspring[worst], bestIndiv, n*sizeof(int));

        /* replace population */
        for(int i = 0; i < population_size; i++)
            memcpy(population[i], offspring[i], n*sizeof(int));
    }

    /* return best */

    fill_tournee_from_perm(graphe,bestIndiv,n,outBest);
    *outBestLen = fitness(bestIndiv,n,distance,graphe);

    free(bestIndiv);
    free(fit);
    for(int i = 0; i < population_size; i++) {
        free(population[i]);
        free(offspring[i]);
        free(selected[i]);
    }
    free(population);
    free(offspring);
    free(selected);
    return 0; 
}


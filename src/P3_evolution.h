#ifndef TSP_EVOLUTION_H
#define TSP_EVOLUTION_H


typedef void (*crossoverFun)(int* parent1, int* parent2, int* child, int n, distanceFun dist, const Graphe* graphe);

/**
 * @brief Realise le croissemente trie de deux individues
 * @param parent1 le premier individue parent pour faire le croisemente
 * @param parent2 le deuxieme individue parent pour faire le croisement
 * @param child pointeur qui donnera le resulta du croissement
 * @param n taille des individues
 * @param dist fonction de distance qui ne sera pas utilise (ici pour accomplire les requis du type pour la fonction de croissement)
 * @param graphe graphe d'un fichier tsp pas utilise
 * @return int 0 si tout se passe bien -1 en cas d'erreur
 */
void orderedCrossover(int* parent1, int* parent2, int* child, int n,DistanceFun dist, const Graphe* graphe);


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
void dpx_crossover(const int* p1, const int* p2, int* child, int n, DistanceFun dist, const Graphe* graphe);



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
    const Graphe* graphe,
    crossoverFun crossover,

    Tournee* outBest, 
    double* outBestLen
);

#endif

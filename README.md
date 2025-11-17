# Projet du TSP de l'UE Projet-Avance 

## Equipe n'13 : 

- DIDIER Yoann
- NGUYEN Bastien
- PUENTE GONZALEZ Oscar
- SAY Virakyuth

## Information generale 

Ce projet se base sur la problematique du "Travelling Saleman Probleme" ou TSP. C'est un projet pedagogique pour l'UE Projet Avance de l'Universite de Toulouse (formellement Universite Toulouse III , Paul SABATIER). Il contient plusieurs algorithme de calcul qui sont explicites dans les differentes parties.

## Utilisation 

- *** make *** : dans le fichier de base (../ProjetTSP), va servir a creer l'exexutable du main.c
- *** make clean *** : dans le fichier de base (../ProjectTSP), va servir a nettoyer les fichiers creer par le make
- *** ./bin/tsp -h *** : pour montrer comment utiliser ces algorithemes.
Les fichiers de tests sont dans le repertoires tests pour l'instant ceux qui sont disponibltes sont : att5 , att10 , att15 , berlin5 , berlin52 , burma14 et burma5.

## Partie 0 

La partie 0 concerne l'implementation des differents fonctions necessaires pour manipuler les fichiers de test et des fonctions de calculs de distance. 
- *** ./bin/tsp -f <u> FICHIER </u> -c *** : pour faire executer calculer la tournee canonique.

## Partie 1

La partie 1 est l'implementation d'un algorithme de brute force pour resoudre le probleme.
- *** ./bin/tsp -f <u> FICHIER </u> -m [bf] *** : pour faire exectuer l'algorithme de bruteforce.

## Partie 2 

La partie 2 concerne l'implementation des algorithmes Nearest Neighbour , Random Walk et 2opt. 
- ***./bin/tsp -f <u> FICHIER </u> -m [nn]*** : pour faire exectuer l'algorithme du Nearest Neighbour.
- ***./bin/tsp -f <u> FICHIER </u> -m [rw]*** : pour faire exectuer l'algorithme du Random Walk.
- ***./bin/tsp -f <u> FICHIER </u> -m [2optnn]*** : pour faire exectuer l'algorithme du 2opt avec Nearest Neighbour.
- ***./bin/tsp -f <u> FICHIER </u> -m [2optrw]*** : pour faire exectuer l'algorithme du 2opt avec Random Walk.

## Partie 3 
- ***./bin/tsp -f <u> FICHIER </u> -m [ga,gadpx] <nombre d'individu> <nombre de generation> <taux de mutation>*** : 

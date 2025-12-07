# Projet du TSP de l'UE Projet-Avance 

## Equipe n'13 : 

- DIDIER Yoann
- NGUYEN Bastien
- PUENTE GONZALEZ Oscar
- SAY Virakyuth

## Organisation de l'équipe 

- <u> Chef de projet </u> : SAY Virakyuth
- <u> Codeur(s) </u> : NGUYEN Bastien , PUENTE GONZALEZ Oscar ,DIDIER Yoann
- <u> Testeur(s) </u> : SAY Virakyuth

## Information generale 

Ce projet se base sur la problematique du "Travelling Saleman Probleme" ou TSP. C'est un projet pedagogique pour l'UE Projet Avance de l'Universite de Toulouse (formellement Universite Toulouse III , Paul SABATIER).Il a pour but d'explorer les différents algorithmes que l'on peut utiliser pour résoudre cette problématique, mais aussi de servir comme une première expérience de travailler en groupe sur un projet informatique sur une période de temps défini.

## Utilisation 

### Instructions de base: 
- **make** : dans le fichier de base (../ProjetTSP), va servir a creer l'exexutable du main.c
- **make clean** : dans le fichier de base (../ProjectTSP), va servir a nettoyer les fichiers creer par le make
- **./bin/tsp -h** : pour montrer comment utiliser ces algorithemes.
Les fichiers de tests sont dans le repertoires tests pour l'instant ceux qui sont disponibltes sont : att5 , att10 , att15 , berlin5 , berlin52 , burma14 et burma5.

### Instructions pour les tests Python: 
Les vérifications des résultats sont effectuées en faisant une comparaison des résultats de notre code avec le code de la librairie "tsplib95" de python. Le code fonctionne sur les machines linux et windows avec WSL2.0. 

Pour lancer le code python, il faut avoir python d'installer sur la machine et ensuite lancer le script shell "../python/setup_env.sh". 

Dans le répertoire  "../ProjectTSP/python" lancer : 
- **source venv/bin/activate**: pour activer le sous environnement python. 
- **python3 test_tsp_c.py**
pour tester notre code.

### Instructions pour le code C: 
Prérequis : avoir lancé la commande make.
#### Partie 0 
La partie 0 concerne l'implementation des differents fonctions necessaires pour manipuler les fichiers de test et des fonctions de calculs de distance. 
- **./bin/tsp -f <u> FICHIER </u> -c** : pour faire executer calculer la tournee canonique.

#### Partie 1

La partie 1 est l'implementation d'un algorithme de brute force pour resoudre le probleme.
- **./bin/tsp -f <u> FICHIER </u> -m [bf]** : pour faire exectuer l'algorithme de bruteforce.

#### Partie 2 

La partie 2 concerne l'implementation des algorithmes Nearest Neighbour , Random Walk et 2opt. 
- **./bin/tsp -f <u> FICHIER </u> -m [nn]** : pour faire exectuer l'algorithme du Nearest Neighbour.
- **./bin/tsp -f <u> FICHIER </u> -m [rw]** : pour faire exectuer l'algorithme du Random Walk.
- **./bin/tsp -f <u> FICHIER </u> -m [2optnn]** : pour faire exectuer l'algorithme du 2opt avec Nearest Neighbour.
- **./bin/tsp -f <u> FICHIER </u> -m [2optrw]** : pour faire exectuer l'algorithme du 2opt avec Random Walk.

#### Partie 3 

La partie 3 concerne l'algorithme génétique évolutif et une version modifie de celle-ci ou on fait des croisements.
- **./bin/tsp -f <u> FICHIER </u> -m [ga,gadpx] <nombre d'individu> <nombre de generation> <taux de mutation>**: pour faire executer les algorithmes de cette partie.

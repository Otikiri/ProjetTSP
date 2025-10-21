#include "calc_distance.h"
#include <math.h>
#include <stdio.h> 
#include <stdlib.h> 

double calcul_poids_tournee(Tournee* tournee, double (*calc_dist)(Point, Point)){
    double distance_totale = 0;
    
    for (int i = 0; i < (tournee->size)-1 ; i++){
        distance_totale += (*calc_dist)(tournee->ord_point_vis[i], tournee->ord_point_vis[i+1]);
    }
    distance_totale+=(*calc_dist)(tournee->ord_point_vis[tournee->size-1],tournee->ord_point_vis[0]);
    return distance_totale;
}

double calc_dist_eucl2d(Point a, Point b){
    double xd = a.x - b.x;
    double yd = a.y - b.y;
    return sqrt(xd*xd + yd*yd);;
}

double calc_dist_geo(Point a, Point b){
    // Calcul latitude et longitude du point A
    double deg = floor(a.x);
    double min = a.x - deg;
    double latitudeA = PI * (deg + (5.0 * min / 3.0)) / 180.0;
    deg = floor(a.y);
    min = a.y - deg;
    double longitudeA = PI * (deg + (5.0 * min / 3.0)) / 180.0;

    // Calcul latitude et longitude du point B
    deg = floor(b.x);
    min = b.x - deg;
    double latitudeB = PI * (deg + (5.0 * min / 3.0)) / 180.0;
    deg = floor(b.y);
    min = b.y - deg;
    double longitudeB = PI * (deg + (5.0 * min / 3.0)) / 180.0;

    // Calcul de la distance geographique entre le point A et B
    double q1 = cos(longitudeA - longitudeB);
    double q2 = cos(latitudeA - latitudeB);
    double q3 = cos(latitudeA + latitudeB);
    double distGeo = RT * acos(0.5 * ((1.0+q1)*q2 - (1.0-q1)*q3)) + 1.0;

    return (int)distGeo;
}

double calc_dist_att(Point a, Point b){
    // Distance euclidienne alternative decrite dans la doc tsp95
    double xd = a.x - b.x;
    double yd = a.y - b.y;
    double rij = sqrt( (xd*xd + yd*yd) / 10.0);
    return nearbyint(rij+0.5);

}



#ifndef UTILITAIRE_H
#define UTILITAIRE_H

typedef struct Vecteur VECTEUR;
struct Vecteur 
{
	double x, y;
};

typedef struct Segment SEGMENT;
struct Segment 
{
	VECTEUR deb, fin;
};

double calculate_distance(VECTEUR vect1, VECTEUR vect2);
double calculate_norme_vecteur(VECTEUR v);
double calculate_norme_segment(SEGMENT s);
VECTEUR vecteur_difference(VECTEUR vect1, VECTEUR vect2);
VECTEUR calculer_difference_unitaire(SEGMENT s);
VECTEUR calculer_vecteur_unitaire(VECTEUR v);
VECTEUR calculer_difference_normal(SEGMENT s);
VECTEUR calculer_sens_inverse(VECTEUR v);
VECTEUR vecteur_multiplication_scalaire(VECTEUR vect, double scalaire);
double produit_vectoriel(VECTEUR v1, VECTEUR v2);
double produit_scalaire(VECTEUR v1, VECTEUR v2);
int detection_parallelisme(SEGMENT s1, SEGMENT s2);
int detection_intersection(SEGMENT s1, SEGMENT s2);
VECTEUR trajectoire_reflechie(VECTEUR position, double alpha);
VECTEUR new_vecteur(double x, double y);

#endif

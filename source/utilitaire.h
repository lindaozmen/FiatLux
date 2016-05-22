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

double utilitaire_calculate_distance(VECTEUR vect1, VECTEUR vect2);
double utilitaire_calculate_norme_vecteur(VECTEUR v);
double utilitaire_calculate_norme_segment(SEGMENT s);
VECTEUR utilitaire_vecteur_difference(VECTEUR vect1, VECTEUR vect2);
VECTEUR utilitaire_calculer_difference_unitaire(SEGMENT s);
VECTEUR utilitaire_calculer_vecteur_unitaire(VECTEUR v);
VECTEUR utilitaire_calculer_difference_normal(SEGMENT s);
VECTEUR utilitaire_calculer_sens_inverse(VECTEUR v);
VECTEUR utilitaire_vecteur_multiplication_scalaire(VECTEUR vect, double scalaire);
int utilitaire_is_segment_null(SEGMENT s);
double utilitaire_produit_vectoriel(VECTEUR v1, VECTEUR v2);
double utilitaire_produit_scalaire(VECTEUR v1, VECTEUR v2);
int utilitaire_detection_parallelisme(SEGMENT s1, SEGMENT s2);
int utilitaire_detection_intersection(SEGMENT s1, SEGMENT s2);
VECTEUR utilitaire_detection_point_intersection(SEGMENT s1, SEGMENT s2);
VECTEUR utilitaire_new_vecteur(double x, double y);

#endif

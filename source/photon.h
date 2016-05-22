#ifndef PHOTON_H
#define PHOTON_H
#include "constantes.h"
#include "error.h"
#include "utilitaire.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct Photon PHOTON;
typedef struct Photon_liste PHOTON_LISTE;
int verifier_fin_photon();
int photon_load(char * tab);
void photon_free();
int photon_get_nb();
void photon_update();
int photon_retirer(int id);
void photon_destruction_dehors(double xmin, double xmax,double ymin, double ymax);
void photon_trajectoire_reflechie(PHOTON* ph, SEGMENT s_reflecteur);
double photon_distance_intersection(SEGMENT s, VECTEUR pos, double angle);
int photon_add_ph(PHOTON ph);
void photon_add(VECTEUR position, double angle);
void photon_print_file(FILE* file);
void photon_drawing();

#endif


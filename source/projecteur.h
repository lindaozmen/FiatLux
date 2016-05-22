#ifndef PROJECTEUR_H
#define PROJECTEUR_H

#include "constantes.h"
#include "error.h"
#include "utilitaire.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Projecteur PROJECTEUR;

int verifier_fin_projecteur();
int projecteur_load(char * tab);
void projecteur_free();
void projecteur_retirer(int id);
int projecteur_get_nombre();
int projecteur_plus_proche_selection(VECTEUR vect_coordonne);
void projecteur_print_file(FILE* file);
int projecteur_add_pr(PROJECTEUR pr);
void projecteur_creation(VECTEUR deb, VECTEUR fin);
void projecteur_creer_photon();
SEGMENT projecteur_get_segment(int id);
void projecteur_draw_obj(int id);
void projecteur_drawing();
void projecteur_draw_selection(int id);
#endif

#ifndef ABSORBEUR_H
#define ABSORBEUR_H
#include "constantes.h"
#include "error.h"
#include "utilitaire.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Absorbeur ABSORBEUR;

int verifier_fin_absorbeur();
int absorbeur_load(char * tab);
void absorbeur_free();
int absorbeur_dectection_collision(SEGMENT s);
int absorbeur_plus_proche_selection(VECTEUR vect_coordonne);
int absorbeur_add_a(ABSORBEUR a);
void absorbeur_creation(VECTEUR* v, int nb_point);
void absorbeur_ramplacer_dernier(VECTEUR* v, int nb_point);
int absorbeur_nombre_vecteur(int id);
void absorbeur_retirer(int id);
int absorbeur_get_nombre();
void absorbeur_print_file(FILE* file);
void absorbeur_draw_obj(int id, int id_pt);
void absorbeur_drawing();
VECTEUR absorbeur_get_vecteur(int id, int nb);

#endif


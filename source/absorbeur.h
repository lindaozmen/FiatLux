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
void absorbeur_creation(ABSORBEUR* a, int nb_point);
int absorbeur_nombre_vecteur(int id);
void absorbeur_retirer(int id);
int absorbeur_get_nombre();
SEGMENT* absorbeur_get_segment(int id);
void absorbeur_print_file(FILE* file);
void absorbeur_draw_obj(int id, int id_pt);
void absorbeur_drawing();
int absorbeur_get_x(int id, int nb);
int absorbeur_get_y(int id,int nb);

#endif


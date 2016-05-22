#ifndef REFLECTEUR_H
#define REFLECTEUR_H
#include "constantes.h"
#include "error.h"
#include "utilitaire.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Reflecteur REFLECTEUR;

int verifier_fin_reflecteur();
int reflecteur_load(char * tab);
void reflecteur_free();
void reflecteur_retirer(int id);
int reflecteur_get_nombre();
SEGMENT reflecteur_dectection_collision(SEGMENT s);
int reflecteur_plus_proche_selection(VECTEUR vect_coordonne);
void reflecteur_print_file(FILE* file);
int reflecteur_add_r(REFLECTEUR r);
void reflecteur_creation(VECTEUR deb, VECTEUR fin);
SEGMENT reflecteur_get_segment(int id);
void reflecteur_draw_obj(int id_r);
void reflecteur_drawing();
void reflecteur_draw_selection(int id);
#endif


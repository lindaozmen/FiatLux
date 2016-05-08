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
int reflecteur_get_nombre();
void projecteur_print_file(FILE* file);
int reflecteur_add_r(REFLECTEUR r);
SEGMENT reflecteur_get_segment(int id);
void reflecteur_draw_obj(int id_r);
void reflecteur_drawing();
int reflecteur_get_deb_x(int id);
int reflecteur_get_deb_y(int id);
int reflecteur_get_fin_x(int id);
int reflecteur_get_fin_y(int id);


#endif


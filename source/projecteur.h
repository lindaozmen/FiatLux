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
int projecteur_get_nombre();
void projecteur_print_file(FILE* file);
int projecteur_add_pr(PROJECTEUR pr);
SEGMENT projecteur_get_segment(int id);
void projecteur_draw_obj(int id);
void projecteur_drawing();
int projecteur_get_deb_x(int id);
int projecteur_get_deb_y(int id);
int projecteur_get_fin_x(int id);
int projecteur_get_fin_y(int id);

#endif

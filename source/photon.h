#ifndef PHOTON_H
#define PHOTON_H
#include "constantes.h"
#include "error.h"
#include "utilitaire.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Photon PHOTON;
typedef struct Photon_liste PHOTON_LISTE;
int verifier_fin_photon();
int photon_load(char * tab);
void photon_free();
int photon_get_nb();
void photon_update();
int photon_retirer(int id);
int photon_add_ph(PHOTON ph);
void photon_print_file(FILE* file);
void photon_drawing();

#endif

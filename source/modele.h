#ifndef MODELE_H
#define MODELE_H
#include "constantes.h"
#include "error.h"
#include "utilitaire.h"
#include "reflecteur.h"
#include "projecteur.h"
#include "absorbeur.h"
#include "photon.h"
#include <stdio.h>
#include <string.h>

int modele_lecture(char * nomfichier, int mode_verification);
void modele_lecture_rendu2();
void destruction_simulation();
void modele_save_elements(char* nom_fichier);

#endif

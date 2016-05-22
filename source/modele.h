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
void modele_element_plus_proche(VECTEUR vect_coordonne);
void modele_effacer_element();
void modele_creation(VECTEUR v);
void modele_draw();
void modele_draw_all();
void modele_dessin_carre_zoom(VECTEUR dessin_start, VECTEUR dessin_end);
void modele_destruction_simulation();
void modele_destruction_photon_dehors(double xmin, double xmax,double ymin, double ymax);
void modele_save_elements(char* nom_fichier);
void modele_check_radio(int mode, int choix);
int modele_verifier_collision();

#endif

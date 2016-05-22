#include "photon.h"
#include "projecteur.h"
#include "reflecteur.h"
#include "absorbeur.h"
#include "graphic.h"
#include "utilitaire.h"

static int nb_element_ph = 0;
static int nb_expected_ph = 0;
static int index_count = 0;

struct Photon
{
	VECTEUR position;
	double alpha;
};

struct Photon_liste
{
	int id;
	PHOTON ph;
	PHOTON_LISTE* suivant;
};

static PHOTON_LISTE* tete;
static int started = 0;
int verifier_fin_photon()
{
	if((nb_element_ph) < nb_expected_ph)
	{
		error_lecture_elements(ERR_PHOTON, ERR_PAS_ASSEZ);
		return 1;
	}
	else if((nb_element_ph) > nb_expected_ph)
	{
		error_lecture_elements(ERR_PHOTON, ERR_TROP);
		return 1;
	}
	return 0;
}

int photon_load(char * tab)
{
	char *deb = tab; 
	PHOTON ph;
	
	if(tete == NULL && started == 0)
	{
		if(sscanf(deb, "%d", &nb_expected_ph) != 1)
		{
			error_lect_nb_elements(ERR_PHOTON);
			return 1;
		}
		
		if(nb_expected_ph < 0)
		{
			error_lect_nb_elements(ERR_PHOTON);
			return 1;
		}
		
		started = 1;
	}
	else
	{
		if(sscanf(deb,"%lf %lf %lf", &ph.position.x, &ph.position.y, &ph.alpha) != 3)
		{
			error_lecture_elements(ERR_PHOTON, ERR_PAS_ASSEZ);
			return 1;
		}

		photon_add_ph(ph);
	}
	return 0;	
}
	
void photon_free()
{
	while(tete){
		photon_retirer(tete->id);
	}
	started = 0;
	nb_element_ph = 0;
	nb_expected_ph = 0;
}


//method pour accéder aux éléments
int photon_get_nb()	
{
	return nb_element_ph;
}


void photon_update()
{
	PHOTON_LISTE* up_tete = tete;
	while(up_tete)
	{
		SEGMENT s;
		s.deb = (up_tete->ph).position;
		(up_tete->ph).position.x += VPHOT*DELTA_T*cos((up_tete->ph).alpha);
		(up_tete->ph).position.y += VPHOT*DELTA_T*sin((up_tete->ph).alpha);
		s.fin = (up_tete->ph).position;
		PHOTON_LISTE* suivant = up_tete->suivant;
		if(absorbeur_dectection_collision(s))
		{
			photon_retirer(up_tete->id);
			up_tete = NULL;
		}
		else
		{
			SEGMENT s_reflecteur;
			s_reflecteur = reflecteur_dectection_collision(s);
		
			if(is_segment_null(s_reflecteur))
			{
				(up_tete->ph).position.x -= VPHOT*DELTA_T*cos((up_tete->ph).alpha);
				(up_tete->ph).position.y -= VPHOT*DELTA_T*sin((up_tete->ph).alpha);
				
				VECTEUR point_intersection;
				point_intersection = detection_point_intersection(s, s_reflecteur);
				double distance = photon_distance_intersection(s_reflecteur, s.deb, (up_tete->ph).alpha);
				(up_tete->ph).position.x += (distance*cos((up_tete->ph).alpha));
				(up_tete->ph).position.y += (distance*sin((up_tete->ph).alpha));
				
				photon_trajectoire_reflechie(&(up_tete->ph), s_reflecteur);
			}
		}
		up_tete = suivant;	
	}
	projecteur_creer_photon();
}

void photon_trajectoire_reflechie(PHOTON* ph, SEGMENT s_reflecteur)
{
	VECTEUR t = {cos(ph->alpha), sin(ph->alpha)};	//car sa longueur = 1
	VECTEUR n = calculer_difference_unitaire(s_reflecteur);
	if(produit_scalaire(t, n) > 0)
		n = calculer_sens_inverse(n);
	VECTEUR intermediaire = vecteur_multiplication_scalaire(n, 2*produit_scalaire(t, n));
	VECTEUR w = vecteur_difference(t, intermediaire);
	double angle = atan2(w.y,w.x);
	ph->alpha = angle;
}

double photon_distance_intersection(SEGMENT s, VECTEUR pos, double angle)		
{
	//VECTEUR intersect;
	//on utilise éq cartésienne droite:ax+by+c=0
	double a = s.deb.y-s.fin.y;
	double b = s.fin.x-s.deb.x;
	double c = -a*s.deb.x-b*s.deb.y;
	
	double xp = pos.x;
	double yp = pos.y;
	
	//on utilise éq de la trajectoire du photon
	double d = (-c-b*yp-a*xp)/(a*cos(angle)+b*sin(angle));	//distance entre photon et pt intersection
	
	return d;
}

int photon_add_ph(PHOTON ph)
{
	PHOTON_LISTE* liste_ph;
	if(!(liste_ph = malloc(sizeof(PHOTON_LISTE))))
	{
		error_msg("Erreur dans allocation mémoire de photon");
		return 1;
	}
	
	liste_ph->id = index_count;
	liste_ph->ph = ph;
	liste_ph->suivant = tete;
	tete = liste_ph;
	nb_element_ph++;
	index_count++;
	
	return 0;
}

void photon_add(VECTEUR position, double angle)
{
	PHOTON ph;
	ph.position = position;
	ph.alpha = angle;
	
	photon_add_ph(ph);
}

int photon_retirer(int id)
{
	PHOTON_LISTE* element = tete;
	PHOTON_LISTE* precedent = NULL;
	//liberer les éléments
	while(element){
		if(element->id == id)
		{
			if(tete->id == id){
				tete = tete->suivant;
			}
			else{
				precedent->suivant = element->suivant;
			}
			free(element);
			nb_element_ph--;
			return 0;
		}
		else
		{
			precedent = element;
			element = element->suivant;
		}
	}
	
	return 1;
}

void photon_destruction_dehors(double xmin, double xmax,double ymin, double ymax)
{
	PHOTON_LISTE* element = tete;
	while (element)
	{
		if ((element->ph).position.x < xmin || (element->ph).position.x > xmax
			||(element->ph).position.y < ymin || (element->ph).position.y > ymax)
			{
				photon_retirer(element->id);
			}
		
		element = element->suivant;
		
	}
}


void photon_print_file(FILE* file)
{
	fprintf(file, "#photon\n");
	fprintf(file, "%d\n", nb_element_ph);
	PHOTON_LISTE* element = tete;
	while(element != NULL)
	{
		fprintf(file, "%f %f %f \n", (element->ph).position.x, (element->ph).position.y, (element->ph).alpha);
		element = element->suivant;
	}
	fprintf(file, "FIN_LISTE\n\n");
	
}

void photon_drawing()
{
	graphic_set_line_width(2.);
	graphic_set_color3f(0.0,0.0,0.0);
	
	PHOTON_LISTE* element = tete;
	while(element != NULL)
	{
		graphic_draw_circle((element->ph).position.x, (element->ph).position.y, EPSIL_PROJ/2, GRAPHIC_EMPTY);
		element = element->suivant;
	}
}



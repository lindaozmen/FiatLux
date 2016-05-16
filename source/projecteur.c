#include "projecteur.h"
#include "graphic.h"
#include "photon.h"
#include <math.h>
#include <GL/glu.h>

static int nb_element_pr = 0;
static int nb_expected_pr = 0;

struct Projecteur
{
	VECTEUR position;
	double alpha;
};

static PROJECTEUR* tab_pr;

int verifier_fin_projecteur()
{
	if((nb_element_pr) < nb_expected_pr)
	{
		error_lecture_elements(ERR_PROJECTEUR, ERR_PAS_ASSEZ);
		return 1;
	}
	else if((nb_element_pr) > nb_expected_pr)
	{
		error_lecture_elements(ERR_PROJECTEUR, ERR_TROP);
		return 1;
	}
	return 0;
}

int projecteur_load(char * tab)
{
	char *deb = tab; 
	PROJECTEUR pr;
	
	if(tab_pr == NULL)
	{
		if(sscanf(deb, "%d", &nb_expected_pr) != 1)
		{
			error_lect_nb_elements(ERR_PROJECTEUR);
			return 1;
		}
		
		if(nb_expected_pr < 0)
		{
			error_lect_nb_elements(ERR_PROJECTEUR);
			return 1;
		}
		
		if(!(tab_pr = malloc(nb_expected_pr*sizeof(PROJECTEUR))))
		{
			error_msg("Erreur dans allocation mémoire de l'projecteur");
			return 1;
		}
	}
	else
	{
		if(sscanf(deb,"%lf %lf %lf", &pr.position.x, &pr.position.y, &pr.alpha) != 3)
		{
			error_lecture_elements(ERR_PROJECTEUR, ERR_PAS_ASSEZ);
			return 1;
		}
		projecteur_add_pr(pr);
	}
	
	return 0;	
}
	
void projecteur_free()
{
	free(tab_pr);
	tab_pr = NULL;
	nb_element_pr = 0;
	nb_expected_pr = 0;
}

void projecteur_retirer(int id)
{
	PROJECTEUR* temp = malloc((nb_element_pr-1)*sizeof(PROJECTEUR));
	int i, j = 0;
	
	for (i = 0; i < nb_element_pr; i++)
	{
		if (i == id)	
			continue;
			
		*(temp+j) = *(tab_pr+i);
		j++; 
	}
	nb_element_pr--;
	free(tab_pr);
	tab_pr = temp;
}

//method pour accéder aux éléments
int projecteur_get_nombre()	
{
	return nb_element_pr;
}


int projecteur_dectection_collision(SEGMENT s)
{
	double longueur, angle;
	VECTEUR v1, v2;
	SEGMENT s_pr;
	
	int i;
	for(i = 0; i < nb_element_pr; i++)
	{
		longueur = NBPH*EPSIL_PROJ;
		angle = (tab_pr+i)->alpha;

		v1 = (tab_pr + i)->position;
		v2.x = ((tab_pr+i)->position).x-longueur*sin(angle);
		v2.y = ((tab_pr+i)->position).y+longueur*cos(angle);				
		s_pr.deb = v1;
		s_pr.fin = v2;
		if(detection_intersection(s, s_pr) == 0)
			return 1;
	}

	return 0;
}

int projecteur_plus_proche_selection(VECTEUR vect_coordonne)
{
	double longueur = NBPH*EPSIL_PROJ;
	double angle = (tab_pr)->alpha;
	
	VECTEUR point = vect_coordonne;
	VECTEUR vect = {((tab_pr)->position).x, ((tab_pr)->position).y};
	double min_distance = calculate_distance(point, vect);
	double min_temporaire = 0;
	int min_i = 0;
	int i;
	for(i = 0; i < nb_element_pr; i++)
	{
		angle = (tab_pr+ i)->alpha;
	
		vect.x = ((tab_pr+i)->position).x-longueur*sin(angle)/2.0;
		vect.y = ((tab_pr+i)->position).y+longueur*cos(angle)/2.0;
					
		min_temporaire = calculate_distance(point, vect);
		if(min_temporaire < min_distance)
		{
			min_distance = min_temporaire;
			min_i = i;
		}
	}
	
	for(i = 0; i < nb_element_pr; i++)
	{
		angle = (tab_pr+ i)->alpha;
	
		vect.x = ((tab_pr+i)->position).x;
		vect.y = ((tab_pr+i)->position).y;
					
		min_temporaire = calculate_distance(point, vect);
		if(min_temporaire < min_distance)
		{
			min_distance = min_temporaire;
			min_i = i;
		}
	}
	return min_i;
}

void projecteur_print_file(FILE* file)
{
	fprintf(file, "#projecteur\n");
	fprintf(file, "%d\n", nb_element_pr);
	int i;
	for(i = 0; i < nb_element_pr; i++)
	{
		fprintf(file, "%f %f %f ", ((tab_pr+i)->position).x, ((tab_pr+i)->position).y, (tab_pr+i)->alpha);
		fprintf(file, "\n");
	}
	fprintf(file, "FIN_LISTE\n\n");
}


int projecteur_add_pr(PROJECTEUR pr)
{
	if(tab_pr == NULL)
	{
		if(!(tab_pr = malloc(sizeof(PROJECTEUR))))
			return 1;
	}
	else if(nb_element_pr >= nb_expected_pr)
	{
		if(!(tab_pr = realloc(tab_pr,sizeof(PROJECTEUR)*(nb_element_pr+1))))
			return 1;
	}
	*(tab_pr+nb_element_pr) = pr;
	nb_element_pr++;

	return 0;
}

void projecteur_creation(VECTEUR deb, VECTEUR fin)
{
	PROJECTEUR pr;
	pr.position = deb;
	VECTEUR v = vecteur_difference(deb, fin);
	pr.alpha = atan2(v.y, v.x);
	
	projecteur_add_pr(pr);
}

void projecteur_creer_photon()
{
	int i, j;
	VECTEUR position;
	double alpha;
	
	for(i = 0; i < nb_element_pr; i++)
	{
		position = (tab_pr+i)->position;
		alpha = (tab_pr+i)->alpha;
		for(j = 0; j < NBPH; j++)
		{
			photon_add(position, alpha);
			position.x -= EPSIL_PROJ*sin(alpha);
			position.y -= EPSIL_PROJ*cos(alpha);
		}
	}
}

SEGMENT projecteur_get_segment(int id)
{
	double longueur = NBPH*EPSIL_PROJ;
	double angle = (tab_pr+id)->alpha;


	VECTEUR v1 = (tab_pr + id)->position;
	VECTEUR v2 = {((tab_pr+id)->position).x-longueur*sin(angle), 
							((tab_pr+id)->position).y+longueur*cos(angle)};
	SEGMENT s = {v1, v2};
	return s;
}

void projecteur_draw_obj(int id)
{
	double longueur = NBPH*EPSIL_PROJ;
	double angle = (tab_pr+id)->alpha;

	graphic_set_line_width(3.);
	graphic_set_color3f(0.0,1.0,0.0);
	graphic_draw_segment(((tab_pr+id)->position).x, ((tab_pr+id)->position).y,
						 ((tab_pr+id)->position).x-longueur*sin(angle), //car on prend alpha+pi/2
						 ((tab_pr+id)->position).y+longueur*cos(angle));

}

void projecteur_drawing()
{
	int id;
	for(id=0;id < nb_element_pr;id++)
	{
		projecteur_draw_obj(id);
	}

}

void projecteur_draw_selection(int id)
{
	double longueur = NBPH*EPSIL_PROJ;
	double angle = (tab_pr+id)->alpha;
	
	graphic_set_line_width(4.);
	graphic_set_color3f(1,1,0);

	graphic_draw_segment(((tab_pr+id)->position).x, ((tab_pr+id)->position).y,
						((tab_pr+id)->position).x-longueur*sin(angle), //car on prend alpha+pi/2
						((tab_pr+id)->position).y+longueur*cos(angle));

}

int projecteur_get_deb_x(int id){
	return (tab_pr+id)->position.x;
}
int projecteur_get_deb_y(int id){
	return (tab_pr+id)->position.y;
}
int projecteur_get_fin_x(int id){
	
	double longueur = NBPH*EPSIL_PROJ;
	double angle = (tab_pr+id)->alpha;
	return ((tab_pr+id)->position).x-longueur*sin(angle);
}
int projecteur_get_fin_y(int id){
	
	double longueur = NBPH*EPSIL_PROJ;
	double angle = (tab_pr+id)->alpha;
	return ((tab_pr+id)->position).y+longueur*cos(angle);
}

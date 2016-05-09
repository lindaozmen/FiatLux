#include "reflecteur.h"
#include "graphic.h"

static int nb_element_r = 0;
static int nb_expected_r = 0;

struct Reflecteur
{
	VECTEUR deb, fin;
};

static REFLECTEUR* tab_r;

int verifier_fin_reflecteur()
{
	if((nb_element_r) < nb_expected_r)
	{
		error_lecture_elements(ERR_REFLECTEUR, ERR_PAS_ASSEZ);
		return 1;
	}
	else if((nb_element_r) > nb_expected_r)
	{
		error_lecture_elements(ERR_REFLECTEUR, ERR_TROP);
		return 1;
	}
	return 0;
}


int reflecteur_load(char * tab)
{
	char *deb = tab; 
	REFLECTEUR r;
	
	if(tab_r == NULL)
	{
		if((sscanf(deb,"%d",&nb_expected_r)) != 1)
		{
			error_lect_nb_elements(ERR_REFLECTEUR);
			return 1;
		}
			
		if(nb_expected_r < 0)
		{
			error_lect_nb_elements(ERR_REFLECTEUR);
			return 1;
		}
		
		if(!(tab_r = malloc(nb_expected_r*sizeof(REFLECTEUR))))
		{
			error_msg("Erreur dans allocation mémoire de l'reflecteur");
			return 1;
		}
	}
	else
	{
		if(sscanf(deb,"%lf %lf %lf %lf", &r.deb.x, &r.deb.y,&r.fin.x,&r.fin.y) != 4)
		{
			error_lecture_elements(ERR_REFLECTEUR, ERR_PAS_ASSEZ);
			return 1;
		}
		if(calculate_distance(r.deb, r.fin) < EPSIL_CREATION)
		{
			error_lecture_point_trop_proche(ERR_REFLECTEUR, nb_element_r);
			return 1;
		}
		reflecteur_add_r(r);
	}
	
	return 0;	
}

void reflecteur_free()
{
	free(tab_r);
	tab_r = NULL;
	nb_element_r = 0;
	nb_expected_r = 0;
}

//method pour accéder aux éléments
int reflecteur_get_nombre()	
{
	return nb_expected_r;
}

int reflecteur_dectection_collision(SEGMENT s)
{
	int i;
	for(i = 0; i < nb_expected_r; i++)
	{
		SEGMENT s_r = {(tab_r + i)->deb, (tab_r + i)->fin};
		if(detection_intersection(s, s_r) == 0)
			return 1;
	}
	
	return 0;
}

void reflecteur_print_file(FILE* file)
{
	fprintf(file, "#reflecteur\n");
	fprintf(file, "%d\n", nb_element_r);
	int i;
	for(i = 0; i < nb_element_r; i++)
	{
		fprintf(file, "%f %f %f %f ", ((tab_r+i)->deb).x, ((tab_r+i)->deb).y, 
										((tab_r+i)->fin).x, ((tab_r+i)->fin).y);
		fprintf(file, "\n");
	}
	fprintf(file, "FIN_LISTE\n\n");
}

int reflecteur_add_r(REFLECTEUR r)
{
	if(tab_r == NULL)
	{
		if(!(tab_r = malloc(sizeof(REFLECTEUR))))
			return 1;
	}
	
	*(tab_r+nb_element_r) = r;
	nb_element_r++;

	return 0;
}

SEGMENT reflecteur_get_segment(int id)
{
	VECTEUR v1 = (tab_r + id)->deb;
	VECTEUR v2 = (tab_r + id)->fin;
	
	SEGMENT s;
	
	return s;
}

//methode pour dessiner réflecteur
void reflecteur_drawing()
{
	int i;
	for(i = 0; i < nb_expected_r; i++)
		reflecteur_draw_obj(i);
}

void reflecteur_draw_obj(int id_r)
{
	graphic_set_line_width(2.);
	graphic_set_color3f(1,0,0);
	
	graphic_draw_segment((tab_r+id_r)->deb.x, (tab_r+id_r)->deb.y,
	                     (tab_r+id_r)->fin.x,(tab_r+id_r)->fin.y);
}
	
int reflecteur_get_deb_x(int id){
	return (tab_r+id)->deb.x;
}
int reflecteur_get_deb_y(int id){
	return (tab_r+id)->deb.y;
}
int reflecteur_get_fin_x(int id){
	return (tab_r+id)->fin.x;
}
int reflecteur_get_fin_y(int id){
	return (tab_r+id)->fin.y;
}

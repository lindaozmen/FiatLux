#include "absorbeur.h"
#include "graphic.h"
#include "tolerance.h"


static int nb_element_a = 0;
static int nb_expected_a = 0;

struct Absorbeur
{
	int nbp;
	VECTEUR* elements;
};

static ABSORBEUR* tab_a = NULL;

int verifier_fin_absorbeur()
{
	if((nb_element_a) < nb_expected_a)
	{
		error_lecture_elements(ERR_ABSORBEUR, ERR_PAS_ASSEZ);
		return 1;
	}
	else if((nb_element_a) > nb_expected_a)
	{
		error_lecture_elements(ERR_ABSORBEUR, ERR_TROP);
		return 1;
	}
	return 0;
}

int absorbeur_load(char * tab)	//on lit ligne par ligne
{
	char *deb = tab; 
	char* fin = NULL;
	ABSORBEUR a;
	
	if(tab_a == NULL)
	{
		if(sscanf(deb, "%d", &nb_expected_a) != 1)
		{
			error_lect_nb_elements(ERR_ABSORBEUR);
			return 1;
		}
		
		if(nb_expected_a < 0)
		{
			error_lect_nb_elements(ERR_ABSORBEUR);
			return 1;
		}
		
		if(!(tab_a = malloc(nb_expected_a*sizeof(ABSORBEUR))))
		{
			error_msg("Erreur dans allocation mémoire de l'absorbeur");
			return 1;
		}
	}
	else
	{
		if(sscanf(deb, "%d", &a.nbp) != 1)
		{
			error_lecture_elements(ERR_ABSORBEUR, ERR_PAS_ASSEZ);
			return 1;
		}
		if(a.nbp < 2 || a.nbp > MAX_PT){
			error_lect_nb_points_absorbeur();
			return 1;
		}
		
		if(!(a.elements = malloc(a.nbp*sizeof(VECTEUR))))
			return 1;
		strtod(deb,&fin); 
		deb = fin;
		
		int i;
		for(i = 0; i < a.nbp; i++)
		{
			if(sscanf(deb,"%lf %lf", &(a.elements+i)->x, &(a.elements+i)->y) != 2)
			{
				error_lecture_elements(ERR_ABSORBEUR, ERR_PAS_ASSEZ);
				return 1;
			}
			strtod(deb,&fin);
			deb = fin;
			strtod(deb,&fin); 
			deb = fin;
			
		}
		for(i = 1; i < a.nbp; i++)
		{
			if(calculate_distance(*(a.elements+i), *(a.elements+i-1)) < EPSIL_CREATION)
			{
				error_lecture_point_trop_proche(ERR_ABSORBEUR, nb_element_a);
				return 1;
			}
		}
		
		absorbeur_add_a(a);
	}
	return 0;	
}
	
void absorbeur_free()
{
	int i;
	for(i = 0; i < nb_element_a; i++)
	{
		free((tab_a+i)->elements);
		(tab_a+i)->elements = NULL;
	}
	free(tab_a);
	tab_a = NULL;
	nb_element_a = 0;
	nb_expected_a = 0;
	
}

int absorbeur_dectection_collision(SEGMENT s)
{

	int i, j;
	for(i = 0; i < nb_expected_a; i++)
	{
		for(j = 0; j < tab_a[i].nbp-1; j++)
		{	
			SEGMENT s_a = {*((tab_a+i)->elements+j), *((tab_a+i)->elements+j+1)};
			if(detection_intersection(s, s_a) == 0)
				return 1;
		}
	}
	return 0;
}

void absorbeur_print_file(FILE* file)
{
	fprintf(file, "#absorbeur\n");
	fprintf(file, "%d\n", nb_element_a);
	int i, j;
	for(i = 0; i < nb_element_a; i++)
	{
		fprintf(file, "%d ", (tab_a+i)->nbp);
		for(j = 0; j < (tab_a+i)->nbp; j++)
		{
			fprintf(file, "%f %f ", ((tab_a+i)->elements)->x, ((tab_a+i)->elements)->y);
		}
		fprintf(file, "\n");
	}
	fprintf(file, "FIN_LISTE\n\n");
}

int absorbeur_add_a(ABSORBEUR a)
{
	if(tab_a == NULL)
	{
		if(!(tab_a = malloc(sizeof(ABSORBEUR))))
			return 1;
	}
	
	*(tab_a+nb_element_a) = a;
	nb_element_a++;
	
	return 0;
}


int absorbeur_get_nombre()
{
	return nb_expected_a;
}

int absorbeur_nombre_vecteur(int id)
{
	return (tab_a+id)->nbp;
}

SEGMENT* absorbeur_get_segment(int id)
{
	SEGMENT* a = malloc(((tab_a+id)->nbp-1)*sizeof(SEGMENT));
	int i;
	for(i = 0; i < (tab_a+id)->nbp-1; i++)
	{
		VECTEUR v1 = *((*(tab_a+id)).elements+i);
		VECTEUR v2 = *((*(tab_a+id)).elements+i+1);
		SEGMENT s = {v1, v2};
		*(a+i) = s;
	}
	return a;
}
		

void absorbeur_draw_obj(int id, int id_pt)
{
	double debx = ((tab_a+id)->elements+id_pt)->x;
	double deby = ((tab_a+id)->elements+id_pt)->y;
	double finx = ((tab_a+id)->elements+id_pt+1)->x;
	double finy = ((tab_a+id)->elements+id_pt+1)->y;
	
	graphic_set_line_width(3.);
	graphic_set_color3f(0,0,1);
	
	graphic_draw_segment(debx,deby,finx,finy);
}

void absorbeur_drawing()
{
	int i,j;
	for(i=0;i<nb_element_a;i++)
	{
		for(j=0;j<((tab_a+i)->nbp)-1;j++)
		{
			absorbeur_draw_obj(i,j);
		}
	}
}
int absorbeur_get_x(int id, int nb){
	return ((tab_a+id)->elements+nb)->x;
}
int absorbeur_get_y(int id, int nb){
	return ((tab_a+id)->elements+nb)->y;
}

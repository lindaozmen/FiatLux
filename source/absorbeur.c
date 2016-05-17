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
	for(i = 0; i < nb_element_a; i++)
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

int absorbeur_plus_proche_selection(VECTEUR vect_coordonne)
{
	VECTEUR point = vect_coordonne;
	VECTEUR vect = {(tab_a->elements)->x, (tab_a->elements)->y};
	double min_distance = calculate_distance(point, vect);
	double min_temporaire = 0;
	int min_i = 0;
	int i, j;
	for(i = 0; i < nb_element_a; i++)
	{
		for(j = 0; j < tab_a[i].nbp-1; j++)
		{	
			vect.x = ((tab_a+i)->elements+j)->x;
			vect.y = ((tab_a+i)->elements+j)->y;
						
			min_temporaire = calculate_distance(point, vect);
			if(min_temporaire < min_distance)
			{
				min_distance = min_temporaire;
				min_i = i;
			}
		}
	}
	return min_i;
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
	else if(nb_element_a >= nb_expected_a)
	{
		if(!(tab_a = realloc(tab_a,sizeof(ABSORBEUR)*(nb_element_a+1))))
			return 1;
	}
	*(tab_a+nb_element_a) = a;
	nb_element_a++;
	
	return 0;
}

void absorbeur_creation(VECTEUR* v, int nb_point)
{
	ABSORBEUR abs;
	abs.nbp = nb_point;
	abs.elements = malloc(sizeof(VECTEUR)*nb_point);
	int i = 0;
	for(i = 0; i < nb_point; i++){
		*(abs.elements+i)=*(v+i);
	}
	absorbeur_add_a(abs);
}

void absorbeur_ramplacer_dernier(VECTEUR* v, int nb_point)
{
	(tab_a+nb_element_a-1)->elements = realloc((tab_a+nb_element_a-1)->elements, sizeof(VECTEUR)*nb_point);
	*((tab_a+nb_element_a-1)->elements +nb_point-1) = *(v+nb_point-1);
	(tab_a+nb_element_a-1)->nbp = nb_point;
}

void absorbeur_retirer(int id)
{
	ABSORBEUR* temp = malloc((nb_element_a-1)*sizeof(ABSORBEUR));
	int i, j = 0;
	
	for (i = 0; i < nb_element_a; i++)
	{
		if (i == id)	
			continue;
				
		*(temp+j) = *(tab_a+i);
		j++; 
	}
	nb_element_a --;
	free((tab_a+id)->elements);
	free(tab_a);
	tab_a = temp;
}
	
	
int absorbeur_get_nombre()
{
	return nb_element_a;
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

void absorbeur_draw_selection(int id)
{
	graphic_set_line_width(4.);
	graphic_set_color3f(1,1,0);
	
	int i;
	for(i = 0; i < (tab_a+id)->nbp-1; i++)
	{
		graphic_draw_segment(((tab_a+id)->elements+i)->x, ((tab_a+id)->elements+i)->y,
							((tab_a+id)->elements+i+1)->x, ((tab_a+id)->elements+i+1)->y);
	}
}

int absorbeur_get_x(int id, int nb){
	return ((tab_a+id)->elements+nb)->x;
}
int absorbeur_get_y(int id, int nb){
	return ((tab_a+id)->elements+nb)->y;
}

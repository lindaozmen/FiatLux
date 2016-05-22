#include "modele.h"
#include "graphic.h"
#include <math.h>
#include <stdio.h>

int id_select = -1;
int nb_element_creation = 0;
int lastMode = -1;
int lastChoix = -1;
VECTEUR v_tab[MAX_PT];

int modele_lecture(char * nomfichier, int mode_verification)
{
	enum Etat_lecture {projecteur, reflecteur, absorbeur, photon, fini};
	int etat = 0;
	char tab[MAX_LINE];
	FILE * fentree = NULL;
	id_select = -1;
	
	fentree = fopen(nomfichier, "r");
	if(fentree == NULL)
	{
		error_fichier_inexistant();
		return 1;
	}
	else
	{
		while(etat != fini && fgets(tab, MAX_LINE, fentree))
		{
			if((tab[0] == '#') || (tab[0] == '\n') || (tab[0] == '\r')){
				continue;
			}
			switch (etat)
			{
				case projecteur : 
					if(strncmp(tab, "FIN_LISTE", 9) == 0)
					{
						etat++;
						if(verifier_fin_projecteur() == 1)
							return 1;
					}
					else if(projecteur_load(tab) == 1)
						return 1;
				break;
				case reflecteur : 
					if(strncmp(tab, "FIN_LISTE", 9) == 0)
					{		
						etat ++;
						if(verifier_fin_reflecteur() == 1)	
							return 1;
					}
					else if(reflecteur_load(tab) == 1)
						return 1;
						
				break;	
				case absorbeur : 
					if(strncmp(tab, "FIN_LISTE", 9) == 0)
					{
						etat ++;
						if(verifier_fin_absorbeur() == 1)
							return 1;
					}
					else if(absorbeur_load(tab) == 1)
						return 1;
						
				break;	
				case photon : 
					if(strncmp(tab, "FIN_LISTE", 9) == 0)
					{
						etat ++;
						if(verifier_fin_photon() == 1){
							return 1;
						}
					}
					else if(photon_load(tab) == 1)
						return 1;					
				break;
				
				default:
				break;
			}
		}
		if(etat != fini)
		{
			error_fichier_incomplet();
			return 1;
		}
		else
		{
			if(mode_verification == 0)
				error_success();
			return 0;
		}
	}
	return 0;
}


void modele_destruction_simulation()
{
	projecteur_free();
	reflecteur_free();
	absorbeur_free();
	photon_free();
}

void modele_lecture_rendu2()
{
	if(modele_verifier_collision() == 1)
		modele_destruction_simulation();
}

int modele_verifier_collision(){
	
	int i, j, w, v, t;
	
	for(i = 0; i < absorbeur_get_nombre(); i++)
	{
		for(j = 0; j < absorbeur_nombre_vecteur(i)-1; j++)
		{
			SEGMENT s_a = {absorbeur_get_vecteur(i,j+1), absorbeur_get_vecteur(i,j)};
			for(w = 0; w < reflecteur_get_nombre(); w++)
			{
				SEGMENT s_r = reflecteur_get_segment(w);
				if(utilitaire_detection_parallelisme(s_a,s_r) == 1)
				{
					error_lecture_intersection(ERR_ABSORBEUR, i, ERR_REFLECTEUR,w);
					return 1;
				}
				for(v = 0; v < projecteur_get_nombre(); v++){
					SEGMENT s_p = projecteur_get_segment(v);
					
					if(utilitaire_detection_parallelisme(s_p,s_r) == 1)
					{
						error_lecture_intersection(ERR_PROJECTEUR, v, ERR_REFLECTEUR,w);
						return 1;
					}
					if(utilitaire_detection_parallelisme(s_a,s_p) == 1)
					{
						error_lecture_intersection(ERR_ABSORBEUR, i, ERR_PROJECTEUR,v);
						return 1;
					}
				}
			}
		}
	}
	//Verifier reflecteurs
	for(w = 0; w < reflecteur_get_nombre(); w++){
				SEGMENT s_r = reflecteur_get_segment(w);
				for(t = w+1; t < reflecteur_get_nombre(); t++){
					SEGMENT s_r2 = reflecteur_get_segment(t);
					if(utilitaire_detection_parallelisme(s_r,s_r2) == 1)
					{
						error_lecture_intersection(ERR_REFLECTEUR,t, ERR_REFLECTEUR, w);
						return 1;
					}
				}
	}
	return 0;
}
void modele_check_radio(int mode, int choix){
	if(lastMode != mode || lastChoix != choix){
		lastMode = mode;
		lastChoix = choix;
		id_select = -1;
		nb_element_creation = 0;
	}
}
void modele_element_plus_proche(VECTEUR vect_coordonne)
{
	if(lastChoix == 1) //cas reflecteur
		{
			if(reflecteur_get_nombre() > 0)
				id_select = reflecteur_plus_proche_selection(vect_coordonne);
		}
		else if(lastChoix== 2) //cas absorbeur
		{
			if(absorbeur_get_nombre() > 0)
				id_select = absorbeur_plus_proche_selection(vect_coordonne);
		}
		else //cas projecteur (par défaut)
		{
			if(projecteur_get_nombre() > 0)
				id_select = projecteur_plus_proche_selection(vect_coordonne);
		}
}

void modele_effacer_element()
{
	
	if (lastMode == 0 && id_select >= 0)
	{
		if(lastChoix == 1) //cas reflecteur
		{
			reflecteur_retirer(id_select);
		}
		else if(lastChoix== 2) //cas absorbeur
		{
			absorbeur_retirer(id_select);
		}
		else //cas projecteur (par défaut)
		{
			projecteur_retirer(id_select);
		}
		id_select = -1;
	}
}

int modele_creation(VECTEUR v)
{
	if (nb_element_creation < MAX_PT)
	{
		v_tab[nb_element_creation] = v;
		nb_element_creation ++;
				
		if (nb_element_creation >= 2)
		{
			if (lastChoix == 1)
			{
				reflecteur_creation(v_tab[0], v_tab[1]);
				nb_element_creation = 0;
				if(modele_verifier_collision() == 1)
				{
						reflecteur_retirer(reflecteur_get_nombre()-1);
						return 1;
				}
			}
			else if (lastChoix == 2)
			{
				if (nb_element_creation > 2)
				{
					absorbeur_ramplacer_dernier(v_tab, nb_element_creation);
				}
				else
				{
					absorbeur_creation(v_tab, nb_element_creation);
				}
				if (modele_verifier_collision() == 1)
				{
					absorbeur_retirer(absorbeur_get_nombre()-1);
					nb_element_creation = 0;
					return 1;
				} 
					
			}
			else
			{
				projecteur_creation(v_tab[0], v_tab[1]);
				nb_element_creation = 0;
				if(modele_verifier_collision() == 1)
				{
					projecteur_retirer(projecteur_get_nombre()-1);
					return 1;	
				}
			}
		
		}
	}
	else
	{
		printf("Ce point est ignoré car le nombre de points max est déjà atteint\n");

	}
	
	return 0;
}
	
	
void modele_draw_all()
{
	projecteur_drawing();
	reflecteur_drawing();
	absorbeur_drawing();
	photon_drawing();

}

void modele_draw()
{
	modele_draw_all();
	
	if(id_select >= 0){
		if(lastMode == 0)
		{
			if(lastChoix == 1) //cas reflecteur
			{
				reflecteur_draw_selection(id_select);
			}
			else if(lastChoix == 2) //cas absorbeur
			{
				absorbeur_draw_selection(id_select);
			}
			else //cas projecteur (par défaut)
			{
				projecteur_draw_selection(id_select);
			}
		}
	}
}

void modele_dessin_carre_zoom(VECTEUR dessin_start, VECTEUR dessin_end)
{
	graphic_set_color3f(0.0, 0.0, 0.0);
	double xc = (dessin_start.x + dessin_end.x)/2;
	double yc = (dessin_start.y + dessin_end.y)/2; 
	double w = fabs(dessin_start.x - dessin_end.x);
	double h = fabs(dessin_start.y - dessin_end.y);
	graphic_draw_rectangle(xc, yc, w, h, GRAPHIC_EMPTY);
}	


void modele_destruction_photon_dehors(double xmin, double xmax,double ymin, double ymax)
{
	photon_destruction_dehors(xmin, xmax, ymin, ymax);
}

void modele_save_elements(char* nom_fichier)
{
	FILE* fsave = fopen(nom_fichier, "w");
	projecteur_print_file(fsave);
	reflecteur_print_file(fsave);
	absorbeur_print_file(fsave);
	photon_print_file(fsave);
	fclose(fsave);
}


#include "modele.h"
#include <stdio.h>

int modele_lecture(char * nomfichier, int mode_verification)
{
	enum Etat_lecture {projecteur, reflecteur, absorbeur, photon, fini};
	int etat = 0;
	char tab[MAX_LINE];
	FILE * fentree = NULL;
	
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

void modele_lecture_rendu2()
{
	int i,j;
	//Reflecteur absorbeur
	int w;
	for(i = reflecteur_get_nombre()-1; i >=0; i--){
		SEGMENT s1;
		s1.deb.x = reflecteur_get_deb_x(i);
		s1.deb.y = reflecteur_get_deb_y(i);
		s1.fin.x = reflecteur_get_fin_x(i);
		s1.fin.y = reflecteur_get_fin_y(i);
		for(j = 0; j < absorbeur_get_nombre(); j++){
			for(w = 0; w < absorbeur_nombre_vecteur(j)-1; w++){
				SEGMENT s2;
				s2.deb.x = absorbeur_get_x(j,w);
				s2.deb.y = absorbeur_get_y(j,w);
				s2.fin.x = absorbeur_get_x(j,w+1);
				s2.fin.y = absorbeur_get_y(j,w+1);
				if(detection_parallelisme(s1,s2) == 1){
					if(detection_intersection(s1,s2) == 0){
						error_lecture_intersection(ERR_ABSORBEUR, j, ERR_REFLECTEUR,i);
						destruction_simulation();
						return;
				}
			 }
			}
		}
	}
	
	//Projecteur absorbeur
	for(i = projecteur_get_nombre()-1; i >=0; i--){
		SEGMENT s1;
		s1.deb.x = projecteur_get_deb_x(i);
		s1.deb.y = projecteur_get_deb_y(i);
		s1.fin.x = projecteur_get_fin_x(i);
		s1.fin.y = projecteur_get_fin_y(i);
		for(j = 0; j < absorbeur_get_nombre(); j++){
			for(w = 0; w < absorbeur_nombre_vecteur(j)-1; w++){
				SEGMENT s2;
				s2.deb.x = absorbeur_get_x(j,w);
				s2.deb.y = absorbeur_get_y(j,w);
				s2.fin.x = absorbeur_get_x(j,w+1);
				s2.fin.y = absorbeur_get_y(j,w+1);
				if(detection_parallelisme(s1,s2) == 1){
					if(detection_intersection(s1,s2) == 0){
						error_lecture_intersection(ERR_ABSORBEUR, j, ERR_PROJECTEUR,i);
						destruction_simulation();
						return;
				}
			 }
			}
		}
	}
	//Reflecteur avec reflecteur
	for(i = reflecteur_get_nombre()-1; i >=0; i--){
		SEGMENT s1;
		s1.deb.x = reflecteur_get_deb_x(i);
		s1.deb.y = reflecteur_get_deb_y(i);
		s1.fin.x = reflecteur_get_fin_x(i);
		s1.fin.y = reflecteur_get_fin_y(i);
		for(j = 0; j < reflecteur_get_nombre(); j++){
			SEGMENT s2;
			s2.deb.x = reflecteur_get_deb_x(j);
			s2.deb.y = reflecteur_get_deb_y(j);
			s2.fin.x = reflecteur_get_fin_x(j);
			s2.fin.y = reflecteur_get_fin_y(j);
			if(detection_parallelisme(s1,s2) == 1){
				if(detection_intersection(s1,s2) == 0){
					error_lecture_intersection(ERR_REFLECTEUR, i, ERR_REFLECTEUR,j);
					destruction_simulation();
					return;
				}
				 
			}
		}
	}	
	//Projecteur reflecteur
	for(i = projecteur_get_nombre()-1; i >=0; i--){
		SEGMENT s1;
		s1.deb.x = projecteur_get_deb_x(i);
		s1.deb.y = projecteur_get_deb_y(i);
		s1.fin.x = projecteur_get_fin_x(i);
		s1.fin.y = projecteur_get_fin_y(i);
		for(j = 0; j < reflecteur_get_nombre(); j++){
			SEGMENT s2;
			s2.deb.x = reflecteur_get_deb_x(j);
			s2.deb.y = reflecteur_get_deb_y(j);
			s2.fin.x = reflecteur_get_fin_x(j);
			s2.fin.y = reflecteur_get_fin_y(j);
			if(detection_parallelisme(s1,s2) == 1){
				if(detection_intersection(s1,s2) == 0){
					error_lecture_intersection(ERR_PROJECTEUR, i, ERR_REFLECTEUR,j);
					destruction_simulation();
					return;
				}
				 
			}
		}
	}
	
}

void destruction_simulation()
{
	projecteur_free();
	reflecteur_free();
	absorbeur_free();
	photon_free();
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


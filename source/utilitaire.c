#include "utilitaire.h"
#include "constantes.h"
#include <math.h>
#include <stdio.h>

double utilitaire_calculate_distance(VECTEUR vect1, VECTEUR vect2)	
{
	double difx = vect2.x - vect1.x;
	double dify = vect2.y - vect1.y;
	
	double distance = sqrt(dify*dify + difx*difx);
	
	return distance;
}

double utilitaire_calculate_norme_vecteur(VECTEUR v)
{
	double norme = sqrt(v.x*v.x + v.y*v.y);
	
	return norme;
}

double utilitaire_calculate_norme_segment(SEGMENT s)
{
	double norme_x = s.fin.x - s.deb.x;
	double norme_y = s.fin.y - s.deb.y;
	
	double norme = sqrt(norme_y*norme_y + norme_x*norme_x);
	
	return norme;
}

VECTEUR utilitaire_vecteur_difference(VECTEUR vect1, VECTEUR vect2)	//on crée le vecteur entre les deux vecteur
{
	VECTEUR new_vect;
	
	new_vect.x = vect2.x - vect1.x;
	new_vect.y = vect2.y - vect1.y;
	
	return new_vect;
}

VECTEUR utilitaire_calculer_difference_unitaire(SEGMENT s)
{
	double norme = utilitaire_calculate_norme_segment(s);
	
	VECTEUR vect = utilitaire_vecteur_difference(s.deb, s.fin);
	VECTEUR vect_unitaire = {vect.x/norme, vect.y/norme};
	
	return vect_unitaire;
}

VECTEUR utilitaire_calculer_vecteur_unitaire(VECTEUR v)
{
	double norme = utilitaire_calculate_norme_vecteur(v);
	
	VECTEUR vect_unitaire = {v.x/norme, v.y/norme};
	
	return vect_unitaire;
}
	
VECTEUR utilitaire_calculer_difference_normal(SEGMENT s)	
{
	VECTEUR vect = utilitaire_calculer_difference_unitaire(s);
	VECTEUR vect_normal = { -vect.y, vect.x };

	return vect_normal;
}

VECTEUR utilitaire_calculer_sens_inverse(VECTEUR v)
{
	VECTEUR v_inverse = {-v.x, -v.y};
	
	return v_inverse;
}

VECTEUR utilitaire_vecteur_multiplication_scalaire(VECTEUR vect, double scalaire)
{
	VECTEUR v_final;
	v_final.x = vect.x*scalaire;
	v_final.y = vect.y*scalaire;
	
	return v_final;
}

int utilitaire_is_segment_null(SEGMENT s)
{
	if(	s.deb.x == 0 && s.deb.y == 0 && s.fin.x == 0 && s.fin.y == 0)
		return 0;
	return 1;
}

double utilitaire_produit_vectoriel(VECTEUR v1, VECTEUR v2)
{	
	double pv = v1.x*v2.y - v1.y*v2.x;
	
	return pv;
}

double utilitaire_produit_scalaire(VECTEUR v1, VECTEUR v2)
{
	double ps = v1.x*v2.x + v1.y*v2.y;
	
	return ps;
}

int utilitaire_detection_parallelisme(SEGMENT s1, SEGMENT s2)
{
	VECTEUR u1 = utilitaire_calculer_difference_unitaire(s1);
	VECTEUR u2 = utilitaire_calculer_difference_unitaire(s2);
	
	double pv = utilitaire_produit_vectoriel(u1, u2);
	double norme1 = utilitaire_calculate_norme_segment(s1);
	VECTEUR vd1d2 = utilitaire_vecteur_difference(s1.deb, s2.deb);
	VECTEUR ud1d2 = utilitaire_calculer_vecteur_unitaire(vd1d2);
	
	if(fabs(pv) <= EPSIL_PARAL)	
	{
		double pvdeb = utilitaire_produit_vectoriel(u1, ud1d2);
		
		if(fabs(pvdeb)<= EPSIL_PARAL)
		{
			VECTEUR v2 = utilitaire_vecteur_difference(s2.deb,s2.fin);
			double psu1v2 = utilitaire_produit_scalaire(u1, v2);
			double psdeb = utilitaire_produit_scalaire(u1, vd1d2);
			
			if(fabs(psu1v2) < EPSIL_PARAL)	//direction opposée
			{
				if(psdeb > 0)
				{
					if((psdeb + psu1v2)>(norme1 + EPSIL_CONTACT))
						return 0;
				}
				else if(psdeb < -EPSIL_CONTACT)
					return 0;
			}
			else 
			{
				if(psdeb > 0)
				{
					if(psdeb > norme1 + EPSIL_CONTACT)
						return 0;
				}
				else if(psdeb + psu1v2 < -EPSIL_CONTACT)
					return 0;
			}
			return 1;
		}
		else
			return 0;
	}
	else
	{
		return utilitaire_detection_intersection(s1,s2);
	}
}

int utilitaire_detection_intersection(SEGMENT s1, SEGMENT s2)
{
	VECTEUR vd1f2 = utilitaire_vecteur_difference(s1.deb,s2.fin);
	VECTEUR vd2f1 = utilitaire_vecteur_difference(s2.deb, s1.fin);
	VECTEUR vd1d2 = utilitaire_vecteur_difference(s1.deb, s2.deb);
	
	VECTEUR n1 = utilitaire_calculer_difference_normal(s1);
	VECTEUR n2 = utilitaire_calculer_difference_normal(s2);
	
	double ps1d1d2 = utilitaire_produit_scalaire(n1, vd1d2);
	double ps1d1f2 = utilitaire_produit_scalaire(n1, vd1f2);
	double ps2d2d1 = utilitaire_produit_scalaire(utilitaire_calculer_sens_inverse(n2), vd1d2);
	double ps2d2f1 = utilitaire_produit_scalaire(n2, vd2f1);
	
	if((fabs(ps1d1d2) < EPSIL_CONTACT || fabs(ps1d1f2) < EPSIL_CONTACT) || ps1d1d2*ps1d1f2 < 0)
	{
		if((fabs(ps2d2d1) < EPSIL_CONTACT || fabs(ps2d2f1) < EPSIL_CONTACT) || ps2d2d1*ps2d2f1 < 0)
		{
			return 1;
		}
	}
	return 0;
}

VECTEUR utilitaire_detection_point_intersection(SEGMENT s1, SEGMENT s2)
{
	VECTEUR v1 = utilitaire_vecteur_difference(s1.deb, s1.fin);
	VECTEUR v2 = utilitaire_vecteur_difference(s2.deb, s2.fin);
	VECTEUR pt_intersection;
	
	if (v1.x == 0)
	{
		double pente2 = v2.y/v2.x;
		pt_intersection.x = s1.deb.x;
		pt_intersection.y = pente2*(pt_intersection.x-s2.deb.x)+s2.deb.y;
	}
	else if (v2.x == 0)
	{
		double pente1 = v1.y/v1.x;
		pt_intersection.x = s2.deb.x;
		pt_intersection.y = pente1*(pt_intersection.x-s1.deb.x)+s1.deb.y;
	}
	else
	{
		double pente1 = v1.y/v1.x;
		double pente2 = v2.y/v2.x;
		pt_intersection.x =  (pente1*s1.deb.x-pente2*s2.deb.x+s2.deb.y-s1.deb.y)/(pente1-pente2);
		pt_intersection.y = pente1*(pt_intersection.x-s1.deb.x)+s1.deb.y;
	}
	
	return pt_intersection;
} 
	
	
	
VECTEUR utilitaire_new_vecteur(double x, double y){
	VECTEUR v = {x,y};
	return v;
}			
			

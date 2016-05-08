#include "utilitaire.h"
#include "constantes.h"
#include <math.h>

double calculate_distance(VECTEUR vect1, VECTEUR vect2)	
{
	double difx = vect2.x - vect1.x;
	double dify = vect2.y - vect1.y;
	
	double distance = sqrt(dify*dify + difx*difx);
	
	return distance;
}

double calculate_norme_vecteur(VECTEUR v)
{
	double norme = sqrt(v.x*v.x + v.y*v.y);
	
	return norme;
}

double calculate_norme_segment(SEGMENT s)
{
	double norme_x = s.fin.x - s.deb.x;
	double norme_y = s.fin.y - s.deb.y;
	
	double norme = sqrt(norme_y*norme_y + norme_x*norme_x);
	
	return norme;
}

VECTEUR vecteur_difference(VECTEUR vect1, VECTEUR vect2)	//on crée le vecteur entre les deux vecteur
{
	VECTEUR new_vect;
	
	new_vect.x = vect2.x - vect1.x;
	new_vect.y = vect2.y - vect1.y;
	
	return new_vect;
}

VECTEUR calculer_difference_unitaire(SEGMENT s)
{
	double norme = calculate_norme_segment(s);
	
	VECTEUR vect = vecteur_difference(s.deb, s.fin);
	VECTEUR vect_unitaire = {vect.x/norme, vect.y/norme};
	
	return vect_unitaire;
}

VECTEUR calculer_vecteur_unitaire(VECTEUR v)
{
	double norme = calculate_norme_vecteur(v);
	
	VECTEUR vect_unitaire = {v.x/norme, v.y/norme};
	
	return vect_unitaire;
}
	
VECTEUR calculer_difference_normal(SEGMENT s)	
{
	VECTEUR vect = vecteur_difference(s.deb, s.fin);
	VECTEUR vect_normal = { -vect.y, vect.x };

	return vect_normal;
}

VECTEUR calculer_sens_inverse(VECTEUR v)
{
	VECTEUR v_inverse = {-v.x, -v.y};
	
	return v_inverse;
}

double produit_vectoriel(VECTEUR v1, VECTEUR v2)
{	
	double pv = v1.x*v2.y - v1.y*v2.x;
	
	return pv;
}

double produit_scalaire(VECTEUR v1, VECTEUR v2)
{
	double ps = v1.x*v2.x + v1.y*v2.y;
	
	return ps;
}

int detection_parallelisme(SEGMENT s1, SEGMENT s2)
{
	VECTEUR u1 = calculer_difference_unitaire(s1);
	VECTEUR u2 = calculer_difference_unitaire(s2);
	
	double pv = produit_vectoriel(u1, u2);
	VECTEUR vd1d2 = vecteur_difference(s1.deb, s2.deb);
	VECTEUR ud1d2 = calculer_vecteur_unitaire(vd1d2);
	
	if(fabs(pv) <= EPSIL_PARAL)	
	{
		double pvdeb = produit_vectoriel(u1, ud1d2);
		
		if(fabs(pvdeb)<= EPSIL_PARAL)
		{
			VECTEUR v2 = vecteur_difference(s2.deb,s2.fin);
			double psu1v2 = produit_scalaire(u1, v2);
			double psdeb = produit_scalaire(u1, vd1d2);
			
			if(psu1v2 < 0)	//direction opposée
			{
				if(psdeb > 0)
				{
					if((psdeb + psu1v2)>(calculate_norme_segment(s1) + EPSIL_CONTACT))
					{
						return 0;
					}
					return 1;
				}
				else 
				{
					if(psdeb < -EPSIL_CONTACT)
					{
						return 0;
					}
					return 1;
				}
				return 1;
			}
			else if(psu1v2 > 0)
			{
				if(psdeb > 0)
				{
					if(psdeb>(calculate_norme_segment(s1) + EPSIL_CONTACT))
					{
						return 0;
					}
					return 1;
				}
				else 
				{
					if((psdeb + psu1v2)<(-EPSIL_CONTACT))
					{
						return 0;
					}
					return 1;
				}
				return 1;
			}
			return 1;
		}
		return 0;
	}
	return 1;
}

int detection_intersection(SEGMENT s1, SEGMENT s2)
{
	VECTEUR vd1f2 = {s2.fin.x - s1.deb.x, s2.fin.y - s1.deb.y};
	VECTEUR vd2f1 = {s1.fin.x - s2.deb.x, s1.fin.y - s2.deb.y};
	VECTEUR vd1d2 = vecteur_difference(s1.deb, s2.deb);
	VECTEUR n1 = calculer_difference_normal(s1);
	VECTEUR n2 = calculer_difference_normal(s2);
	
	double ps1d1d2 = produit_scalaire(n1, vd1d2);
	double ps1d1f2 = produit_scalaire(n1, vd1f2);
	double ps2d2d1 = produit_scalaire(calculer_sens_inverse(n2), vd1d2);
	double ps2d2f1 = produit_scalaire(n2, vd2f1);
	
	if((fabs(ps1d1d2) < EPSIL_CONTACT || fabs(ps1d1f2) < EPSIL_CONTACT) || ps1d1d2*ps1d1f2 < 0)
	{
		if((fabs(ps2d2d1) < EPSIL_CONTACT || fabs(ps2d2f1) < EPSIL_CONTACT) || ps2d2d1*ps2d2f1 < 0)
		{
			//ou effectuer le calcul du point d'intersection
			return 0;
		}
	}
	return 1;
}
	
	

VECTEUR new_vecteur(double x, double y)
{
	VECTEUR new_vecteur =  {x, y};
	
	return new_vecteur;
}			
			
			
			
			
			

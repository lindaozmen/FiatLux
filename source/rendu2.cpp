#include <stdlib.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glui.h>

extern "C"
{
	#include "modele.h"
	#include "graphic.h"
	#include <math.h>
	#include "utilitaire.h"
}

namespace
{
	int main_window;
	int window_width = 400, window_height = 480; //largeur et hauteur de la fenetre
	double centerX = 0, centerY = 0;
	static GLfloat aspect_ratio;
	int dessiner = 0;
	VECTEUR dessin_start, dessin_end;		//pour zoom
	double xmin = -DMAX, xmax = DMAX, ymin = -DMAX, ymax = DMAX;
	int bouton_start = 0;
	int selecting = 0;

	//Fenetre glui
	GLUI *glui;
	/*pointeurs pour créer les objets de GLUI*/
	GLUI_Panel *p_file,*p_sim,*p_inf,*mp,*mpa,*mpe;
	GLUI_EditText *fn1, *fn2;
	GLUI_Button *load,*save,*start, *step;
    GLUI_EditText *edt_ph,*edt_pr, *edt_abs,*edt_ref;
    GLUI_RadioGroup *radio1,*radio2;
    GLUI_Button *exit_button;
    
}

void reshape_cp(int x, int y);
void display_cb();
void control_cb(int id);
void start_graphic(int* argc, char ** argv);
void add_elements_glui();
void mouse_cb(int button, int state, int x, int y);
VECTEUR glut_to_opengl_coordonnees(int x, int y);

void reshape_cb(int w, int h)
{
	if(h == 0) h = 1;
	
	aspect_ratio = (GLfloat) w / (GLfloat) h;
	
	glViewport(0,0,w,h);
	
	//sert pour les intéraction avec la souris
	window_width = w;
	window_height = h;
	
//	glutPostRedisplay();
}

//l'affichage de l'état courant du système
void display_cb()
{	
	glLoadIdentity();
	GLfloat gauche = xmin, droite = xmax, bas = ymin, haut = ymax;
	
	//préparation du fond pour le nouveau dessin
	glClear(GL_COLOR_BUFFER_BIT);
	
	if(aspect_ratio <= 1.0)
		glOrtho(gauche, droite, bas/aspect_ratio, haut/aspect_ratio, -1, 1);
	else
		glOrtho(gauche*aspect_ratio, droite*aspect_ratio, bas, haut, -1, 1);
    
	modele_draw();
	
	if(dessiner == 1)
	{
		modele_dessin_carre_zoom(dessin_start, dessin_end);
	}
	
	glutSwapBuffers();
}

void mouse_cb(int button, int state, int x, int y)
{   
	if(state == GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
		{
			dessiner = 1;
			dessin_start = glut_to_opengl_coordonnees(x, y);
			dessin_end = dessin_start;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			VECTEUR coordonnees = glut_to_opengl_coordonnees(x, y);
			if (radio1->get_int_val() == 0)
			{
				modele_element_plus_proche(coordonnees);
			}
			selecting = 1;
		}
		
   	}
   	else if(state == GLUT_UP)
   	{
		if(button == GLUT_LEFT_BUTTON)
		{
			if(calculate_distance(dessin_start, dessin_end) > EPSIL_CREATION)
			{
				/*
				xmin = dessin_start.x > dessin_end.x ? dessin_end.x : dessin_start.x;
				xmax = dessin_start.x > dessin_end.x ? dessin_start.x : dessin_end.x;
				ymin = dessin_start.y > dessin_end.y ? dessin_end.y : dessin_start.y;
				ymax = dessin_start.y > dessin_end.y ? dessin_start.y : dessin_end.y;
				*/
				if(fabs(dessin_end.x-dessin_start.x) > fabs(dessin_end.y-dessin_start.y))
				{
					xmin = dessin_start.x > dessin_end.x ? dessin_end.x : dessin_start.x;
					xmax = dessin_start.x > dessin_end.x ? dessin_start.x : dessin_end.x;
					double l_y = fabs(dessin_end.x-dessin_start.x)/aspect_ratio;
					ymax = (dessin_end.y+dessin_start.y)/2 + l_y/2;
					ymin = (dessin_end.y+dessin_start.y)/2 - l_y/2;
				}
				else
				{
					double l_x = fabs(dessin_end.y-dessin_start.y)*aspect_ratio;
					xmin = (dessin_end.x+dessin_start.x)/2.0 - l_x/2;
					xmax = (dessin_end.x+dessin_start.x)/2.0 + l_x/2;
					ymin = dessin_start.y > dessin_end.y ? dessin_end.y : dessin_start.y;
					ymax = dessin_start.y > dessin_end.y ? dessin_start.y : dessin_end.y;
				}
			}
			dessiner = 0;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			selecting = 0;
		}
		
	}
	
	glutPostRedisplay();
}

VECTEUR glut_to_opengl_coordonnees(int x, int y)
{
	VECTEUR new_vecteur;
	if(aspect_ratio <= 1.0)
	{
		new_vecteur.x = xmin + x*(fabs(xmax-xmin))/window_width;
		new_vecteur.y = ymax/aspect_ratio - y*(fabs(ymax-ymin))/(window_height*aspect_ratio);
	}
	else
	{
		new_vecteur.x = xmin*aspect_ratio + x*(fabs(xmax-xmin))/window_width*aspect_ratio;
		new_vecteur.y = ymax - y*(fabs(ymax-ymin))/window_height;
	}
	
	return new_vecteur;
}

void motion_cb(int x, int y)
{
	VECTEUR coordonnees = glut_to_opengl_coordonnees(x, y);
	dessin_end = coordonnees;
	
	if(selecting == 1){
			if (radio1->get_int_val() == 0)
			{
				modele_element_plus_proche(coordonnees);
			}
	}
	glutPostRedisplay();
}

void keyboard_cb(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'r':
		{
			xmin = -DMAX;
			xmax = DMAX;
			ymin = -DMAX;
			ymax = DMAX;
			break;
		}
		case 'k':
		{
			modele_destruction_photon_dehors(xmin, xmax, ymin, ymax);
			break;
		}
		case 'd':
		{
			modele_effacer_element();
			break;
		}
	}
	glutPostRedisplay();		
}

void control_cb(int id)
{
	switch(id)
	{
		case 1: 
		{
			modele_destruction_simulation();
			char* load_file = (char*)(fn1->get_text());
			int verif_lecture = modele_lecture(load_file,0);
			
			if(verif_lecture == 1)
			{
				modele_destruction_simulation();
			}
			aspect_ratio = 1.0*window_width/window_height;
			xmin = -DMAX, xmax = DMAX, ymin = -DMAX, ymax = DMAX;
			glutPostRedisplay();	
			break;
		}
		case 2: 
		{
			char* save_file = (char*)(fn2->get_text());
			modele_save_elements(save_file);
		    break;
		}
		case 3:
		{
			if(bouton_start == 0)
			{
				bouton_start = 1;
				start->set_name((char*)"Stop");
			}
			else if(bouton_start == 1)
			{
				bouton_start = 0;
				start->set_name((char*)"Start !");
			}
			break;
		}
		case 4:
		{
			bouton_start = 0;
			start->set_name((char*)"Start !");
			photon_update();
			break;
		}
			
	}
}

/*que la mise à jour du modèle
puis un événement d'affichage pour garantir que display_cb
fasse l'affichage */
void idle_cb()
{
	if(glutGetWindow() != main_window)
		glutSetWindow(main_window);
		
	//Check if one of the radio buttons has changed	
	modele_check_radio(radio1->get_int_val(), radio2->get_int_val());
	if(bouton_start == 1)
	{
		photon_update();
	}
	
	edt_ph->set_int_val(photon_get_nb());
	edt_pr->set_int_val(projecteur_get_nombre());
	edt_abs->set_int_val(absorbeur_get_nombre());
	edt_ref->set_int_val(reflecteur_get_nombre());
	
	glutPostRedisplay();
}

void start_graphic(int* argc, char ** argv)
{
	glutInit(argc,argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowSize(window_width,window_height);
	aspect_ratio = (GLfloat)window_width/(GLfloat)window_height;
	
	main_window=glutCreateWindow("Fiat Lux !");
	
	glClearColor(1.0, 1.0, 1.0, 0.0); //pour un fond blanc;

	glutDisplayFunc(display_cb);
	glutReshapeFunc(reshape_cb);	
	glutMouseFunc(mouse_cb);
	glutMotionFunc(motion_cb);
	glutKeyboardFunc(keyboard_cb);
	glutIdleFunc(idle_cb);
	
	glui=GLUI_Master.create_glui("Fiat Lux !",0,600,100);
	
	add_elements_glui();
	
	if(*argc > 2){
		fn1->set_text(argv[2]);
	}
	glui->set_main_gfx_window( main_window );
}	

void add_elements_glui(){
	//pour le premier panneau File
	p_file=glui->add_panel("File");
	fn1=glui->add_edittext_to_panel(p_file,"Filename:"); 
	fn1->set_text((char*)"test.txt");
	load=glui->add_button_to_panel(p_file,"Load",1,control_cb);
	fn2=glui->add_edittext_to_panel(p_file,"Filename:"); 
	fn2->set_text((char*)"save.txt");
	save=glui->add_button_to_panel(p_file,"Save",2,control_cb);
	
	//pour le second panneau Simulation
	p_sim=glui->add_panel("Simulation");
	start=glui->add_button_to_panel(p_sim,"Start !",3,control_cb);
	step=glui->add_button_to_panel(p_sim,"Step",4,control_cb);

	//pour le bouton quitter
	exit_button = glui->add_button("Exit",0,(GLUI_Update_CB)exit);

	//dessin de la colonne séparatrice
	glui->add_column(true);

	//pour le panneau Information
	p_inf=glui->add_panel("Information");
	edt_ph=glui->add_edittext_to_panel(p_inf,"Nb Photons",GLUI_SPINNER_INT);
	edt_ph->set_int_val(photon_get_nb());
	edt_pr=glui->add_edittext_to_panel(p_inf,"Nb Projecteurs",GLUI_SPINNER_INT);
	edt_pr->set_int_val(projecteur_get_nombre());
	edt_abs=glui->add_edittext_to_panel(p_inf,"Nb Absorbeurs",GLUI_SPINNER_INT);
	edt_abs->set_int_val(absorbeur_get_nombre());
	edt_ref=glui->add_edittext_to_panel(p_inf,"Nb Reflecteurs",GLUI_SPINNER_INT);
	edt_ref->set_int_val(reflecteur_get_nombre());

	//pour le panneau Mouse Panel
	mp=glui->add_panel("Mouse panel");

	mpa=glui->add_panel_to_panel(mp,"Action selection");
	radio1=glui->add_radiogroup_to_panel(mpa);  
	glui->add_radiobutton_to_group(radio1,"Selection");
	glui->add_radiobutton_to_group(radio1,"Creation");

	mpe=glui->add_panel_to_panel(mp,"Entity selection");
	radio2=glui->add_radiogroup_to_panel(mpe);
	glui->add_radiobutton_to_group(radio2,"Projecteur");
	glui->add_radiobutton_to_group(radio2,"Reflecteur");

	glui->add_radiobutton_to_group(radio2,"Absorbeur");

	glui->set_main_gfx_window(main_window);
}

int main(int argc, char * argv[])
{
	char* mode_test;
	if(argc > 1)
		mode_test = argv[1];
	
	if((mode_test != NULL) && (strncmp(mode_test, "Error", 5) == 0))
	{
		//pour ne pas faire le dessin
		
		if(argc > 2){
			int test_dest = modele_lecture(argv[2],0);
			if(test_dest == 1)
			{
				modele_destruction_simulation();
			}
		}
	}
	else if((mode_test != NULL)  && strncmp(mode_test, "Verification", 12) == 0)
	{
		if(argc > 2){
			int test_dest = modele_lecture(argv[2],1);
			if(test_dest == 1)
			{
				modele_destruction_simulation();
			}
		}
		modele_lecture_rendu2();
	}
	else 
	{
		if(argc > 2){
			int test_dest = modele_lecture(argv[2],0);
			if(test_dest == 1)
			{
				modele_destruction_simulation();
			}
		}
		
		start_graphic(&argc, argv);
		glutMainLoop();
	}
	modele_destruction_simulation();
	return EXIT_SUCCESS;
}



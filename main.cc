#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "common.hh"
#include "sydan_filled.hh"
#include "hiutale.hh"
#include "snow.hh"
#include "sydan_parts.hh"
#include "tausta.hh"

static float aika=0.0;

static GLfloat l0_pos[] = {0.1,1.1,1.0,1.0};
//static GLfloat l0_col[] = { 1.0, 1.0, 1.0, 1.0};
//static GLfloat l1_pos[] = {0.0,-2.0,-10.0,1.0};
//static GLfloat l1_col[] = { 0.5, 0.5, 0.5, 1.0};
  

int loadGLTextures()
{
    int status;
    textureImage *texti;

    status = 0;

    texti = static_cast<textureImage*> (malloc(sizeof(textureImage)));
    if (loadBMP("kuvat/flare.bmp", texti))
    {
        status = 1;
        glGenTextures(1, &textures[0]);
        glBindTexture(GL_TEXTURE_2D, textures[0]);

        glTexImage2D(GL_TEXTURE_2D, 0, 3, texti->width, texti->height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, texti->data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    if (texti)
    {
        if (texti->data)
            free(texti->data);
        free(texti);
	texti=NULL;
    }

    texti = static_cast<textureImage*> (malloc(sizeof(textureImage)));
    if (loadBMP("kuvat/flake.bmp", texti))
    {
        status = 1;
        glGenTextures(1, &textures[1]);
        glBindTexture(GL_TEXTURE_2D, textures[1]);

        glTexImage2D(GL_TEXTURE_2D, 0, 3, texti->width, texti->height, 0,
            GL_RGB, GL_UNSIGNED_BYTE, texti->data);

	        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_LINEAR);
    }

    if (texti)
    {
        if (texti->data)
            free(texti->data);
        free(texti);
    }
    return status;
}


static void redraw(void)
{
	timespec viive={0,10000000};

	aika += 0.03;
	//        glLightfv(GL_LIGHT0, GL_POSITION, l0_pos);
	glLightfv(GL_LIGHT0, GL_POSITION, l0_pos);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	
	glPushMatrix();

	glRotatef(8.0*sin(aika*0.6),0.2,1.0,0.0);

        glTranslatef(0,0,-30+5.0*cos(aika*0.5));
	draw_snow();


	  //          glRotated((GLdouble) aika, 1.0, 0.0, 0.0);



	draw_sydan_filled();

	draw_particles(aika);
	
	drawTausta();

	glPopMatrix();
	
	glutSwapBuffers();
	glutPostRedisplay();
	nanosleep(&viive, NULL);
}


int main(int argc, char **argv) 
{



  glutInit(&argc,argv);
  
  
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

  glutCreateWindow("Katjalle");
  //glutGameModeString( "800x600" );
  //glutEnterGameMode();
  
  initialize();  
  generate_flake();
  init_flakes();

  if (!loadGLTextures())
    {
      return 0;
    }
  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  //	glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  //glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  
  //  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);
  //  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_MULTISAMPLE_ARB);
  //  glEnable(GL_LIGHT1);
  //  glLightfv(GL_LIGHT0, GL_DIFFUSE , l0_col);
  //glLightfv(GL_LIGHT1, GL_DIFFUSE , l1_col);
  glutDisplayFunc(redraw);	
  generate_sydan_filled();
  init_particles();
  init_tausta();
  
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45, //view angle
		 1.0,	//aspect ratio
		 1.0, //near clip
		 2000.0);//far clip
  glMatrixMode(GL_MODELVIEW);
  
  //  glDisable(GL_CULL_FACE);
  
  
  //	initParts();
  //init_sdn();
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glutMainLoop();
  
  return 0; 
}



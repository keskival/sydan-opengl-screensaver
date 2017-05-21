#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>

#include "common.hh"
#include "sydan.hh"

#define NUMPARTS 21
#define PI 3.1415926535897932384626


/*
typedef struct {
    int width;
    int height;
    unsigned char *data;
} textureImage;
*/


struct Sydan_Particles {
  struct {
    float pos[3];
    float col[4];
  } ver[NUMPARTS];
} part_sdn;


//struct Particle part_sdn;

GLuint part_texture;

static int loadGLTextures()
{
    int status;
    textureImage *texti;

    status = 0;

    texti = static_cast<textureImage*> (malloc(sizeof(textureImage)));
    if (loadBMP("kuvat/flare.bmp", texti))
    {
        status = 1;
        glGenTextures(1, &part_texture);
        glBindTexture(GL_TEXTURE_2D, part_texture);

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



    return status;
}


void setParts(float ta) {
  int i=0;
  float t=ta;
  for (i=0; i<NUMPARTS; i++) {
    sydanfunc(t,part_sdn.ver[i].pos,1.5);
    part_sdn.ver[i].pos[1]+=0.6;
    t+=1.0/NUMPARTS;
    if (t>=1.0) t-=1.0;
  }
}

void init_particles(void) {
  loadGLTextures();
  setParts(0);
  for (int i=0; i<NUMPARTS; i++) {
    part_sdn.ver[i].col[0]=0.20;
    part_sdn.ver[i].col[1]=0.20;
    part_sdn.ver[i].col[2]=0.99;
    part_sdn.ver[i].col[3]=0.8;
  }
}

void draw_particles(float t)
{
  float x,y,z;
  float right[3], up[3];
  float modelview[16];
  int i;
  
  glPushMatrix();

  setParts((float)((int)(t*100) % 1000)/1000.0);
  glRotatef(190.0*cos(t),0.0,1.0,0.0);
  
  glDisable(GL_LIGHTING);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, part_texture);


  glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
  right[0] = modelview[0]/2.0;
  right[1] = modelview[4]/2.0;
  right[2] = modelview[8]/2.0;
  up[0] = modelview[1]/2.0;
  up[1] = modelview[5]/2.0;
  up[2] = modelview[9]/2.0;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glDisable(GL_DEPTH_TEST);
  for (i=0; i<NUMPARTS; i++) {
    x=part_sdn.ver[i].pos[0];
    y=part_sdn.ver[i].pos[1];
    z=part_sdn.ver[i].pos[2];

    /*
    glGetFloatv(GL_MODELVIEW_MATRIX , modelview);
    
    // undo all rotations
    // beware all scaling is lost as well 
    for( j=0; j<3; j++ ) 
      for( k=0; k<3; k++ ) {
	if ( j==k )
	  modelview[j*4+k] = 1.0;
	else
	  modelview[j*4+k] = 0.0;
      }

    // set the modelview with no rotations and scaling
    glLoadMatrixf(modelview);
    */
    
    glBegin(GL_TRIANGLE_STRIP);
    glColor4fv(part_sdn.ver[ i ].col);
    glTexCoord2d(1, 1);
    glVertex3f(x + right[0]+up[0], y + right[1]+up[1], z + right[2]+up[2]);
    glTexCoord2d(0, 1);
    glVertex3f(x -right[0]+up[0], y-right[1]+up[1], z-right[2]+up[2]);
    glTexCoord2d(1, 0);
    glVertex3f(x + right[0]-up[0], y +right[1]-up[1], z +right[2]-up[2]);
    glTexCoord2d(0, 0);
    glVertex3f(x - right[0]-up[0], y - right[1]-up[1], z -right[2]-up[2]);
    glEnd();
  }
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
  glPopMatrix();
}

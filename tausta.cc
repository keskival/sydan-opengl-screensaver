#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include "common.hh"

#define PI 3.1415926535897932384626

GLuint tausta_texture;

void drawTausta(void) {

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_COLOR,GL_ONE);
  glPushMatrix();
  glLoadIdentity();
  glBlendFunc(GL_ONE, GL_ONE);
  glBindTexture(GL_TEXTURE_2D, tausta_texture);
  glEnable(GL_TEXTURE_2D);
  glColor3f(1.0f,1.0f,1.0f);
  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex3f(-1.1f, -1.1f,  -2.6f);
  glTexCoord2f(1.0f, 0.0f);
  glVertex3f( 1.1f, -1.1f,  -2.6f);
  glTexCoord2f(1.0f, 1.0f);
  glVertex3f( 1.1f,  1.1f,  -2.6f);
  glTexCoord2f(0.0f, 1.0f);
  glVertex3f(-1.1f,  1.1f,  -2.6f);
  glDisable(GL_TEXTURE_2D);
  glEnd();
  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
}

int loadTaustaTexture()
{
    int status;
    textureImage *texti;

    status = 0;

    texti = static_cast<textureImage*> (malloc(sizeof(textureImage)));
    if (loadBMP("kuvat/tausta.bmp", texti))
    {
        status = 1;
        glGenTextures(1, &tausta_texture);
        glBindTexture(GL_TEXTURE_2D, tausta_texture);

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

void init_tausta(void) {
  loadTaustaTexture();
}

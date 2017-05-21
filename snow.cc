#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include "common.hh"
#include "hiutale.hh"
#include "snow.hh"

struct Flakes part_flakes;

void regen_flake(int i, int anywhere) {

    part_flakes.ver[i].pos[0] = (random()%10000)/10000.0*100.0-50.0;
    if (anywhere) part_flakes.ver[i].pos[1] = (random()%10000)/10000.0*(ALARAJA-YLARAJA)+(YLARAJA);
    else part_flakes.ver[i].pos[1] = YLARAJA;
    //    part_flakes.ver[i].pos[2] = 0.0;
    part_flakes.ver[i].pos[2] = -(random()%10000)/10000.0*80.0+20.0;
    part_flakes.ver[i].t = (random()%10000)/10000.0*2*PI;
}

void init_flakes(void) {
  for (int i=0; i<NUMFLAKES; i++) {
    regen_flake(i, 1);
  }
}

void update_flake(float* pos) {
  //pos[0] = pos[0];
  pos[0] = pos[0]+sin(pos[1]*1.0+((random()%10000)/10000.0*0.5-0.25))*0.1;
  pos[1] = pos[1]-0.1;
}

void update_flakes(void) {
  for (int i=0; i<NUMFLAKES; i++) {
    update_flake(part_flakes.ver[i].pos);
    if(part_flakes.ver[i].pos[1]<ALARAJA) {
      regen_flake(i, 0);
    }
   
  }
}

void draw_snow(void)
{
  float x,y,z;
  int detail=0;
  update_flakes();
  glEnable(GL_DEPTH_TEST);
  
  for (int i=0; i<NUMFLAKES; i++) {
    part_flakes.ver[i].t+=0.01;
    glPushMatrix();
    x=part_flakes.ver[i].pos[0];
    y=part_flakes.ver[i].pos[1];
    z=part_flakes.ver[i].pos[2];
    glTranslatef(x,y,z);
    glRotatef(800.0*sin(part_flakes.ver[i].t),0.0,1.0,0.0);
    glRotatef(600.0*sin(part_flakes.ver[i].t*1.5),0.0,0.0,1.0);
    glRotatef(400.0*sin(part_flakes.ver[i].t*1.3),1.0,0.0,0.0);
    if (z>-20.0) {
      detail=2;
    } else if (z>-40.0) {
      glDisable(GL_LIGHTING);
      detail=1;
    } else {
      glDisable(GL_LIGHTING);
      detail=0;
    }

    draw_flake(detail);
    glEnable(GL_LIGHTING);
    glPopMatrix();
  }
  glDisable(GL_DEPTH_TEST);
  
}

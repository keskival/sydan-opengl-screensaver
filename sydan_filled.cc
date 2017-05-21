#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "common.hh"
#include "sydan.hh"
#include "sydan_filled.hh"

#define NUMSDNVERTS 24

struct Object sydan_filled;

static GLfloat heart_mat_col[]={0.8,0.05,0.05,1.0};
static GLfloat heart_mat_spec[] = { 0.7, 0.7, 0.7, 1.0 };
static GLfloat heart_mat_shin[] = { 7.0 };

static float aika_sydan_filled=0.0;

void generate_sydan_filled(void) {
  Triangle * iterator = NULL;
  sydan_filled.listind=-1;

  sydan_filled.first_part=NULL;
  sydan_filled.first_quad=NULL;
  sydan_filled.first_ctriag=NULL;
  sydan_filled.num_vertices=NUMSDNVERTS;
  sydan_filled.vertices = (struct Vertex*) malloc(NUMSDNVERTS * sizeof(struct Vertex));
  iterator = (struct Triangle*) malloc(sizeof(struct Triangle));
  sydan_filled.first_triag = iterator;
  iterator->texture=-1;
  memcpy(iterator->matColor, heart_mat_col, sizeof(heart_mat_col));
  memcpy(iterator->matSpecular, heart_mat_spec, sizeof(heart_mat_spec));
  memcpy(iterator->matShininess, heart_mat_shin, sizeof(heart_mat_shin));


  iterator->next=NULL;
  for (int i=1; i<(NUMSDNVERTS-1); i++) {
    iterator->next = (struct Triangle*) malloc(sizeof(struct Triangle));
    iterator->texture=-1;
    memcpy(iterator->matColor, heart_mat_col, sizeof(heart_mat_col));
    memcpy(iterator->matSpecular, heart_mat_spec, sizeof(heart_mat_spec));
    memcpy(iterator->matShininess, heart_mat_shin, sizeof(heart_mat_shin));
    iterator=iterator->next;
  }
  iterator->texture=-1;
  memcpy(iterator->matColor, heart_mat_col, sizeof(heart_mat_col));
  memcpy(iterator->matSpecular, heart_mat_spec, sizeof(heart_mat_spec));
  memcpy(iterator->matShininess, heart_mat_shin, sizeof(heart_mat_shin));
  iterator->next=NULL;

  sydan_filled.vertices[0].pos[0]=0.0;
  sydan_filled.vertices[0].pos[1]=0.0;
  sydan_filled.vertices[0].pos[2]=0.0;

  sydan_filled.vertices[0].col[0]=0.8;
  sydan_filled.vertices[0].col[1]=0.0;
  sydan_filled.vertices[0].col[2]=0.0;
  sydan_filled.vertices[0].col[3]=1.0;

  sydanfunc(0.5, sydan_filled.vertices[1].pos, 1.0);
  sydan_filled.vertices[1].col[0]=0.8;
  sydan_filled.vertices[1].col[1]=0.0;
  sydan_filled.vertices[1].col[2]=0.0;
  sydan_filled.vertices[1].col[3]=1.0;
  sydanfunc(0.625,sydan_filled.vertices[2].pos, 1.0); 
  sydan_filled.vertices[2].col[0]=0.8;
  sydan_filled.vertices[2].col[1]=0.0;
  sydan_filled.vertices[2].col[2]=0.0;
  sydan_filled.vertices[2].col[3]=1.0;
  sydanfunc(0.375,sydan_filled.vertices[3].pos, 1.0);
  sydan_filled.vertices[3].col[0]=0.8;
  sydan_filled.vertices[3].col[1]=0.0;
  sydan_filled.vertices[3].col[2]=0.0;
  sydan_filled.vertices[3].col[3]=1.0;

  iterator=sydan_filled.first_triag;
  iterator->ver[0]=1;
  iterator->ver[1]=2;
  iterator->ver[2]=0;
  iterator->texture=-1;

  iterator = iterator->next;

  iterator->ver[0]=3;
  iterator->ver[1]=1;
  iterator->ver[2]=0;
  iterator->texture=-1;
  iterator=iterator->next;

  for (int i=4; i<NUMSDNVERTS; i++) {
    float t=0.375-0.75*((float)i-3.0)/((float)NUMSDNVERTS-3.0);
    if (t<0.0) t+=1.0;
    if (t>=1.0) t-=1.0;
    sydanfunc(t, sydan_filled.vertices[i].pos, 1.0);
    sydan_filled.vertices[i].col[0]=0.8;
    sydan_filled.vertices[i].col[1]=0.0;
    sydan_filled.vertices[i].col[2]=0.0;
    sydan_filled.vertices[i].col[3]=1.0;

    iterator->ver[0]=0;
    iterator->ver[1]=i;
    iterator->ver[2]=i-1;

    iterator=iterator->next;
  }
  iterator->ver[0]=0;
  iterator->ver[1]=2;
  iterator->ver[2]=NUMSDNVERTS-1;
  genListObject(&sydan_filled);
}

void draw_sydan_filled(void)
{
  aika_sydan_filled+=0.01;
  //  aika_sydan_filled=0;

  glDisable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glPushMatrix();
  glRotatef(200.0*cos(aika_sydan_filled),0.0,1.0,0.0);
  drawObject(&sydan_filled);
  glPopMatrix();
  glDisable(GL_DEPTH_TEST);
}

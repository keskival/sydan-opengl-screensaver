#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include "common.hh"
#include "hiutale.hh"

Object sflake[3];

static GLfloat flake_mat_col[]={0.6,0.6,0.6,1.0};
static GLfloat flake_mat_spec[] = { 0.98, 0.98, 0.98, 1.0 };
static GLfloat flake_mat_shin[] = { 9.0 };

struct CTriangle * iterate_sflake(struct Vertex alku, struct Vertex loppu,
				  struct CTriangle* next, int kierros) {
  struct Vertex ver0, ver1, ver2;

  kierros--;
  next->next = (struct CTriangle*)malloc(sizeof (struct CTriangle));
  next=next->next;

  next->texture=-1;

  memcpy(next->matColor, flake_mat_col, sizeof(flake_mat_col));
  memcpy(next->matSpecular, flake_mat_spec, sizeof(flake_mat_spec));
  memcpy(next->matShininess, flake_mat_shin, sizeof(flake_mat_shin));


  next->ver[1].pos[0]=alku.pos[0]+(loppu.pos[0]-alku.pos[0])*0.333;
  next->ver[1].pos[1]=alku.pos[1]+(loppu.pos[1]-alku.pos[1])*0.333;
  next->ver[1].pos[2]=0.0;

  next->ver[0].pos[0]=alku.pos[0]+(loppu.pos[0]-alku.pos[0])*0.5-
    (loppu.pos[1]-alku.pos[1])*sqrt(1.0/12.0);
  next->ver[0].pos[1]=alku.pos[1]+(loppu.pos[1]-alku.pos[1])*0.5+
    (loppu.pos[0]-alku.pos[0])*sqrt(1.0/12.0);
  next->ver[0].pos[2]=0.0;

  next->ver[2].pos[0]=alku.pos[0]+(loppu.pos[0]-alku.pos[0])*0.666;
  next->ver[2].pos[1]=alku.pos[1]+(loppu.pos[1]-alku.pos[1])*0.666;
  next->ver[2].pos[2]=0.0;

  ver0 = next->ver[0];
  ver1 = next->ver[1];
  ver2 = next->ver[2];

  next->ver[0].col[0]=1.0;
  next->ver[0].col[1]=1.0;
  next->ver[0].col[2]=1.0;
  next->ver[0].col[3]=1.0;

  next->ver[1].col[0]=1.0;
  next->ver[1].col[1]=1.0;
  next->ver[1].col[2]=1.0;
  next->ver[1].col[3]=1.0;

  next->ver[2].col[0]=1.0;
  next->ver[2].col[1]=1.0;
  next->ver[2].col[2]=1.0;
  next->ver[2].col[3]=1.0;

  if (kierros>0) {
    next = iterate_sflake(ver1, ver0, next, kierros);
    next = iterate_sflake(ver0, ver2, next, kierros);
    next = iterate_sflake(alku, ver1, next, kierros);
    next = iterate_sflake(ver2, loppu, next, kierros);
  }
  return next;
}


void generate_flake_a(Object* sflake, int detail) {
  struct CTriangle* next;
  
  sflake->listind=-1;

  sflake->first_quad = NULL;
  sflake->first_part = NULL;
  sflake->first_triag = NULL;

  sflake->first_ctriag=(struct CTriangle*) malloc(sizeof (struct CTriangle));
  sflake->first_ctriag->ver[1].pos[0]=0.0;
  sflake->first_ctriag->ver[1].pos[1]=1.0;
  sflake->first_ctriag->ver[1].pos[2]=0.0;
  sflake->first_ctriag->ver[0].pos[0]=0.7;
  sflake->first_ctriag->ver[0].pos[1]=-0.7;
  sflake->first_ctriag->ver[0].pos[2]=0.0;
  sflake->first_ctriag->ver[2].pos[0]=-0.7;
  sflake->first_ctriag->ver[2].pos[1]=-0.7;
  sflake->first_ctriag->ver[2].pos[2]=0.0;

  sflake->first_ctriag->ver[0].col[0]=1.0;
  sflake->first_ctriag->ver[0].col[1]=1.0;
  sflake->first_ctriag->ver[0].col[2]=1.0;
  sflake->first_ctriag->ver[0].col[3]=1.0;

  sflake->first_ctriag->ver[1].col[0]=1.0;
  sflake->first_ctriag->ver[1].col[1]=1.0;
  sflake->first_ctriag->ver[1].col[2]=1.0;
  sflake->first_ctriag->ver[1].col[3]=1.0;

  sflake->first_ctriag->ver[2].col[0]=1.0;
  sflake->first_ctriag->ver[2].col[1]=1.0;
  sflake->first_ctriag->ver[2].col[2]=1.0;
  sflake->first_ctriag->ver[2].col[3]=1.0;


  sflake->first_ctriag->texture=-1;
  next = sflake->first_ctriag;

  memcpy(next->matColor, flake_mat_col, sizeof(flake_mat_col));
  memcpy(next->matSpecular, flake_mat_spec, sizeof(flake_mat_spec));
  memcpy(next->matShininess, flake_mat_shin, sizeof(flake_mat_shin));

  next=iterate_sflake(sflake->first_ctriag->ver[1],
		      sflake->first_ctriag->ver[0],next,detail);
  next=iterate_sflake(sflake->first_ctriag->ver[0],
		      sflake->first_ctriag->ver[2],next,detail);
  next=iterate_sflake(sflake->first_ctriag->ver[2],
		      sflake->first_ctriag->ver[1],next,detail);
  next->next=NULL;
  genListObject(sflake);
}

void generate_flake(void) {
  generate_flake_a(&sflake[0],0);
  generate_flake_a(&sflake[1],1);
  generate_flake_a(&sflake[2],2);
}

void draw_flake(int detail) {
  glPushMatrix();
  glScalef(0.2,0.2,0.2);
  drawObject(&sflake[detail]);
  glPopMatrix();
}

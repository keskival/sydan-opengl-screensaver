#ifndef SNOW_HH
#define SNOW_HH

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

#define NUMFLAKES 700

#define ALARAJA -70.0
#define YLARAJA 70.0

struct Flakes {
  struct {
    float pos[3];
    float t;
  } ver[NUMFLAKES];
};

void regen_flake(int i, int anywhere);

void init_flakes(void);

void update_flake(float* pos);

void update_flakes(void);

void draw_snow(void);

#endif

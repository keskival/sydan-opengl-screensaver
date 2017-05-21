#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>

#define NUMPARTS 11
#define PI 3.1415926535897932384626

void init_particles(void);

void draw_particles(float t);

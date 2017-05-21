#ifndef COMMON_HH
#define COMMON_HH

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>

#define PI 3.1415926535897932384626
#define MAXTEXTURES 10
#define MAXLISTS 10

extern GLuint lists;

extern GLuint textures[MAXTEXTURES];

typedef struct {
  int width;
  int height;
  unsigned char* data;
} textureImage;

struct Vertex {
  float pos[3];
  float col[4];
};

struct Triangle {
  GLfloat matSpecular[4];
  GLfloat matShininess[1];
  GLfloat matColor[4];
  int ver[3];
  int texture; // -1 = no texture
  struct Triangle * next;
};

struct CTriangle {
  GLfloat matSpecular[4];
  GLfloat matShininess[1];
  GLfloat matColor[4];
  struct Vertex ver[3];
  int texture; // -1 = no texture
  struct CTriangle * next;
};

struct Quad {
  int ver[4];
  int texture; // -1 = no texture
  struct Quad * next;
};

struct Particle {
  int ver;
  struct Particle * next;
};

struct Object {
  struct Vertex * vertices; // Taulukko
  int num_vertices;
  int listind;
  
  struct Triangle * first_triag;
  struct CTriangle * first_ctriag;
  struct Quad * first_quad;
  struct Particle * first_part;
};


void initialize(void);

/* simple loader for 24bit bitmaps (data is in rgb-format) */
int loadBMP(char *filename, textureImage *texture);

void genListObject(struct Object * object);
void drawObject(struct Object * object);

#endif

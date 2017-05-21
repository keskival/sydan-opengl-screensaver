#include <stdio.h>
#include <stdlib.h>
#include "common.hh"

GLuint textures[MAXTEXTURES];

GLuint lists;
static int listlast=0;

void initialize(void) {
  lists=glGenLists(MAXLISTS);
}

/* simple loader for 24bit bitmaps (data is in rgb-format) */
int loadBMP(char *filename, textureImage *texture)
{
    FILE *file;
    unsigned short int bfType;
    long int bfOffBits;
    short int biPlanes;
    short int biBitCount;
    long int biSizeImage;
    int i;
    unsigned char temp;
    /* make sure the file is there and open it read-only (binary) */
    if ((file = fopen(filename, "rb")) == NULL)
    {
        printf("File not found : %s\n", filename);
        return 0;
    }
    if(!fread(&bfType, sizeof(short int), 1, file))
    {
        printf("Error reading file!\n");
        return 0;
    }
    /* check if file is a bitmap */
    if (bfType != 19778)
    {
        printf("Not a Bitmap-File!\n");
        return 0;
    }
    /* get the file size */
    /* skip file size and reserved fields of bitmap file header */
    fseek(file, 8, SEEK_CUR);
    /* get the position of the actual bitmap data */
    if (!fread(&bfOffBits, 4, 1, file))
    {
        printf("Error reading file!\n");
        return 0;
    }
    printf("Data at Offset: %ld\n", bfOffBits);
    /* skip size of bitmap info header */
    fseek(file, 4, SEEK_CUR);
    /* get the width of the bitmap */
    fread(&texture->width, sizeof(int), 1, file);
    printf("Width of Bitmap: %d\n", texture->width);
    /* get the height of the bitmap */
    fread(&texture->height, sizeof(int), 1, file);
    printf("Height of Bitmap: %d\n", texture->height);
    /* get the number of planes (must be set to 1) */
    fread(&biPlanes, sizeof(short int), 1, file);
    if (biPlanes != 1)
    {
        printf("Error: number of Planes not 1!\n");
        return 0;
    }
    /* get the number of bits per pixel */
    if (!fread(&biBitCount, sizeof(short int), 1, file))
    {
        printf("Error reading file!\n");
        return 0;
    }
    printf("Bits per Pixel: %d\n", biBitCount);
    if (biBitCount != 24)
    {
        printf("Bits per Pixel not 24\n");
        return 0;
    }
    /* calculate the size of the image in bytes */
    biSizeImage = texture->width * texture->height * 3;
    printf("Size of the image data: %ld\n", biSizeImage);
    texture->data = static_cast<unsigned char*> (malloc(biSizeImage));
    /* seek to the actual data */
    fseek(file, bfOffBits, SEEK_SET);
    if (!fread(texture->data, biSizeImage, 1, file))
    {
        printf("Error loading file!\n");
        return 0;
    }
    /* swap red and blue (bgr -> rgb) */
    for (i = 0; i < biSizeImage; i += 3)
    {
        temp = texture->data[i];
        texture->data[i] = texture->data[i + 2];
        texture->data[i + 2] = temp;
    }
    return 1;
}

void genListObject(struct Object * object) {
  glNewList(lists+listlast,GL_COMPILE);
  drawObject(object);
  glEndList();
  object->listind=listlast;
  listlast++;
}

void drawObject(struct Object * object) {

  if (object != NULL) {
    struct Vertex * vrs = object->vertices;

    if (object->listind!=-1) {
      glCallList(lists+object->listind);
    } else {
      struct Triangle * triag = object->first_triag;
      struct CTriangle * ctriag = object->first_ctriag;
      struct Quad * quad = object->first_quad;
      struct Particle * part = object->first_part;
      int i=0;
      glBegin(GL_TRIANGLES);
      while (triag != NULL) {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, 
		     triag->matColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, 
		     triag->matSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, 
		     triag->matShininess);

	if (triag->texture != -1) {
	  glEnable(GL_TEXTURE_2D);
	  glBindTexture(GL_TEXTURE_2D, textures[triag->texture]);
	}
	
	glColor4fv(vrs[triag->ver[0]].col);
	if (triag->texture != -1) glTexCoord2d(1, 1);
	glVertex3f(vrs[triag->ver[0]].pos[0],
		   vrs[triag->ver[0]].pos[1],
		   vrs[triag->ver[0]].pos[2]);
	
	glColor4fv(vrs[triag->ver[1]].col);
	if (triag->texture != -1) glTexCoord2d(0, 1);
	glVertex3f(vrs[triag->ver[1]].pos[0],
		   vrs[triag->ver[1]].pos[1],
		   vrs[triag->ver[1]].pos[2]);
	
	glColor4fv(vrs[triag->ver[2]].col);
	if (triag->texture != -1) glTexCoord2d(1, 0);
	glVertex3f(vrs[triag->ver[2]].pos[0],
		   vrs[triag->ver[2]].pos[1],
		   vrs[triag->ver[2]].pos[2]);

	triag = triag->next;
      }
      //      glEnd();
      
      //      glBegin(GL_TRIANGLES);
      while (ctriag != NULL) {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, 
		     ctriag->matColor);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, 
		     ctriag->matSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, 
		     ctriag->matShininess);

	if (ctriag->texture != -1) {
	  glEnable(GL_TEXTURE_2D);
	  glBindTexture(GL_TEXTURE_2D, textures[ctriag->texture]);
	}
	
	glColor4fv(ctriag->ver[0].col);
	if (ctriag->texture != -1) glTexCoord2d(1, 1);
	glVertex3f(ctriag->ver[0].pos[0],
		   ctriag->ver[0].pos[1],
		   ctriag->ver[0].pos[2]);
	
	glColor4fv(ctriag->ver[1].col);
	if (ctriag->texture != -1) glTexCoord2d(0, 1);
	glVertex3f(ctriag->ver[1].pos[0],
		   ctriag->ver[1].pos[1],
		   ctriag->ver[1].pos[2]);
	
	glColor4fv(ctriag->ver[2].col);
	if (ctriag->texture != -1) glTexCoord2d(1, 0);
	glVertex3f(ctriag->ver[2].pos[0],
		   ctriag->ver[2].pos[1],
		   ctriag->ver[2].pos[2]);

	ctriag = ctriag->next;
      }
      glEnd();

      while (quad != NULL) {
	if (quad->texture != -1) {
	  glEnable(GL_TEXTURE_2D);
	  glBindTexture(GL_TEXTURE_2D, textures[quad->texture]);
	} else {
	  glDisable(GL_TEXTURE_2D);
	}
	glBegin(GL_TRIANGLE_STRIP);
	
	glColor4fv(vrs[quad->ver[0]].col);
	if (quad->texture != -1) glTexCoord2d(1, 1);
	glVertex3f(vrs[quad->ver[0]].pos[0],
		   vrs[quad->ver[0]].pos[1],
		   vrs[quad->ver[0]].pos[2]);
	
	glColor4fv(vrs[quad->ver[1]].col);
	if (quad->texture != -1) glTexCoord2d(0, 1);
	glVertex3f(vrs[quad->ver[1]].pos[0],
		   vrs[quad->ver[1]].pos[1],
		   vrs[quad->ver[1]].pos[2]);
	
	glColor4fv(vrs[quad->ver[2]].col);
	if (quad->texture != -1) glTexCoord2d(1, 0);
	glVertex3f(vrs[quad->ver[2]].pos[0],
		   vrs[quad->ver[2]].pos[1],
		   vrs[quad->ver[2]].pos[2]);
	
	glColor4fv(vrs[quad->ver[3]].col);
	if (quad->texture != -1) glTexCoord2d(0, 0);
	glVertex3f(vrs[quad->ver[3]].pos[0],
		   vrs[quad->ver[3]].pos[1],
		   vrs[quad->ver[3]].pos[2]);
	glEnd();
	
	quad = quad->next;
      }
      glBegin(GL_POINTS);
      while (part != NULL) {
	glDisable(GL_TEXTURE_2D);
	
	glColor4fv(vrs[part->ver].col);
	glVertex3f(vrs[part->ver].pos[0],
		   vrs[part->ver].pos[1],
		   vrs[part->ver].pos[2]);
	
	part = part->next;
      }
      
      glEnd();
    }
  }
}

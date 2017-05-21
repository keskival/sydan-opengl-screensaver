#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glext.h>

#define NUMFLAKES 150
#define NUMPARTS 11
#define NUMSDNVERTS 40
#define PI 3.1415926535897932384626

#define ALARAJA -20.0
#define YLARAJA 10.0

float aika=0;
float aika2=0;


typedef struct {
    int width;
    int height;
    unsigned char *data;
} textureImage;

struct Particle {
  struct {
    float pos[3];
    float col[4];
  } ver[NUMPARTS];
} part_sdn;

struct Flakes {
  struct {
    float pos[3];
    float col[4];
  } ver[NUMFLAKES];
} part_flakes;

GLuint texture[2];

struct Sdn_filled {
  struct {
    float pos[3];
    float col[4];
  } ver[NUMSDNVERTS];
  struct {
    unsigned int ver[3];
  } triag[NUMSDNVERTS-1];
} filled_sdn;

struct Triag_chain {
  float pos[3][3];
  float col[3][4];
  struct Triag_chain * next;
};

struct Triag_chain flake;

void iterate_sflake() {
  struct Triag_chain* iter=&flake;
  struct Triag_chain* newTriags=NULL;
  while (iter!=NULL) {
    
  };
}

void generate_flake(void) {
  flake.pos[0][0]=0.0;
  flake.pos[0][1]=1.0;
  flake.pos[0][2]=0.0;
  flake.pos[1][0]=-sin(PI/2+PI/4);
  flake.pos[1][1]=-sin(PI/4);
  flake.pos[1][2]=0.0;
  flake.pos[1][0]=sin(PI/2+PI/4);
  flake.pos[1][1]=-sin(PI/4);
  flake.pos[1][2]=0.0;
  flake.next=NULL;

  for (int i=0; i<5; i++) iterate_sflake();  
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



int loadGLTextures()
{
    int status;
    textureImage *texti;

    status = 0;

    texti = static_cast<textureImage*> (malloc(sizeof(textureImage)));
    if (loadBMP("flare.bmp", texti))
    {
        status = 1;
        glGenTextures(1, &texture[0]);
        glBindTexture(GL_TEXTURE_2D, texture[0]);

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
    if (loadBMP("flake.bmp", texti))
    {
        status = 1;
        glGenTextures(1, &texture[1]);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

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
    }

    return status;
}



void sdnfunc(float t, float* vert) {
  if ((t>=0)&&(t<(5*PI/4))) {

    vert[0]=1.0*cos(t)-1.0; // Ylhäältä vastapäivään.
    vert[1]=1.0*sin(t);

  } else if ((t>=(5*PI/4))&&(t<2*PI)) {
    //    float d=sqrt(2*(1+sqrt(2))*(1+sqrt(2)));
    float d=1+1/sqrt(2);

    vert[0]=cos(5*PI/4)-1.0+d*(t-(5*PI/4))/(2*PI-(5*PI/4));
    vert[1]=sin(5*PI/4)-d*(t-(5*PI/4))/(2*PI-(5*PI/4));

  } else if ((t>=2*PI)&&(t<11*PI/4)) {
    //    float d=sqrt(2*(1+sqrt(2))*(1+sqrt(2)));
    float d=1+1/sqrt(2);

    vert[0]=d*(t-2*PI)/(11*PI/4-2*PI);
    vert[1]=-d+sin(5*PI/4) + d*(t-2*PI)/(11*PI/4-2*PI);

  } else if ((t>=11*PI/4)&&(t<4*PI)) {

    // sqrt((1+sqrt(2))**2 *2)

    vert[0]=1.0*cos(t-12*PI/4)+1.0;
    vert[1]=1.0*sin(t-12*PI/4);
  }
  vert[2]=0;
}

void regen_flake(int i, int anywhere) {

    part_flakes.ver[i].pos[0] = (random()%10000)/10000.0*20.0-10.0;
    if (anywhere) part_flakes.ver[i].pos[1] = (random()%10000)/10000.0*(ALARAJA-YLARAJA)+(YLARAJA);
    else part_flakes.ver[i].pos[1] = YLARAJA;
    //    part_flakes.ver[i].pos[2] = 0.0;
    part_flakes.ver[i].pos[2] = (random()%10000)/10000.0*35.0-20.0;

    part_flakes.ver[i].col[0] = 0.99;
    part_flakes.ver[i].col[1] = 0.99;
    part_flakes.ver[i].col[2] = 0.99;
    part_flakes.ver[i].col[3] = 0.99;
}

void init_flakes() {
  for (int i=0; i<NUMFLAKES; i++) {
    regen_flake(i, 1);
  }
}

void update_flake(float* pos) {
  //pos[0] = pos[0];
  pos[0] = pos[0]+sin(pos[1]*1.0+((random()%10000)/10000.0*0.5-0.25))*0.1;
  pos[1] = pos[1]-0.1;
}

void update_flakes() {
  for (int i=0; i<NUMFLAKES; i++) {
    update_flake(part_flakes.ver[i].pos);
    if(part_flakes.ver[i].pos[1]<ALARAJA) {
      regen_flake(i, 0);
    }
   
  }
}

void init_sdn() {
  filled_sdn.ver[0].pos[0]=0.0;
  filled_sdn.ver[0].pos[1]=0.0;
  filled_sdn.ver[0].pos[2]=0.0;

  filled_sdn.ver[0].col[0]=0.02;
  filled_sdn.ver[0].col[1]=0.0;
  filled_sdn.ver[0].col[2]=0.0;
  filled_sdn.ver[0].col[3]=1.0;

  sdnfunc(2*PI, filled_sdn.ver[1].pos);
  filled_sdn.ver[1].col[0]=0.8;
  filled_sdn.ver[1].col[1]=0.0;
  filled_sdn.ver[1].col[2]=0.0;
  filled_sdn.ver[1].col[3]=1.0;
  sdnfunc(4*PI-(PI+PI/2),filled_sdn.ver[2].pos); 
  filled_sdn.ver[2].col[0]=0.8;
  filled_sdn.ver[2].col[1]=0.0;
  filled_sdn.ver[2].col[2]=0.0;
  filled_sdn.ver[2].col[3]=1.0;
  sdnfunc(PI+PI/2,filled_sdn.ver[3].pos);
  filled_sdn.ver[3].col[0]=0.8;
  filled_sdn.ver[3].col[1]=0.0;
  filled_sdn.ver[3].col[2]=0.0;
  filled_sdn.ver[3].col[3]=1.0;


  filled_sdn.triag[0].ver[0]=2;
  filled_sdn.triag[0].ver[1]=1;
  filled_sdn.triag[0].ver[2]=0;
  filled_sdn.triag[1].ver[0]=1;
  filled_sdn.triag[1].ver[1]=3;
  filled_sdn.triag[1].ver[2]=0;


  for (int i=4; i<NUMSDNVERTS; i++) {
    float t=(PI+PI/2)-(3*PI)*((float)i-4.0)/((float)NUMSDNVERTS-5.0);
    if (t<0) t+=4*PI;
    sdnfunc(t,filled_sdn.ver[i].pos);
    filled_sdn.ver[i].col[0]=0.8;
    filled_sdn.ver[i].col[1]=0.0;
    filled_sdn.ver[i].col[2]=0.0;
    filled_sdn.ver[i].col[3]=1.0;

    filled_sdn.triag[i-2].ver[0]=0;
    filled_sdn.triag[i-2].ver[1]=i;
    filled_sdn.triag[i-2].ver[2]=i-1;
    
  }
  filled_sdn.triag[NUMSDNVERTS-2].ver[0]=0;
  filled_sdn.triag[NUMSDNVERTS-2].ver[1]=NUMSDNVERTS-1;
  filled_sdn.triag[NUMSDNVERTS-2].ver[2]=2;
}



void setParts(float ta) {
  int i=0;
  float t=ta;
  for (i=0; i<NUMPARTS; i++) {
    sdnfunc(t,part_sdn.ver[i].pos);
    t+=4*PI/NUMPARTS;
    if (t>4*PI) t-=4*PI;
  }
}

void initParts(void) {
  setParts(0);
  for (int i=0; i<NUMPARTS; i++) {
    part_sdn.ver[i].col[0]=0.99;
    part_sdn.ver[i].col[1]=0.2;
    part_sdn.ver[i].col[2]=0.70;
    part_sdn.ver[i].col[3]=0.99;
  }
}

static void redraw(void)
{
	int a,b;
        float x,y,z;
	unsigned int currentVer;

	timespec viive={0,1000000};

	setParts(aika2);
        update_flakes();

        aika+=0.01;
        aika2+=0.04;
        if (aika2>=4*PI) aika2-=4*PI;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

       	glRotatef(8.0*sin(aika*0.6),0.2,1.0,0.0);

	glPushMatrix();

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture[1]);

        glTranslatef(0,0,-20);

	for (int i=0; i<NUMFLAKES; i++) {
	  x=part_flakes.ver[i].pos[0];
          y=part_flakes.ver[i].pos[1];
	  z=part_flakes.ver[i].pos[2];
	  glBegin(GL_TRIANGLE_STRIP);
	  glColor4fv(part_flakes.ver[ i ].col);
	  glTexCoord2d(1, 1);
	  glVertex3f(x + 0.5f, y + 0.5f, z);
	  glTexCoord2d(0, 1);
	  glVertex3f(x - 0.5f, y + 0.5f, z);
	  glTexCoord2d(1, 0);
	  glVertex3f(x + 0.5f, y - 0.5f, z);
	  glTexCoord2d(0, 0);
	  glVertex3f(x - 0.5f, y - 0.5f, z);
	  glEnd();
	}


       	glRotatef(190.0*cos(aika),0.0,1.0,0.0);

	glBindTexture(GL_TEXTURE_2D, texture[0]);

	for (int i=0; i<NUMPARTS; i++) {
	  x=part_sdn.ver[i].pos[0];
          y=part_sdn.ver[i].pos[1];
	  z=part_sdn.ver[i].pos[2];


	  
	  glBegin(GL_TRIANGLE_STRIP);
	  glColor4fv(part_sdn.ver[ i ].col);
	  glTexCoord2d(1, 1);
	  glVertex3f(x + 0.5f, y + 0.5f, z);
	  glTexCoord2d(0, 1);
	  glVertex3f(x - 0.5f, y + 0.5f, z);
	  glTexCoord2d(1, 0);
	  glVertex3f(x + 0.5f, y - 0.5f, z);
	  glTexCoord2d(0, 0);
	  glVertex3f(x - 0.5f, y - 0.5f, z);
	  glEnd();
	}

	for (int i=0; i<NUMPARTS; i++) {
	  x=part_sdn.ver[i].pos[0];
          y=part_sdn.ver[i].pos[1];
	  z=part_sdn.ver[i].pos[2];


	  
	  glBegin(GL_TRIANGLE_STRIP);
	  glColor4fv(part_sdn.ver[ i ].col);
	  glTexCoord2d(0, 1);
	  glVertex3f(x - 0.5f, y + 0.5f, z);
	  glTexCoord2d(1, 1);
	  glVertex3f(x + 0.5f, y + 0.5f, z);
	  glTexCoord2d(0, 0);
	  glVertex3f(x - 0.5f, y - 0.5f, z);
	  glTexCoord2d(1, 0);
	  glVertex3f(x + 0.5f, y - 0.5f, z);
	  glEnd();
	}

	

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	
	
	for (a=0;a<NUMSDNVERTS-1;++a)
	  {
	    for (b=0;b<3;++b)
	      {
		currentVer=filled_sdn.triag[a].ver[b];
		
		glColor4fv(filled_sdn.ver[ currentVer ].col);
		glVertex3fv(filled_sdn.ver[ currentVer ].pos);
	      }
	  }
	
	glEnd();

	glBegin(GL_TRIANGLES);
	
	
	for (a=0;a<NUMSDNVERTS-1;++a)
	  {
	    for (b=2;b>=0;--b)
	      {
		currentVer=filled_sdn.triag[a].ver[b];
		
		glColor4fv(filled_sdn.ver[ currentVer ].col);
		glVertex3fv(filled_sdn.ver[ currentVer ].pos);
	      }
	  }
	
	glEnd();

	
		
	glPopMatrix();
	glPopMatrix();
	
	glutSwapBuffers();
	glutPostRedisplay();
	nanosleep(&viive, NULL);
}


int main(int argc, char **argv) 
{



 static GLfloat l0_pos[] = { 1.0, 1.0,-1.0,1.0};
  static GLfloat l1_pos[] = {-1.0,1.0,-1.0,1.0};
  static GLfloat l0_col[] = { 1.0, 1.0, 1.0,1.0};
  static GLfloat l1_col[] = { 1.0, 0.4, 0.4,1.0};


	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Katjalle");

	if (!loadGLTextures())
	  {
	    return 0;
	  }
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	//	glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

  //  glShadeModel(GL_SMOOTH);
  //glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT0, GL_POSITION, l0_pos);
  glLightfv(GL_LIGHT1, GL_POSITION, l1_pos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE , l0_col);
  glLightfv(GL_LIGHT1, GL_DIFFUSE , l1_col);
	glutDisplayFunc(redraw);	
	init_flakes();

	glMatrixMode(GL_PROJECTION);
	gluPerspective(45, //view angle
						1.0,	//aspect ratio
						1.0, //near clip
						2000.0);//far clip
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_CULL_FACE);


	initParts();
	init_sdn();

	glutMainLoop();

	return 0; 
}



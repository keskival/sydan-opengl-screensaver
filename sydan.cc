#include <stdio.h>
#include <math.h>
#include "common.hh"
#include "sydan.hh"

// 0.0 <= t <= 1.0
// vert = float[3]
void sydanfunc(float t, float* vert, float scale) {
  t *= 4*PI;
  if ((t>=0)&&(t<(5*PI/4))) {
    vert[0]=scale*cos(t)-scale; // Ylhäältä vastapäivään.
    vert[1]=scale*sin(t);

  } else if ((t>=(5*PI/4))&&(t<2*PI)) {
    float d=scale*(1+1/sqrt(2));

    vert[0]=scale*cos(5*PI/4)-scale+d*(t-(5*PI/4))/(2*PI-(5*PI/4));
    vert[1]=scale*sin(5*PI/4)-d*(t-(5*PI/4))/(2*PI-(5*PI/4));

  } else if ((t>=2*PI)&&(t<11*PI/4)) {
    float d=scale*(1+1/sqrt(2));

    vert[0]=d*(t-2*PI)/(11*PI/4-2*PI);
    vert[1]=-d+scale*sin(5*PI/4) + d*(t-2*PI)/(11*PI/4-2*PI);

  } else if ((t>=11*PI/4)&&(t<4*PI)) {

    vert[0]=scale*cos(t-12*PI/4)+scale;
    vert[1]=scale*sin(t-12*PI/4);
  } else {
    puts("ERROR! Bug?");
  }
  vert[2]=0;
}

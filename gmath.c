
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"


/*============================================
  IMPORANT NOTE

  Ambient light is represeneted by a color value

  Point light sources are 2D arrays of doubles.
       - The fist index (LOCATION) represents the vector to the light.
       - The second index (COLOR) represents the color.

  Reflection constants (ka, kd, ks) are represened as arrays of
  doubles (red, green, blue)
  ============================================*/


//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  color aref, dref, sref;
  normalize(normal);
  normalize(light[LOCATION]);
  normalize(view);
  aref = calculate_ambient(alight,areflect);
  dref = calculate_diffuse(light, dreflect, normal);
  sref = calculate_specular(light, sreflect, view, normal);
  i.red = aref.red + dref.red + sref.red;
  i.green = aref.green + dref.green + sref.green;
  i.blue = aref.blue + dref.blue + sref.blue;
  limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[RED];
  a.green = alight.green * areflect[GREEN];
  a.blue = alight.blue * areflect[BLUE];
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  double cos;
  normalize(normal);
  normalize(light[LOCATION]);
  cos = dot_product(normal, light[LOCATION]);
  if(cos < 0){
    cos = 0;
  }
  d.red = light[COLOR][RED] * dreflect[RED] * cos;
  d.green = light[COLOR][GREEN] * dreflect[GREEN] * cos;
  d.blue = light[COLOR][BLUE] * dreflect[BLUE] * cos;
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  double cos;
  int i;
  double ref[3];
  normalize(normal);
  normalize(light[LOCATION]);
  normalize(ref);
  cos = 2 * dot_product(normal, light[LOCATION]);
  if(cos < 0){
    cos = 0;
  }
  for(i = 0;i<3;i++){
    ref[i] = normal[i] * cos - light[LOCATION][i];
  }
  cos = dot_product(ref, view);
  if(cos < 0){
    cos = 0;
  }
  cos = pow(cos, SPECULAR_EXP);

  s.red = light[COLOR][RED] * sreflect[RED] * cos;
  s.green = light[COLOR][GREEN] * sreflect[GREEN] * cos;
  s.blue = light[COLOR][BLUE] * sreflect[BLUE] * cos;
  return s;
}

//limit each component of c to a max of 255
void limit_color( color * c ) {
  if(c->red > 255){
    c->red = 255;
  }
  if(c->green > 255){
    c->green = 255;
  }
  if(c->blue > 255){
    c->blue = 255;
  }
  if(c->red < 0){
    c->red = 0;
  }
  if(c->green < 0){
    c->green = 0;
  }
  if(c->blue < 0){
    c->blue = 0;
  }
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude;
  magnitude = sqrt( vector[0] * vector[0] +
                    vector[1] * vector[1] +
                    vector[2] * vector[2] );
  int i;
  for (i=0; i<3; i++) {
    vector[i] = vector[i] / magnitude;
  }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


//Calculate the surface normal for the triangle whose first
//point is located at index i in polygons
double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}

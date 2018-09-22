#include <cmath>
#include "model.h"

double MyDensity(double x, double y, double z);
double MyBnuT(double x, double y, double z);
Vector3D MyBfield(double x, double y, double z);

model::model(){
  Density_ = MyDensity;
  BnuT_ = MyBnuT;
  Bfield_ = MyBfield;

  kappa_abs = 1.;
  kappa_sca = 0.;
  kappa_ext = kappa_abs+kappa_sca;

  P0 = 0.02;

  Rmax = 100*AU;

  init_user_model();
}

void __attribute__((weak)) model::init_user_model(){return;}

double __attribute__((weak)) MyDensity(double x, double y, double z){
  double theta = atan(z/sqrt(x*x+y*y));
  if (fabs(theta) < PI/12) return 1e-15;
  else return 0;
}

double __attribute__((weak)) MyBnuT(double x, double y, double z){return 1;}

Vector3D __attribute__((weak)) MyBfield(double x, double y, double z){
  Vector3D B;
  B[0] = 1; B[1] = 0; B[2] = 0;
  
  return B;
}

bool __attribute__((weak)) model::reachBoundary(double x, double y, double z){return x*x+y*y+z*z>Rmax*Rmax;}

#include <cmath>
#include "../model.h"

double MyDensity(double x, double y, double z);
Vector3D MyBfield(double x, double y, double z);

void model::init_user_model(){
  Density_ = MyDensity;
  Bfield_ = MyBfield;
}

Vector3D MyBfield(double x, double y, double z){
  Vector3D B;
  B[0] = 1; B[1] = 0; B[2] = 0;

  double phi0 = atan2(y,x);
  double tBx, tBy;
  tBx = B[0]*cos(phi0)-B[1]*sin(phi0); tBy = B[0]*sin(phi0)+B[1]*cos(phi0);
  B[0] = tBx; B[1]=tBy;
  return B;
}

double MyDensity(double x, double y, double z){
  double r = sqrt(x*x+y*y+z*z);
  double theta = acos(z/r);
  double Sigma0 = 0.18387776826;
  double Rmax = 100*AU;
  double H = 0.17*r*pow((r/Rmax),0.17);

  double rho = Sigma0 / (r/Rmax)/sqrt(2*PI)/H*exp(-0.5*(z*z/H/H));

  if (fabs(z)>3*H) rho=0;

  return rho;
}


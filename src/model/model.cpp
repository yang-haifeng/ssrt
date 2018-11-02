#include <cmath>
#include "model.h"
#include "dust.h"

double MyDensity(double x, double y, double z);
double MyBnuT(double x, double y, double z);
Vector3D MyBfield(double x, double y, double z);


model::model(ParameterInput* pin){
  Density_ = MyDensity;
  BnuT_ = MyBnuT;
  Bfield_ = MyBfield;

  pdust = new dust(pin);
  kappa_ext = pin->GetReal("dust", "kappa_ext");

  tiny_step = pin->GetOrAddReal("scheme", "tiny_step", 0.1)*AU;
  dtau = pin->GetOrAddReal("scheme", "dtau", 0.1);
  Nphi = pin->GetOrAddInteger("scheme", "Nphi", 16);
  Ntheta = pin->GetOrAddInteger("scheme", "Nphi", 16);
  dOmega = 2./Ntheta*2.*PI/Nphi;

  init_user_model(pin);
}

model::~model(){
  delete pdust;
}

void __attribute__((weak)) model::init_user_model(ParameterInput* pin){return;}

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

bool __attribute__((weak)) model::reachBoundary(double x, double y, double z){
  const double Rmax=100*AU;
  return x*x+y*y+z*z>Rmax*Rmax;
}

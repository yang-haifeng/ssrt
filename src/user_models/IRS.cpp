#include <cmath>
#include <iostream>
#include "../model/model.h"

static const double Rmax = 100*AU;
static int Bflag;
static double nu;

double MyDensity(double x, double y, double z);
Vector3D MyBfield(double x, double y, double z);
double MyBnuT(double x, double y, double z);

void model::init_user_model(ParameterInput* pin){
  Density_ = MyDensity;
  Bfield_ = MyBfield;
  BnuT_ = MyBnuT;

  double lam_micron = pin->GetReal("dust", "wavelength");
  nu = con_c / (lam_micron*1e-4);

  Bflag = pin->GetOrAddInteger("model", "Bflag", 0);
}

bool model::reachBoundary(double x, double y, double z){
  double R = sqrt(x*x+y*y);
  if (R>Rmax) return true;
  double H = 0.17*R*pow((R/Rmax),0.17);
  if (fabs(z)>Rmax) return true;
  return false;
}

Vector3D MyBfield(double x, double y, double z){
  Vector3D B;
  B[0] = 0; B[1] = 0; B[2] = 0;
  B[Bflag] = 1.;

  double phi0 = atan2(y,x);
  double tBx, tBy;
  tBx = B[0]*cos(phi0)-B[1]*sin(phi0); tBy = B[0]*sin(phi0)+B[1]*cos(phi0);
  B[0] = tBx; B[1]=tBy;
  return B;
}

double MyDensity(double x, double y, double z){
  double R = sqrt(x*x+y*y);
  double Sigma0 = 0.18387776826;
  double Fsettle = 0.01;
  double H = 0.17*R*pow((R/Rmax),0.17)*Fsettle;

  double rho = Sigma0 / (R/Rmax)/sqrt(2*PI)/H*exp(-0.5*(z*z/H/H));

  if (fabs(z)>3*H) rho=0;

  if (R<0.01*AU) rho=0;

  return rho;
}

double MyBnuT(double x, double y, double z){
  double R = sqrt(x*x+y*y);
  double T = 20. * pow((R/Rmax), (-0.33));

  double result = bnuT(nu, T);
  //std::cout<<R/AU<<'\t'<<T<<'\t'<<result<<std::endl;

  if (R<0.01*AU) result=0;

  return result;
}

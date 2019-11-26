#include <cmath>
#include <iostream>
#include "../model/model.h"

static const double Rmax = 100*AU;
static int Bflag;
static double nu;
static double p,q;
static double Rc;
static double H0,T0,rho0;
static double Ts0, rs0;
static double alpha;

double MyDensity(double x, double y, double z);
Vector3D MyBfield(double x, double y, double z);
double MyBnuT(double x, double y, double z);

double hR(double R);

void model::init_user_model(ParameterInput* pin){
  Density_ = MyDensity;
  Bfield_ = MyBfield;
  BnuT_ = MyBnuT;

  double lam_micron = pin->GetReal("dust", "wavelength");
  nu = con_c / (lam_micron*1e-4);

  Bflag = pin->GetOrAddInteger("model", "Bflag", 1);

  p = pin->GetOrAddReal("model", "p", 1.585);
  q = pin->GetOrAddReal("model", "q", 0.43);
  Rc = pin->GetOrAddReal("model", "Rc", 100.)*AU;
  H0 = pin->GetOrAddReal("model", "H0", 12.*0.05)*AU;
  T0 = pin->GetOrAddReal("model", "T0", 70.);
  double Sigma_c = pin->GetOrAddReal("model", "Sigma_c", 25);
  rho0 = Sigma_c/100./sqrt(2*PI)/H0;

  Ts0 = pin->GetOrAddReal("model", "Ts0", 400.);
  rs0 = pin->GetOrAddReal("model", "rs0", 3.)*AU;
  alpha = 7./2-p-q/2.;
}

bool model::reachBoundary(double x, double y, double z){
  double R = sqrt(x*x+y*y);
  if (R>Rmax) return true;
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
  double H = hR(R);
  if (R<0.1*AU) return 0.0;
  if (fabs(z)>3*H) return 0.0;

  double rho;
  rho = rho0*pow(R/Rc,-p)*exp(-pow(R/Rc,alpha))*exp(-pow(z/H,2));

  return rho;
}

double MyBnuT(double x, double y, double z){
  double R = sqrt(x*x+y*y);
  double r = sqrt(x*x+y*y+z*z);
  double H = hR(R);
  double W = exp(-pow(z/3.0/H,2));
  double T,Tm;
  Tm = T0*pow(R/Rc,-q);
  T = W*Tm+(1-W)*Ts0*pow(r/rs0,-q);

  return bnuT(nu, T);
}

double hR(double R){
  return H0*pow(R/Rc,1.5-q/2.0);
}

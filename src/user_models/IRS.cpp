#include <cmath>
#include "../model/model.h"

static const double Rmax = 100*AU;
static int Bflag;

double MyDensity(double x, double y, double z);
Vector3D MyBfield(double x, double y, double z);

void model::init_user_model(ParameterInput* pin){
  Density_ = MyDensity;
  Bfield_ = MyBfield;

  kappa_ext = pin->GetOrAddReal("dust", "kappa_ext", 9.2622e-1);
  double albedo = pin->GetOrAddReal("dust", "albedo", 3.2498e-16/7.9751e-16);
  kappa_sca = kappa_ext * albedo;
  kappa_abs = kappa_ext - kappa_sca;

  P0 = pin->GetOrAddReal("dust", "p0", 0.05);;

  Bflag = pin->GetOrAddInteger("model", "Bflag", 0);
}

bool model::reachBoundary(double x, double y, double z){
  double R = sqrt(x*x+y*y);
  if (R>Rmax) return true;
  double H = 0.17*R*pow((R/Rmax),0.17);
  if (fabs(z)>3*H) return true;
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
  double H = 0.17*R*pow((R/Rmax),0.17);

  double rho = Sigma0 / (R/Rmax)/sqrt(2*PI)/H*exp(-0.5*(z*z/H/H));

  if (fabs(z)>3*H) rho=0;

  return rho;
}


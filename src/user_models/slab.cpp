#include <cmath>
#include "../model/model.h"

static const double Rmax=100*AU;
static int Bflag;
static double H;
static double rho0;

double MyDensity(double x, double y, double z);
Vector3D MyBfield(double x, double y, double z);

void model::init_user_model(ParameterInput* pin){
  Bflag = pin->GetOrAddInteger("model", "Bflag", 0);
  H = pin->GetReal("model", "H")*AU;
  rho0 = pin->GetReal("model", "rho0");

  Density_ = MyDensity;
  Bfield_ = MyBfield;
}

bool model::reachBoundary(double x, double y, double z){
  if (fabs(z)>H) return true;
  return (x*x+y*y)>Rmax*Rmax;
}

Vector3D MyBfield(double x, double y, double z){
  Vector3D B;
  B[0] = 0; B[1] = 0; B[2] = 0;
  B[Bflag] = 1.;

  return B;
}

double MyDensity(double x, double y, double z){
  return rho0;
}


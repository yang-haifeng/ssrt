#include <cmath>
#include <iostream>
#include "../model/model.h"

static const double Rmax = 100*AU;
static int Bflag;
static double nu;
static double p,q;
static double Rt,Ro;
static double ht,Tt,rhot;

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

  Bflag = pin->GetOrAddInteger("model", "Bflag", 2);

  p = pin->GetOrAddReal("model", "p", 2.0);
  q = pin->GetOrAddReal("model", "q", 0.75);
  Rt = pin->GetOrAddReal("model", "Rt", 36.)*AU;
  Ro = pin->GetOrAddReal("model", "Ro", 68.)*AU;
  ht = pin->GetOrAddReal("model", "ht", 12.)*AU;
  Tt = pin->GetOrAddReal("model", "Tt", 73.);
  double nt = pin->GetOrAddReal("model", "nt", 2.2e10);
  rhot = 1.4*nt*3.32e-24;
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
  if (fabs(z)>2*H) return 0.0;
  if (R<0.1*AU) return 0.0;

  double rho;
  if (R>Ro){
    rho=0.0;
  }
  else{
    rho = rhot*pow(R/Rt,-p)*exp(-z*z/2.0/H/H);
  }

  return rho;
}

double MyBnuT(double x, double y, double z){
  double R = sqrt(x*x+y*y);
  double H = hR(R);
  if (fabs(z)>2*H) return 0.0;
  if (R<0.1*AU) return 0.0;

  double T;
  if (R>Ro){
    T=0.0;
  }
  else if(R<Rt){
    T = Tt * pow( R/Rt, -q ) * exp( z*z/2.0/H/H );
  }
  else{
    T = Tt * pow( R/Rt, -q );
  }

  return bnuT(nu, T);
}

double hR(double R){
  double HR; 
  if (R<Rt){
    HR = ht*pow( (R/Rt), 1+(1-q)/2. );
  }
  else if(R>Ro){
    HR = 0.0;
  }
  else{
    HR = ht*exp(-(R-Rt)*(R-Rt)/(Ro-Rt)/(Ro-Rt));
  }
  return HR;
}

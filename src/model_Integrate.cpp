#include "model.h"
#include <iostream>

inline void initT(Matrix& T);
static const double dtau = 0.1;
static const double tiny_step = 0.1*AU;
static const int Nphi = 16;
static const int Ntheta = 16;
static const double dOmega = PI/Ntheta*2*PI/Nphi;

Vector model::Integrate(double x0, double y0, double z0, double nx, double ny, double nz, bool ScaFlag){
  Matrix T; initT(T);
  Vector Sabs; Matrix Mext; Matrix Z;
  Vector result; for (int i=0; i<4; i++) result[i]=0.;
  double x=x0,y=y0,z=z0;
  double step, rho;
  Vector Sin, Ssca;
  while( !reachBoundary(x,y,z) ){
    double rho = Density_(x,y,z);
    if (rho!=0){
      step = dtau/rho/kappa_ext;
      if (step>tiny_step) step=tiny_step;
    }
    else{
      step = tiny_step;
      x-=step*nx; y-=step*ny; z-=step*nz;
      continue;
    }
    Sabs = calcEmission(x,y,z,nx,ny,nz);
    Mext = calcExtinction(x,y,z,nx,ny,nz);
    for (int i=0; i<4; i++) Ssca[i]=0.;
    if (ScaFlag){
      for (int i=0; i<Ntheta; i++){
        double theta = PI/Ntheta*i;
        for (int j=0; j<Nphi; j++){
          double phi = 2*PI/Nphi*j;
          double nx1=sin(theta)*cos(phi), ny1=sin(theta)*sin(phi), nz1=cos(theta);
          Sin = Integrate(x, y, z, nx1, ny1, nz1, false);
	  Ssca += calcZMatrix(x,y,z,nx1,ny1,nz1,nx,ny,nz) * Sin * sin(theta) * dOmega;
        }
      }
    }
    result += T*(Sabs+Ssca)*step*rho;
    T -= T*Mext*step*rho;
    x-=step*nx; y-=step*ny; z-=step*nz;
  }
  return result;
}

void initT(Matrix& T){
  for(int i=0; i<4; i++){
   for(int j=0; j<4; j++){
     if(i==j) T[i*4+j] = 1;
     else T[i*4+j] = 0;
   }
  }
}

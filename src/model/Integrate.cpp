#include "model.h"
#include <iostream>
#include <fstream>

inline void initT(Matrix& T);

Vector model::Integrate(double x0, double y0, double z0, double nx, double ny, double nz, bool ScaFlag){
  Matrix T; initT(T);
  Vector Sabs; Matrix Mext; Matrix Z;
  Vector result; for (int i=0; i<4; i++) result[i]=0.;
  double x=x0,y=y0,z=z0;
  double step, rho;
  Vector Sin, Ssca;
  double tau=0;
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
    tau += step*kappa_ext*rho;
    if (tau>20) break;
    Sabs = calcEmission(x,y,z,nx,ny,nz);
    Mext = calcExtinction(x,y,z,nx,ny,nz);
    for (int i=0; i<4; i++) Ssca[i]=0.;
    if (ScaFlag){
      for (int i=0; i<Ntheta; i++){
	double cost = 2./Ntheta*i-1;
	double sint = sqrt(1-cost*cost);
        for (int j=0; j<Nphi; j++){
          double phi = 2*PI/Nphi*j;
          double nx1=sint*cos(phi), ny1=sint*sin(phi), nz1=cost;
          Sin = Integrate(x, y, z, nx1, ny1, nz1, false);
	  Ssca += calcZMatrix(x,y,z,nx1,ny1,nz1,nx,ny,nz) * Sin * dOmega;
        }
      }
    }
    if (ScaFlag)
      result += T*Ssca*step*rho;
    else
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

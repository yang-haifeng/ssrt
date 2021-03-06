#include <iostream>
#include "../../model.h"
#include "dust.h"

// In 1,2 frame:
// p = P0 sin(i)^2 / (1+P0 cos(i)^2)
// I = kappa_abs * (1+P0 cos(i)^2) / (1+P0)
// Q = kappa_abs * P0 sin(i)^2 / (1+P0)
Vector model::calcEmission(double x, double y, double z, double nx, double ny, double nz){
  double Bx, By, Bz;
  Vector3D Bvec = Bfield_(x,y,z);
  Bx = Bvec[0]; By = Bvec[1]; Bz = Bvec[2];

  double B = sqrt(Bx*Bx+By*By+Bz*Bz);
  Bx/=B; By/=B; Bz/=B; // Normalize B field first.

//double phi0 = atan2(y,x); // Rotate B field by phi0, the location of the point;
//double tBx, tBy;
//tBx = Bx*cos(phi0)-By*sin(phi0); tBy = Bx*sin(phi0)+By*cos(phi0);
//Bx = tBx; By=tBy;

  double cosinc; // inclination angle with respect to B field
  cosinc = fabs(Bx*nx+By*ny+Bz*nz); 

  double theta, phi;
  theta = acos(nz); phi = atan2(ny, nx);
  double et[3]; double ep[3];
  et[0] = cos(theta)*cos(phi); et[1]=cos(theta)*sin(phi); et[2]=-sin(theta);
  ep[0] = -sin(phi);           ep[1]=cos(phi);            ep[2]=0;
  double e1[3]; //double e2[3];
  e1[0] = By*nz-Bz*ny;
  e1[1] = Bz*nx-Bx*nz;
  e1[2] = Bx*ny-By*nx;

  double cosga = e1[0]*et[0]+e1[1]*et[1]+e1[2]*et[2];
  double singa = e1[0]*ep[0]+e1[1]*ep[1]+e1[2]*ep[2];
  Vector S; double bnuT = BnuT_(x,y,z);
  std::complex<double> a1, a3; a1=pdust->a1; a3=pdust->a3;
  S[0] = std::imag( a1 + 0.5*(a3-a1)*(1-cosinc*cosinc) );
  S[1] = std::imag( 0.5*(a1-a3)*(1-cosinc*cosinc) ) *(cosga*cosga - singa*singa);
  S[2] = std::imag( 0.5*(a1-a3)*(1-cosinc*cosinc) ) *2*cosga*singa;
  S[3] = 0.;
//S[0] = std::imag( 2./3*a1 + 1./3*a3 );
//S[1] = 0.; S[2] = 0.; S[3] = 0.;

  S = S* bnuT * 4*PI*pdust->k / pdust->mass;

  //std::cout<<S[0]/bnuT<<std::endl;

  return S;
}

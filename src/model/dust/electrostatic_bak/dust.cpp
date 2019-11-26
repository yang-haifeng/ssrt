#include <iostream>
#include "dust.h"
#include <cmath>

void GeometricPars(double s, double & L1, double & L3);

dust::dust(ParameterInput* pin){
  double eps_real, eps_imag;
  eps_real = pin->GetReal("dust", "eps_real");
  eps_imag = pin->GetReal("dust", "eps_imag");
  s = pin->GetReal("dust", "aspect_ratio");

  re = pin->GetReal("dust", "grain_size")/1e4;     // in micron
  lambda = pin->GetReal("dust", "wavelength")/1e4; // in micron
  rho = pin->GetReal("dust", "density");           // in g/cm^3

  eps = std::complex<double> (eps_real, eps_imag);
  
  double L1, L3;
  GeometricPars(s, L1, L3);

  a1 = pow(re,3) * (eps-1.)/(3.*1.+3*L1*(eps-1.));
  a3 = pow(re,3) * (eps-1.)/(3.*1.+3*L3*(eps-1.));

  mass = (4*PI/3.)*pow(re,3)*rho;
  k    = 2*PI/lambda;
  k6   = pow(k,4);

  //std::cout<<mass<<'\t'<<k<<std::endl;
}

dust::~dust(){return;}

// Calculate the geometric parameters L1 and L3.
// The convention adopted here is: L1 = L2 ? L3. For oblate ?=<; for prolate ?=>.
void GeometricPars(double s, double & L1, double & L3){
  if (s==1) {L1=1./3; L3=1./3; return;} // Spherical. All set to 1/3. 
  else if (s<1){ // prolate. c.f. Eq. (2). Note the difference in convention.
    double e2 = 1-s*s; double e=std::sqrt(e2);
    L3 = (1-e2)/e2*(-1+log( (1+e)/(1-e) )/2./e);
    L1 = (1-L3)/2.;
  }
  else{ // oblate. c.f. Eq. (3)
    double ge, e2;
    e2 = 1-1./s/s;
    ge = std::sqrt((1-e2)/e2);
    L1 = ge/2/e2 * (PI/2-atan(ge)) - ge*ge/2.;
    L3 = 1-2*L1;
  }
}

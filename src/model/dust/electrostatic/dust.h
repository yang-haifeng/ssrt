#ifndef _DUST_H
#define _DUST_H

#include "../../../io/parameter_input.h"
#include "../../../io/io_wrapper.h"
#include "../../../defs.h"
#include <complex>

class dust{
  public:
    std::complex<double> eps; // dielectric constants; eps = m^2;
    double s; // apect ratio; s<1 for prolate, s>1 for oblate;
    std::complex<double> a1, a3; // a1=a2 ? a3; ?=> for oblate, ?=< for prolate

    double sig_a1, sig_a2;
    double sig_s1, sig_s2;

    //double x; // = 2 pi a/lambda. The size parameter.
    double re; // effective grain size. 
    double lambda; // wavelength in question.
    double rho; // density of dust grain.

    double mass, k, k6; // auxiliary definition.

    explicit dust(ParameterInput* pin);
    ~dust();
};

#endif

#ifndef _DUST_H
#define _DUST_H

#include "../../../io/parameter_input.h"
#include "../../../io/io_wrapper.h"

class dust{
  public:
    double kappa_abs, kappa_sca, kappa_ext;
    double p0;

    explicit dust(ParameterInput* pin);
    ~dust();
};

#endif

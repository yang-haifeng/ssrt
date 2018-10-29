#include "dust.h"

dust::dust(ParameterInput* pin){
  kappa_abs = pin->GetReal("dust", "kappa_abs");
  kappa_sca = pin->GetReal("dust", "kappa_sca");
  kappa_ext = kappa_abs+kappa_sca;

  p0 = pin->GetOrAddReal("dust", "p0", 0.0);
}

dust::~dust(){return;}

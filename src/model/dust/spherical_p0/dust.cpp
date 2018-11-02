#include "dust.h"

dust::dust(ParameterInput* pin){
  kappa_ext = pin->GetReal("dust", "kappa_ext");
  double albedo = pin->GetOrAddReal("dust", "albedo", 0.0);
  kappa_sca = kappa_ext * albedo;
  kappa_abs = kappa_ext-kappa_sca;

  p0 = pin->GetOrAddReal("dust", "p0", 0.0);
}

dust::~dust(){return;}

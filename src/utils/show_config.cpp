#include <iostream>
#include "utils.h"
#include "../defs.h"

void show_config(){
  std::cout<<"Your SSRT distribution was configured with the following options:"<<std::endl;
  std::cout<<"  User-defined model       "<<USER_MODEL<<std::endl;
  std::cout<<"  Dust module:             "<<DUST_MODULE<<std::endl;
#ifdef OPENMP_PARALLEL
  std::cout<<"  OpenMP parallelism:      ON"<<std::endl;
#else
  std::cout<<"  OpenMP parallelism:      OFF"<<std::endl;
#endif
}

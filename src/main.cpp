#include "model.h"
#include <iostream>

int main(){
  model* M;
  M = new model();
  //Vector S = M->Integrate(25*AU, 0, 25*AU, 0,0,1);
  //std::cout<<S[0]<<'\t'<<S[1]<<'\t'<<S[2]<<'\t'<<S[3]<<std::endl;
  M->Image(PI/4, 100, false);
  //M->Image_circular(PI/4, 20, 9, 10*AU, 100*AU, true, false, "image.out");
}

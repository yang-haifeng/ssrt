#include "model/model.h"
#include <iostream>
#include <fstream>

int main(){
  model* M;
  M = new model();
  //Vector S = M->Integrate(25*AU, 0, 25*AU, 0,0,1);
  //std::cout<<S[0]<<'\t'<<S[1]<<'\t'<<S[2]<<'\t'<<S[3]<<std::endl;
  //M->Image(PI/4, 100, false);
  M->Image_circular(PI/4, 20, 9, 10*AU, 99*AU, true);
//std::ifstream Fin;
//Fin.open("xyz.dat", std::ios::binary);
//double temp;
//Fin.read(reinterpret_cast<char*>(&temp), sizeof(double));
//double x = temp;
//Fin.read(reinterpret_cast<char*>(&temp), sizeof(double));
//double  y = temp;
//Fin.read(reinterpret_cast<char*>(&temp), sizeof(double));
//double z = temp;
//Fin.read(reinterpret_cast<char*>(&temp), sizeof(double));
//double  nx1 = temp;
//Fin.read(reinterpret_cast<char*>(&temp), sizeof(double));
//double  ny1 = temp;
//Fin.read(reinterpret_cast<char*>(&temp), sizeof(double));
//double  nz1 = temp;
//M->Integrate(x, y, z, nx1, ny1, nz1, false);
}

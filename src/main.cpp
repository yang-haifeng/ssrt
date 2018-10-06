#include "model/model.h"
#include "io/parameter_input.h"
#include "io/io_wrapper.h"
#include <iostream>
#include <fstream>

int main(int argc, char *argv[]){
  ParameterInput * pin;
  pin = new ParameterInput;
  if (argc>1){
    IOWrapper parfile;
    parfile.Open(argv[1], IO_WRAPPER_READ_MODE);
    pin->LoadFromFile(parfile);
  }
  else{
    std::cout<<"ERROR: Input file needed!"<<std::endl;
    std::cout<<"usage: ssrt input.par"<<std::endl;
    exit(0);
  }

  model* M;
  M = new model(pin);
  //Vector S = M->Integrate(25*AU, 0, 25*AU, 0,0,1);
  //std::cout<<S[0]<<'\t'<<S[1]<<'\t'<<S[2]<<'\t'<<S[3]<<std::endl;
  //M->Image(PI/4, 100, false);

  std::string job, output_file;
  job = pin->GetOrAddString("job", "job", "circular_image");
  output_file = pin->GetOrAddString("job", "output_file", "image.out");

  if (job == "circular_image"){
    double inc = pin->GetOrAddReal("camera", "inc", 45.)*PI/180.; // inc in degree
    int Nr = pin->GetOrAddInteger("camera", "Nr", 20);
    int Nphi = pin->GetOrAddInteger("camera", "Nphi", 9);
    double Rin = pin->GetOrAddReal("camera", "Rin", 10)*AU; // Rin in AU
    double Rout = pin->GetOrAddReal("camera", "Rout", 99)*AU; // Rout in AU
    bool ifsca = pin->GetOrAddBoolean("model", "ifsca", true);

    M->Image_circular(inc, Nr, Nphi, Rin, Rout, ifsca, false, output_file);
  }
  else{
    std::cout<<job<<" not supported!"<<std::endl;
  }

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

  delete pin;
}

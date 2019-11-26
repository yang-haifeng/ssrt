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
    std::cout<<"No Input file. Showing the configuration. "<<std::endl;
    show_config();
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
  else if (job == "one_point"){
    double inc = pin->GetOrAddReal("camera", "inc", 45.)*PI/180.; // inc in degree
    double x = pin->GetReal("camera", "x")*AU;
    double y = pin->GetReal("camera", "y")*AU;
    double z = pin->GetOrAddReal("camera", "z", 0.0)*AU;
    bool ifsca = pin->GetOrAddBoolean("model", "ifsca", true);

    M->OnePointImage(inc, x, y, z, ifsca, output_file);
  }
  else if (job == "square_image"){
    double inc = pin->GetOrAddReal("camera", "inc", 45.)*PI/180.; // inc in degree
    int Npix = pin->GetOrAddInteger("camera", "Npix", 100);
    double FoV = pin->GetOrAddReal("camera", "FoV", 200)*AU; // Field of view in AU
    bool ifsca = pin->GetOrAddBoolean("model", "ifsca", true);

    M->Image(inc, Npix, ifsca, FoV, output_file);
  }
  else{
    std::cout<<job<<" not supported!"<<std::endl;
  }

  delete pin;
}

#ifndef _MODEL_H
#define _MODEL_H

#include <string>
#include "../defs.h"
#include "../utils.h"
#include "../io/parameter_input.h"
#include "../io/io_wrapper.h"

class model{
  protected:
    double kappa_abs, kappa_sca, kappa_ext;
    double P0;

  public:
    model(ParameterInput* pin);
    ~model();

    FieldFunction_ Density_;
    FieldFunction_ BnuT_;
    VectorFieldFunction_ Bfield_;

    bool reachBoundary(double x, double y, double z);
    Vector calcEmission(double x, double y, double z, double nx, double ny, double nz);
    Matrix calcExtinction(double x, double y, double z, double nx, double ny, double nz);
    Matrix calcZMatrix(double x, double y, double z, double nxi, double nyi, double nzi, double nxo, double nyo, double nzo);

    Vector Integrate(double x, double y, double z, double nx, double ny, double nz, bool ScaFlag=true);

    void Image(double inc, int Npix, bool ScaFlag=true, double FoV=200*AU, std::string fname="image.out");
    bool getSurface(double &x, double &y, double &z, double nx, double ny, double nz);
    //void Image_circular(double inc, int Nr, int Nph, double Rin, double Rout, bool ifsca=true, bool fAppend=false, std::string fname="image.out");
    void Image_circular(double inc, int Nr, int Nph, double Rin, double Rout, bool ifsca=true, bool fAppend=false, std::string fname="image.out");

    void init_user_model(ParameterInput* pin);
};

#endif

#include <iostream>
#include <fstream>
#include "model.h"

void progressBar(int N, int Ntot);

// Generate a image of Npix x Npix pixels. 
// inc is the inclination angle. Line of sight is in (sin(i), 0, cos(i)) direction.
// fName is the output file name. (default: Image.out)
// Npix default is 100.
void model::Image(double inc, int Npix, bool ifsca, double FoV, std::string fName){
  std::ofstream Fout;
  Fout.open(fName.c_str());

  double nx=sin(inc), ny=0, nz=cos(inc);
  double dx, dy, dz; 
  dy=FoV/(Npix-1);
  dx=dy*cos(inc);
  dz=-dy*sin(inc);
  double x0, y0, z0;
  y0 = -FoV/2.;
  x0 = y0*cos(inc);
  z0 = -y0*sin(inc);
  double x,y,z;
  Vector S;
  bool status;
  int Ncount=0; 
  for (int i=0; i<Npix; i++){
    for (int j=0; j<Npix; j++){
      progressBar(Ncount, Npix*Npix); Ncount++;
      x=x0+i*dx; y=y0+j*dy; z=z0+i*dz;

      status = getSurface(x, y, z, nx, ny, nz);
      if (!status) { //status==true means the point is out of the domain.
        Fout<<0.<<" "<<0.<<" "<<0.<<" "<<0.<<std::endl; // write all 0 here.
	continue; // Move on to the next point.
      }

      S = this->Integrate(x,y,z, nx,ny,nz, ifsca);

      Fout<<S[0]<<" "<<S[1]<<" "<<S[2]<<" "<<S[3]<<std::endl;
    }
  }
  Fout.close();
}

void model::Image_circular(double inc, int Nr, int Nph, double Rin, double Rout, 
        bool ifsca, bool fAppend, std::string fname){
  std::ofstream Fout;
  if (fAppend) Fout.open(fname.c_str(), std::ios::out | std::ios::app);
  else Fout.open(fname.c_str());
  if (!Fout.is_open()){
    std::cout<<"ERROR: failed to open output file: "<<fname<<std::endl;
    return;
  }

  double nx=sin(inc), ny=0, nz=cos(inc);
  Fout<<Nr<<" "<<Nph<<std::endl;
  Fout<<inc<<std::endl;
  Fout<<"#R(AU)\tphi(rad)\tI\tQ\tU\tV"<<std::endl;

  double mR[Nr][Nph], mphi[Nr][Nph];
  double mS0[Nr][Nph], mS1[Nr][Nph], mS2[Nr][Nph], mS3[Nr][Nph];

  #pragma omp parallel for
  for (int i=0; i<Nr; i++){
    Vector S;
    double R, phi;
    double x,y,z;
    if (Nr == 1) R = Rin;
    else R = Rin*exp( log(Rout/Rin)*i/(Nr-1) );
    for (int j=0; j<Nph; j++){
      //phi = 2*PI*j/Nph; // Full 2 PI
      phi = PI*j/(Nph-1); // Half circle: need more tension in data reduction

      mR[i][j] = R;
      mphi[i][j] = phi;

      std::cout<<"Working on: "<<R/AU<<" "<<phi/PI*180<<std::endl;
      x=R*cos(phi); y=R*sin(phi); z=0;
      getSurface(x, y, z, nx, ny, nz);
      S = this->Integrate(x,y,z, nx,ny,nz, ifsca);

      mS0[i][j]=S[0]; mS1[i][j]=S[1]; mS2[i][j]=S[2]; mS3[i][j]=S[3]; 
    }
  }

  for (int i=0; i<Nr; i++){
  for (int j=0; j<Nph; j++){
    Fout<<mR[i][j]/AU<<" "<<mphi[i][j]<<" "
        <<mS0[i][j]<<" "<<mS1[i][j]<<" "<<mS2[i][j]<<" "<<mS3[i][j]<<std::endl;
  }}
  Fout.close();
}

// getSurface function starts from the point (x, y, z) and moves in the direction
// (nx, ny, nz), until reaches the edge of the calculation domain.
// status=true if the initial point is already out of the calculation domain.
bool model::getSurface(double &x, double &y, double &z, double nx, double ny, double nz){
  if (reachBoundary(x,y,z)) return false;
  while ( !reachBoundary(x,y,z) ){
    x += nx*AU; y += ny*AU; z += nz*AU;
  }
  x-=nx*AU; y-=ny*AU; z-=nz*AU;
  return true;
}

void progressBar(int N, int Ntot){
  const int barWidth = 70;
  int pos = barWidth*N/Ntot;

  for (int i = 0; i < barWidth; ++i) {
      if (i < pos) std::cout << "=";
      else if (i == pos) std::cout << ">";
      else std::cout << " ";
  }
  std::cout << "] " << int(N*100/Ntot) << " %\r";
  std::cout.flush();
}

#include "utils.h"

// Calculate the matrix multiplication of two 4x4 matrices.
Matrix operator*(Matrix a, Matrix b){ 
  Matrix s;
  for (int i=0; i<4; i++){
    for (int j=0; j<4; j++){
      s[4*i+j] = 0;
      for (int k=0; k<4; k++){
        s[4*i+j] += a[4*i+k] * b[4*k+j];
      }   
    }   
  }
  return s;
}

Matrix operator*(Matrix a, double b){
  Matrix S;
  for(int i=0; i<16; i++){
    S[i] = a[i] * b;
  }
  return S;
}

Matrix operator*(double b, Matrix a){
  Matrix S;
  for(int i=0; i<16; i++){
    S[i] = a[i] * b;
  }
  return S;
}

// Calculate the produce of a 4x4 matrix and a length 4 vector.
Vector operator*(Matrix a, Vector b){ 
  Vector s;
  for (int i=0; i<4; i++){
    s[i] = 0;
    for (int j=0; j<4; j++){
      s[i] += a[4*i+j] * b[j];
    }   
  }
  return s;
}

Vector operator*(Vector a, double b){
  Vector s;
  for (int i=0; i<4; i++){
    s[i] = a[i] * b;
  }
  return s;
}

Vector operator/(Vector a, double b){
  Vector s;
  for (int i=0; i<4; i++){
    s[i] = a[i] / b;
  }
  return s;
}

Vector operator+(Vector a, Vector b){
  Vector s;
  for (int i=0; i<4; i++){
    s[i] = a[i] + b[i];
  }
  return s;
}

Vector& operator+=(Vector& lhs, const Vector rhs){
  for (int i=0; i<4; i++){
    lhs[i] += rhs[i];
  }
  return lhs;
}

Matrix& operator*=(Matrix& lhs, const double rhs){
  for (int i=0; i<16; i++) lhs[i]*=rhs;
  return lhs;
}

Matrix& operator-=(Matrix& lhs, const Matrix rhs){
  for (int i=0; i<16; i++){
    lhs[i]-=rhs[i];
  }
  return lhs;
}

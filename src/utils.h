#ifndef _UTILS_H
#define _UTILS_H

#include "math.h"
#include <array>

#define Matrix std::array <double, 16>
#define Vector std::array <double, 4>
#define Vector3D std::array <double, 3>

double dot(double * a, double * b);
Matrix operator*(Matrix a, Matrix b);
Matrix operator*(Matrix a, double b);
Matrix operator*(double b, Matrix a);
Vector operator*(Matrix a, Vector b);
Vector operator*(Vector a, double b);
Vector operator/(Vector a, double b);
Vector operator+(Vector a, Vector b);

Vector& operator+=(Vector& lhs, const Vector rhs);
Matrix& operator*=(Matrix& lhs, const double rhs);
Matrix& operator-=(Matrix& lhs, const Matrix rhs);

typedef double (*FieldFunction_)(double x, double y, double z);
typedef Vector3D (*VectorFieldFunction_)(double x, double y, double z);

#endif

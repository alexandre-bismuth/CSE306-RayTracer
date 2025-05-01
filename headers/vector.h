#ifndef VECTOR_H
#define VECTOR_H

#include <vector>

class Vector
{ // Code partially from initial handin
public:
    explicit Vector(double x = 0, double y = 0, double z = 0);
    double norm2() const;
    double norm() const;
    void normalize();
    double operator[](int i) const;
    double &operator[](int i);
    double data[3];
};

// Vector operators - from initial code handin
Vector operator+(const Vector &a, const Vector &b);
Vector operator-(const Vector &a, const Vector &b);
Vector operator-(const Vector &a);
Vector operator*(const double a, const Vector &b);
Vector operator*(const Vector &a, const double b);
Vector operator*(const Vector &a, const Vector &b);
Vector operator/(const Vector &a, const double b);
double dot(const Vector &a, const Vector &b);
Vector cross(const Vector &a, const Vector &b);

void operator+=(Vector &a, const Vector &b);
Vector randomCos(const Vector &a);
Vector normalize(const Vector &a);
unsigned char getLongest(const Vector &a);

#endif
#include "headers/vector.h"
#include "headers/utils.h"

Vector::Vector(double x, double y, double z)
{
	data[0] = x;
	data[1] = y;
	data[2] = z;
}

double Vector::norm2() const { return sqr(data[0]) + sqr(data[1]) + sqr(data[2]); }

double Vector::norm() const { return sqrt(norm2()); }

void Vector::normalize()
{
	double n = norm();
	data[0] /= n;
	data[1] /= n;
	data[2] /= n;
}

double Vector::operator[](int i) const { return data[i]; }
double &Vector::operator[](int i) { return data[i]; }

Vector operator+(const Vector &a, const Vector &b) { return Vector(a[0] + b[0], a[1] + b[1], a[2] + b[2]); }
Vector operator-(const Vector &a, const Vector &b) { return Vector(a[0] - b[0], a[1] - b[1], a[2] - b[2]); }
Vector operator-(const Vector &a) { return Vector(-a[0], -a[1], -a[2]); }
Vector operator*(const double a, const Vector &b) { return Vector(a * b[0], a * b[1], a * b[2]); }
Vector operator*(const Vector &a, const double b) { return Vector(a[0] * b, a[1] * b, a[2] * b); }
Vector operator*(const Vector &a, const Vector &b) { return Vector(a[0] * b[0], a[1] * b[1], a[2] * b[2]); }
Vector operator/(const Vector &a, const double b) { return Vector(a[0] / b, a[1] / b, a[2] / b); }

void operator+=(Vector &a, const Vector &b) { a = a + b; }

double dot(const Vector &a, const Vector &b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }

Vector cross(const Vector &a, const Vector &b)
{
	return Vector(a[1] * b[2] - a[2] * b[1],
				  a[2] * b[0] - a[0] * b[2],
				  a[0] * b[1] - a[1] * b[0]);
}

Vector randomCos(const Vector &a)
{
	int threadId = omp_get_thread_num();
	double randHeight = uniform(engine[threadId]);
	double randAngle = 2. * M_PI * uniform(engine[threadId]);
	double x = sqrt(1 - randHeight) * cos(randAngle);
	double y = sqrt(1 - randHeight) * sin(randAngle);
	double z = sqrt(randHeight);

	Vector tangent;
	if (abs(a[0]) <= abs(a[1]) && abs(a[0]) <= abs(a[2]))
		tangent = Vector(0, -a[2], a[1]);
	else if (abs(a[1]) <= abs(a[2]) && abs(a[1]) <= abs(a[0]))
		tangent = Vector(-a[2], 0, a[0]);
	else
		tangent = Vector(-a[1], a[0], 0);
	tangent.normalize();
	return tangent * x + cross(a, tangent) * y + a * z;
}

// Alternative normalize function
Vector normalize(const Vector &a) { return a / sqrt(sqr(a[0]) + sqr(a[1]) + sqr(a[2])); }

unsigned char getLongest(const Vector &a)
{
	unsigned char longestAxis = 0;
	if (abs(a[1]) > abs(a[longestAxis]))
		longestAxis = 1;
	if (abs(a[2]) > abs(a[longestAxis]))
		longestAxis = 2;
	return longestAxis;
}
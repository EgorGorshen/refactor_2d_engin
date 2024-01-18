#pragma once

#define pi 3.1415926535
class Vector
{
  public:
    double X = 0;
    double Y = 0;

    Vector(double x = 0, double y = 0) : X(x), Y(y)
    {
    }
    Vector(double x1, double y1, double x2, double y2) : X(x2 - x1), Y(y2 - y1)
    {
    }

    double length();
    double vector_rotation();

    void parse_vector(double x, double y);
    void rotate_vector(float deg);

    Vector normalise();
};

Vector operator+(const Vector &v1, const Vector &v2);
Vector operator-(const Vector &v1, const Vector &v2);
Vector operator*(const Vector &v1, double f);

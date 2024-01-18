#include "./vector.hpp"
#include <math.h>

Vector operator+(Vector v1, Vector v2)
{
    return Vector(v1.X + v2.X, v1.Y + v2.Y);
}

Vector operator-(Vector v1, Vector v2)
{
    return Vector(v1.X - v2.X, v1.Y - v2.Y);
}

Vector operator*(Vector v1, float f)
{
    return Vector(v1.X * f, v1.Y * f);
}

Vector operator*(Vector v1, double f)
{
    return Vector(v1.X * f, v1.Y * f);
}

double Vector::length()
{
    return pow((this->X * this->X + this->Y * this->Y), 0.5);
}

void Vector::parse_vector(double x, double y)
{
    this->X = this->X + x;
    this->Y = this->Y + y;
}

void Vector::rotate_vector(float deg)
{
    this->X = this->X * cos(deg / 180.0 * pi) - this->Y * sin(deg / 180.0 * pi);
    this->Y = this->X * sin(deg / 180.0 * pi) + this->Y * cos(deg / 180.0 * pi);
}

double Vector::vector_rotation()
{
    double res = atan(this->Y / this->X) / pi * 180;
    if (this->X < 0 and this->Y < 0)
        res += 180;
    if (this->X < 0 and this->Y > 0)
        res -= 180;
    if (res < 0)
        res = 360 + res;
    if (res >= 360)
        res = res - 360;
    return res;
}

Vector Vector::normalise()
{
    return Vector(this->X / this->length(), this->Y / this->length());
}

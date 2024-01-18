#pragma once

#include "../Vector/vector.hpp"

class TmPoint
{
  public:
    float X, Y;
    bool smoothed;

    TmPoint(Vector l, bool s) : X(l.X), Y(l.Y), smoothed(s)
    {
    }
};

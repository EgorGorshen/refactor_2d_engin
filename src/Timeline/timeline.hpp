#pragma once

#include "../TmPoint/tm_point.hpp"
#include <vector>

class Timeline
{
  private:
    bool looped = false;
    bool reversed = false;

  public:
    double current = 0.0001;
    std::vector<TmPoint> points;

    void AddPoint(float time = 0, float value = 0, bool smoothed = false);

    void update(double delta = 0);

    void reset();

    float GetVal();

    float GetVal(double time);
};

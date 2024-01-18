#pragma once

#include "../Vector/vector.hpp"
#include <cstdint>

#define DOT(v1, v2) v1.X *v2.X + v1.Y *v2.Y

struct Color
{
  public:
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;
};

bool BetweenX(double number, double between1, double between2);

bool BetweenV(Vector point, Vector between1, Vector between2);

float GetSin(float x1, float y1, float x2, float y2, float x);

float GetStr(float x1, float y1, float x2, float y2, float x);

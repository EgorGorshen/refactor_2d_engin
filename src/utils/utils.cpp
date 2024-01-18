#include "./utils.hpp"
#include "../Vector/vector.hpp"
#include <math.h>

bool BetweenX(double number, double between1, double between2)
{
    if ((between1 <= number <= between2) or (between2 <= number <= between1))
        return true;
    else
        return false;
}
bool BetweenV(Vector point, Vector between1, Vector between2)
{
    if (BetweenX(point.X, between1.X, between2.X) and BetweenX(point.Y, between1.Y, between2.Y))
        return true;
    else
        return false;
}
float GetSin(float x1, float y1, float x2, float y2, float x)
{
    float xfactor = x2 - x1;
    float yfactor = y2 - y1;
    float cpx = xfactor / 2 + x1;
    float cpy = yfactor / 2 + y1;
    return sin((x - cpx) * (pi / xfactor)) * (yfactor / 2) + cpy;
}

float GetStr(float x1, float y1, float x2, float y2, float x)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float tgx = dy / dx;
    float res = tgx * (x - x1) + y1;
    return res;
}

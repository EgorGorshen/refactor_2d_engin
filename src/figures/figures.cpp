#include "./figures.hpp"

Mesh CreateRect(float x1, float y1, float x2, float y2, int r, int g, int b, int a)
{
    std::vector<Vector> points;
    points.push_back(Vector(x1, y1));
    points.push_back(Vector(x2, y1));
    points.push_back(Vector(x2, y2));
    points.push_back(Vector(x1, y2));
    int cll[4] = {r, g, b, a};
    return Mesh(points, cll);
}

Mesh CreateNangle(int angles, double radius, int r, int g, int b, int a)
{
    std::vector<Vector> points;
    int color[4] = {r, g, b, a};
    Vector s = Vector(0, radius);
    double angle = 360 / angles;
    points.push_back(s);
    for (int i = 0; i < angles - 1; i++)
    {
        s.rotate_vector(angle);
        points.push_back(s);
    }
    return Mesh(points, color);
}

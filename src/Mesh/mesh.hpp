#pragma once

#include "../Vector/vector.hpp"
#include <vector>

class Mesh
{
  private:
    std::vector<Vector> points;
    int len = 0;

  public:
    bool CollisionEnabled = true;
    int color[4] = {0, 0, 0, 255};
    Vector Cpoint;
    Mesh()
    {
    }
    // Измените конструктор так, чтобы он принимал массив по ссылке
    Mesh(std::vector<Vector> arr, int (&cl)[4]) : points(arr)
    {
        len = arr.size();
        double sumx = 0, sumy = 0;
        for (Vector &i : points)
        {
            sumx += i.X;
            sumy += i.Y;
        }
        Cpoint = Vector(sumx / points.size(), sumy / points.size());

        // Копируйте элементы массива в цвет
        for (int i = 0; i < 4; ++i)
        {
            color[i] = cl[i];
        }
    }

    void change_color(int r = 0, int g = 0, int b = 0, int a = 255);

    void change_mesh_point(Vector newv = {0, 0}, int num = 0);

    std::vector<Vector> GetPoints();

    int GetSize() const;
};

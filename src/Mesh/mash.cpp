#include "./mesh.hpp"

void Mesh::change_color(int r, int g, int b, int a)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
}

void Mesh::change_mesh_point(Vector newv, int num)
{
    points[num] = newv;
}

std::vector<Vector> Mesh::GetPoints()
{
    return this->points;
}

int Mesh::GetSize() const
{
    return len;
}

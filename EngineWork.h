#include "FileWork.h"
#include "Utilities.h"
#include "src/Mesh/mesh.hpp"
#include "src/Timeline/timeline.hpp"
#include "src/TmPoint/tm_point.hpp"
#include "src/Vector/vector.hpp"
#include "src/utils/utils.hpp"
#include <iostream>

#ifndef _EngineWork_H_

std::vector<Timeline *> tms;

// objects
std::vector<std::vector<Vector>> RecountPoints(std::vector<Mesh> &meshes, double x, double y, double rot, double sc,
                                               Vector center)
{
    std::vector<std::vector<Vector>> nmn;
    for (int b = 0; b < meshes.size(); b++)
    {
        std::vector<Vector> nn;
        for (Vector &i : meshes[b].GetPoints())
        {
            double NewX = 0;
            double NewY = 0;
            NewX = i.X + x;
            NewY = i.Y + y;
            Vector NewV = {NewX, NewY};
            NewV = Vector(center.X + x, center.Y + y, NewX, NewY);
            NewV.rotate_vector(rot);
            NewV = NewV * sc;
            NewV.parse_vector(center.X + x, center.Y + y);
            nn.push_back(NewV);
        }
        nmn.push_back(nn);
    }
    return nmn;
}
// Objects default template
class Object
{
  private:
    double X = 0, Y = 0;
    double rotation = 0, scale = 1;
    Vector center;
    std::string type = "phys";
    std::vector<Mesh> meshes;
    std::vector<std::vector<Vector>> ready_points;
    std::vector<std::string> tags;

  public:
    bool toggle = true;
    int FocusMesh = 0;
    int layer = 0;

    virtual std::string update(double delta, std::string task = "", std::string arguments = "")
    {
        return "";
    }

    virtual void init(Mesh m, int fm, int x, int y, int z, float rot, float sc, std::string tgs, std::string arg,
                      std::string tp)
    {
        this->X = x;
        this->Y = y;
        this->layer = z;
        this->FocusMesh = fm;
        this->meshes.push_back(m);
        this->rotation = rot;
        this->scale = sc;
        this->tags = Split(tgs);
        this->type = tp;
        this->center = this->meshes[0].Cpoint;
        this->ready_points = RecountPoints(this->meshes, this->X, this->Y, this->rotation, this->scale, this->center);
    }

    virtual std::vector<Vector> GetPoints(int i)
    {
        return this->ready_points[i];
    }

    virtual Vector GetLocation()
    {
        return Vector(this->X, this->Y);
    }

    float GetRotation()
    {
        return this->rotation;
    }

    float GetScale()
    {
        return this->scale;
    }

    virtual Mesh *GetMesh(int i = 0)
    {
        return &this->meshes[i];
    }

    virtual int GetMeshesCount()
    {
        return this->meshes.size();
    }

    virtual void ChangeLocation(double x = 0, double y = 0, bool offset = false)
    {
        if (offset)
        {
            this->X += x;
            this->Y += y;
        }
        else
        {
            this->X = x;
            this->Y = y;
        }
        this->ready_points = RecountPoints(this->meshes, this->X, this->Y, this->rotation, this->scale, this->center);
    }

    virtual void ChangeRotation(float deg = 0, bool offset = false)
    {
        if (offset == false)
            this->rotation = deg;
        else
            this->rotation += deg;
        this->ready_points = RecountPoints(this->meshes, this->X, this->Y, this->rotation, this->scale, this->center);
    }

    virtual void ChangeScale(float sc = 0, bool offset = false)
    {
        if (offset == false)
            this->scale = sc;
        else
            this->scale += sc;
        this->ready_points = RecountPoints(this->meshes, this->X, this->Y, this->rotation, this->scale, this->center);
    }

    void AddTag(std::string tag = "")
    {
        this->tags.push_back(tag);
    }

    void RemoveTag(std::string tag = "")
    {
        for (int i = 0; i < this->tags.size(); i++)
        {
            if (this->tags[i] == tag)
            {
                this->tags.erase(std::next(this->tags.begin(), i));
                break;
            }
        }
    }

    bool CheckTag(std::string tag = "")
    {
        bool k = false;
        for (std::string i : this->tags)
        {
            if (i == tag)
            {
                return true;
                k = true;
                break;
            }
        }
        if (k == false)
        {
            return false;
        }
    }
    virtual void AddMesh(Mesh m)
    {
        this->meshes.push_back(m);
    }
    void RemoveMesh(int i = 0)
    {
        this->meshes.erase(std::next(meshes.begin(), i));
    }
    ~Object()
    {
    }
};

class ObjectPointer
{
  private:
    Object *ptr = nullptr;

  public:
    ObjectPointer()
    {
    }
    Object *get()
    {
        return ptr;
    }
    ObjectPointer &operator=(Object *u)
    {
        ptr = u;
        return *this;
    }
    ~ObjectPointer()
    {
    }
};

E_log e_log("sys/log.txt");

// World
class World
{
  public:
    std::vector<ObjectPointer> objects;
    std::vector<std::string> names;
    template <typename Class>
    ObjectPointer spawn(Mesh mesh, double x = 0, double y = 0, double rot = 0, double scale = 1,
                        std::string type = "phys", std::string name = "")
    {
        ObjectPointer gh;
        gh = new Class;
        gh.get()->init(mesh, 0, x, y, 0, rot, scale, "", "", type);
        objects.push_back(gh);
        names.push_back(name);
        e_log.write(*typeid(Class).name() + " " + name + " spawned\n");
        return gh;
    }
    void remove(std::string name = "")
    {
        for (int i = 0; i < names.size(); i++)
        {
            if (names[i] == name)
            {
                ObjectPointer buf = objects[i];
                objects.erase(std::next(objects.begin(), i));
                names.erase(std::next(names.begin(), i));
                delete buf.get();
                break;
            }
            e_log.write(name + " removed\n");
        }
    }
    ObjectPointer FindByName(std::string name = "")
    {
        ObjectPointer res;
        res = nullptr;
        for (int i = 0; i < names.size(); i++)
        {
            if (names[i] == name)
            {
                res = objects[i];
                break;
            }
        }
        return res;
    }
};

World ActiveWorld = World();

// ray cast
struct TraceResult
{
  public:
    bool Hited = false;
    Vector EndPoint = Vector(0, 0);
    Vector StartPoint = Vector(0, 0);
    double Distance = 0;
    ObjectPointer HitedObject;
};

TraceResult LineTraceForObject(double dir, Vector start, ObjectPointer obj, double dist)
{
    TraceResult res;
    double dista = -1;
    for (int b = 0; b < obj.get()->GetMeshesCount(); b++)
    {
        auto a = obj.get()->GetPoints(b);
        for (int j = 0; j < a.size(); j++)
        {
            Vector A = a[j];
            Vector B;
            if (j == a.size() - 1)
            {
                B = a[0];
            }
            else
            {
                B = a[j + 1];
            }
            Vector C = start;
            Vector D = Vector(dist * cos(dir / 180.0 * pi) + C.X, dist * sin(dir / 180.0 * pi) + C.Y);
            double r1 = (C.Y - A.Y) * (B.X - A.X) - (C.X - A.X) * (B.Y - A.Y);
            double r2 = (D.X - C.X) * (B.Y - A.Y) - (D.Y - C.Y) * (B.X - A.X);
            if (r2 != 0)
            {
                double r = r1 / r2;
                Vector P = (D - C) * r + C;
                if (r >= 0 and BetweenX(P.X, A.X, B.X) and BetweenX(P.Y, A.Y, B.Y))
                {
                    if (dista == -1)
                    {
                        dista = (P - C).length();
                        res.Hited = true;
                        res.EndPoint = P;
                        res.StartPoint = start;
                        res.HitedObject = obj;
                        res.Distance = dista;
                    }
                    else if ((P - C).length() < dista)
                    {
                        dista = (P - C).length();
                        res.Hited = true;
                        res.EndPoint = P;
                        res.StartPoint = start;
                        res.HitedObject = obj;
                        res.Distance = dista;
                    }
                }
            }
        }
    }
    if (dista == -1)
    {
        res.EndPoint = Vector(dist * cos(dir / 180.0 * pi) + start.X, dist * sin(dir / 180.0 * pi) + start.Y);
    }
    return res;
}

TraceResult RayCast(double dir = 0, Vector start = Vector(0, 0), double dist = 100)
{
    TraceResult res, buf;
    res.Distance = -1;
    res.EndPoint = Vector(dist * cos(dir / 180.0 * pi) + start.X, dist * sin(dir / 180.0 * pi) + start.Y);
    for (int i = 0; i < ActiveWorld.objects.size(); i++)
    {
        buf = LineTraceForObject(dir, start, ActiveWorld.objects[i], dist);
        if (res.Distance == -1 and buf.Hited == true)
        {
            res = buf;
        }
        else if (buf.Hited == true and buf.Distance < res.Distance)
        {
            res = buf;
        }
    }
    return res;
}

// collision
bool SimpleBoxCollision(std::vector<Vector> set1, std::vector<Vector> set2)
{
    double minX = set1[0].X, maxX = set1[0].X, minY = set1[0].Y, maxY = set1[0].Y;
    for (Vector &i : set1)
    {
        if (i.X > maxX)
        {
            maxX = i.X;
        }
        if (i.X < minX)
        {
            minX = i.X;
        }
        if (i.Y > maxY)
        {
            maxY = i.Y;
        }
        if (i.Y < minY)
        {
            minY = i.Y;
        }
    }
    Vector bc1[4] = {Vector(minX, minY), Vector(maxX, minY), Vector(maxX, maxY), Vector(minX, maxY)};
    minX = set2[0].X;
    maxX = set2[0].X;
    minY = set2[0].Y;
    maxY = set2[0].Y;
    for (Vector &i : set2)
    {
        if (i.X > maxX)
        {
            maxX = i.X;
        }
        if (i.X < minX)
        {
            minX = i.X;
        }
        if (i.Y > maxY)
        {
            maxY = i.Y;
        }
        if (i.Y < minY)
        {
            minY = i.Y;
        }
    }
    Vector bc2[4] = {Vector(minX, minY), Vector(maxX, minY), Vector(maxX, maxY), Vector(minX, maxY)};
    for (Vector &i : bc2)
    {
        if ((bc1[0].X <= i.X and i.X <= bc1[1].X) and (bc1[1].Y <= i.Y and i.Y <= bc1[2].Y))
        {
            return true;
        }
    }
    for (Vector &i : bc1)
    {
        if ((bc2[0].X <= i.X and i.X <= bc2[1].X) and (bc2[1].Y <= i.Y and i.Y <= bc2[2].Y))
        {
            return true;
        }
    }
}
bool BoxCollision(ObjectPointer o1, ObjectPointer o2)
{
    bool res = false;
    for (int i = 0; i < o1.get()->GetMeshesCount(); i++)
    {
        for (int b = 0; b < o2.get()->GetMeshesCount(); b++)
        {
            if (SimpleBoxCollision(o1.get()->GetPoints(i), o2.get()->GetPoints(b)))
            {
                res = true;
                break;
            }
        }
    }
    if (o1.get()->GetMesh()->CollisionEnabled == false or o2.get()->GetMesh()->CollisionEnabled == false)
    {
        res = false;
    }
    return res;
}

struct CrossResult
{
  public:
    bool crossed = false;
    bool HasPoint = false;
    Vector crosspoint = Vector(0, 0);
};
struct CollisionResult
{
  public:
    bool collision = false;
    std::vector<Vector> points;
};
CrossResult LineCrossing(Vector A, Vector B, Vector C, Vector D)
{
    CrossResult res;
    double rup = (C.Y - A.Y) * (B.X - A.X) - (C.X - A.X) * (B.Y - A.Y);
    double rdn = (D.X - C.X) * (B.Y - A.Y) - (D.Y - C.Y) * (B.X - A.X);
    double r;
    if (rdn == 0)
    {
        r = 0;
    }
    else
    {
        r = rup / rdn;
    }

    if (r > 0)
    {
        Vector P = ((D - C) * r) + C;
        if (BetweenV(P, A, B) and BetweenV(P, C, D))
        {
            res.crossed = true;
            res.HasPoint = true;
            res.crosspoint = P;
        }
    }
    if (r == 0)
    {
        Vector P = ((D - C) * r) + C;
        if (BetweenV(P, A, B) and BetweenV(P, C, D))
        {
            res.crossed = false;
            res.HasPoint = false;
        }
    }
    if (r < 0)
    {
        res.crossed = false;
        res.HasPoint = false;
    }
    return res;
}
CollisionResult SimpleCollision(std::vector<Vector> set1, std::vector<Vector> set2)
{
    CollisionResult res;
    for (int i = 0; i < set1.size(); i++)
    {
        Vector A = set1[i];
        Vector B = set1[(i + 1) % set1.size()];
        for (int b = 0; b < set2.size(); b++)
        {
            Vector C = set2[b];
            Vector D = set2[(b + 1) % set2.size()];
            CrossResult ef = LineCrossing(A, B, C, D);
            if (ef.crossed)
            {
                res.collision = true;
                if (ef.HasPoint)
                {
                    res.points.push_back(ef.crosspoint);
                }
            }
        }
    }
    return res;
}
CollisionResult Collision(ObjectPointer o1, ObjectPointer o2)
{
    CollisionResult res;
    for (int i = 0; i < o1.get()->GetMeshesCount(); i++)
    {
        for (int b = 0; b < o2.get()->GetMeshesCount(); b++)
        {
            CollisionResult a = SimpleCollision(o1.get()->GetPoints(i), o2.get()->GetPoints(b));
            if (a.collision)
            {
                res.collision = true;
                for (Vector &i : a.points)
                {
                    res.points.push_back(i);
                }
            }
        }
    }
    return res;
}
// file reading
Mesh ReadMesh(std::string FileName = "")
{
    std::ifstream streamer;
    streamer.open(FileName);
    std::string a;
    std::getline(streamer, a);
    std::vector<Vector> points;
    int Color[4];
    if (a == "#PTPMesh#")
    {
        std::getline(streamer, a);
        std::vector<std::string> crds = SlowSplit(a, ", ");
        for (int i = 0; i < crds.size(); i++)
        {
            points.push_back(Vector(stoi(Split(crds[i])[0]), stoi(Split(crds[i])[1])));
        }
        std::getline(streamer, a);
        Color[0] = stoi(Split(a)[0]);
        Color[1] = stoi(Split(a)[1]);
        Color[2] = stoi(Split(a)[2]);
        Color[3] = stoi(Split(a)[3]);
    }
    streamer.close();
    return Mesh(points, Color);
}
#endif

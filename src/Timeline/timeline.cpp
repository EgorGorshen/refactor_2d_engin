#include "./timeline.hpp"

void Timeline::AddPoint(float time, float value, bool smoothed)
{
    points.push_back(TmPoint(Vector(time, value), smoothed));
}
void Timeline::update(double delta)
{
    current += delta;
}

void Timeline::reset()
{
    current = 0.0001;
}

float Timeline::GetVal()
{
    TmPoint right = TmPoint(Vector(0, 0), false);
    TmPoint left = points[0];

    for (TmPoint &i : points)
    {
        if (i.X > left.X)
        {
            left = i;
        }
    }
    if (0 <= current <= left.X and points.size() >= 2)
    {
        for (TmPoint &i : points)
        {
            if (right.X <= i.X < current)
            {
                right = i;
            }
            if (current <= i.X <= left.X)
            {
                left = i;
            }
        }
        if (left.smoothed and right.smoothed)
        {
            return GetSin(left.X, left.Y, right.X, right.Y, current);
        }
        else if (left.smoothed)
        {
            return GetSin(left.X, left.Y, (right.X - left.X) + right.X, (right.Y - left.Y) + right.Y, current);
        }
        else if (right.smoothed)
        {
            return GetSin(left.X - (right.X - left.X), left.Y - (right.Y - left.Y), right.X, right.Y, current);
        }
        else
        {
            return GetStr(left.X, left.Y, right.X, right.Y, current);
        }
    }
    else if (current >= left.X)
    {
        return left.Y;
    }
    return 0;
}

float Timeline::GetVal(double time)
{
    TmPoint right = TmPoint(Vector(0, 0), false), left = points[0];
    for (TmPoint &i : points)
    {
        if (i.X > left.X)
        {
            left = i;
        }
    }
    if (time <= left.X and time >= 0 and points.size() >= 2)
    {
        for (TmPoint &i : points)
        {
            if (i.X >= right.X and i.X < time)
            {
                right = i;
            }
            if (i.X <= left.X and i.X >= time)
            {
                left = i;
            }
        }
        if (left.smoothed and right.smoothed)
        {
            return GetSin(left.X, left.Y, right.X, right.Y, time);
        }
        else if (left.smoothed)
        {
            return GetSin(left.X, left.Y, (right.X - left.X) + right.X, (right.Y - left.Y) + right.Y, time);
        }
        else if (right.smoothed)
        {
            return GetSin(left.X - (right.X - left.X), left.Y - (right.Y - left.Y), right.X, right.Y, time);
        }
        else
        {
            return GetStr(left.X, left.Y, right.X, right.Y, time);
        }
    }
    else if (time >= left.X)
    {
        return left.Y;
    }
    else
    {
        return 0;
    }
}

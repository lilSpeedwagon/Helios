#include "point.h"

Point::Point()
{

}

Point::Point(float x, float y) : x(x), y(y) {}

Point::Point(Point const& p)    {
    x = p.X();
    y = p.Y();
}

void Point::operator=(Point const& p)   {
    x = p.X();
    y = p.Y();
}

float& Point::X()   {
    return x;
}

float& Point::Y()   {
    return y;
}

float Point::X() const  {
    return x;
}

float Point::Y() const  {
    return y;
}

float Point::distantion(Point a, Point b)   {
    return qSqrt(qPow(a.X() - b.X(), 2) + qPow(a.Y() - b.Y(), 2));
}

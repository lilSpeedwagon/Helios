#ifndef POINT_H
#define POINT_H


class Point
{
private:
    float x;
    float y;
public:
    Point();
    Point(float x, float y);
    Point(Point const& p);
    void operator=(Point const& p);
    float X() const;
    float Y() const;
    float& X();
    float& Y();
};

#endif // POINT_H

#include "EdgeAndPoint.h"

Point::Point() : x(0), y(0) {}

Point::Point(float xCord, float yCord) : x(xCord), y(yCord) {}
Point& Point::operator=(const Point& p) {
    this->x = p.x;
    this->y = p.y;
    return *this;
}

Point::~Point() {}

Edge::Edge() : next(nullptr) {}

Edge::Edge(int ym, float xpos, float xstep, Edge* nxt) : 
    ymax(ym), x(xpos), dx(xstep), next(nxt)
{}

Edge::~Edge() {}
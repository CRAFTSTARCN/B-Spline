#ifndef POLYGENCONTROLLER_H
#define POLYGENCONTROLLER_H

#include "../core/Component.h"
#include "EdgeAndPoint.h"

#include <glm/glm.hpp>

#include <list>

class PolygenController : public Component {

    std::list<Point>* pointList;
    std::list<Point>::iterator selectedPoint;
    float threshold;
    int portSize;
    bool selected;

    float x0, y0, x1, y1;
    int listSize;

    public:
    PolygenController(float dist_threshold, int port_size);
    ~PolygenController();

    virtual void onActivate() override;
    virtual void update() override;
    virtual std::string componentType() const override;

    std::list<Point>::iterator findNearByPoint();
    void genNewPoint();

    void selectNewPoint();
    void movePoint();
    void erasePoint();

    float dist(const glm::vec2& a, const glm::vec2& b, const glm::vec2& p);
};

#endif
#ifndef GIZMOS_H
#define GIZMOS_H

#include "../core/Component.h"
#include "EdgeAndPoint.h"
#include "../core/Mesh.h"

#include <list>

class Gizmos : public Component {
    std::list<Point>* pointList;
    Mesh* mesh;

    public:

    Gizmos(std::list<Point>* pl);
    ~Gizmos();

    virtual void onActivate() override;
    virtual void update() override;
    virtual std::string componentType() const override;
};

#endif
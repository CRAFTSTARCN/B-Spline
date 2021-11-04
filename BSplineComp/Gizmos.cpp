#include "Gizmos.h"

#include "../core/RenderableObject.h"

Gizmos::Gizmos(std::list<Point>* pl) : pointList(pl) {}

Gizmos::~Gizmos() {}

void Gizmos::onActivate() {
    mesh = attachedObject->getMesh();
}

void Gizmos::update() {
    int oriMs = (int)mesh->vertexSize() / 3;
    int pos = 0;
    for(auto it = pointList->begin(); it != pointList->end(); ++it) {
        if(pos < oriMs) {
            mesh->updateVertex(it->x, it->y, 1.0f,pos);
            ++pos;
        } else {
            mesh->addVertex(it->x,it->y,1.0f);
            ++pos;
        }
    }

    while (pos < (int)mesh->vertexSize() / 3)  {
        mesh->popVertex();
    }
}

std::string Gizmos::componentType() const {
    return "Gizmos";
}
#include "PolygenController.h"

#include "../core/RenderableObject.h"
#include "../util/Exception.hpp"
#include "BSplineComp.h"
#include "../util/InputHandler.h"

PolygenController::PolygenController(float dist_threshold, int port_size) 
    : threshold(dist_threshold), portSize(port_size)
{
    selected = false;
}

PolygenController::~PolygenController() {}

void PolygenController::onActivate() {
    Component* bspline = attachedObject->getComponent("BSplineComp");
    if(!bspline) throw Throwable("Can not found bezier component of owner object");
    pointList = dynamic_cast<BSplineComp*>(bspline)->getPointLoop();
    listSize = pointList->size();
}

void PolygenController::update() {
    if(InputHandler::getMouseDownL()) {
        selectNewPoint();
    } else if(InputHandler::getMouseL()) {
        movePoint();
    } else if(InputHandler::getMouseUpL()){
        selected = false;
    } else if(InputHandler::getKeyDown(GLFW_KEY_D)) {
        erasePoint();
    }
    
}

std::list<Point>::iterator PolygenController::findNearByPoint() {
    for(auto it = pointList->begin(); it != pointList->end(); ++it) {
        if(std::abs(x0 - it->x) <= threshold && std::abs(y0 - it->y) <= threshold ) {
            return it;
        }
    }
    return pointList->end();
}

void PolygenController::genNewPoint() {
    auto lst = std::prev(pointList->end(),1);
    float line_dist;
    for(auto it = pointList->begin(); it != lst; ++it) {
        auto nxt = std::next(it,1);
        line_dist = dist(glm::vec2((float)it->x,(float)it->y), 
                               glm::vec2((float)nxt->x,(float)nxt->y),
                               glm::vec2(x0,y0));
        if(std::abs(line_dist) <= threshold) {
            selectedPoint = pointList->insert(nxt,Point((int)x0,(int)y0));
            selected = true;
            listSize++;
            return;
        }
    }

    auto& beg = pointList->front();
    auto& tail = pointList->back();
    line_dist = dist(glm::vec2((float)tail.x,(float)tail.y), glm::vec2((float)beg.x,(float)beg.y),glm::vec2(x0,y0));
    if(std::abs(line_dist) <= threshold) {
        pointList->push_back(Point((int)x0,(int)y0));
        selectedPoint = std::prev(pointList->end(),1);
        selected = true;
        listSize++;
    } else {
        pointList->emplace_back(x0,y0);
        listSize++;
    }

}

void PolygenController::selectNewPoint() {
    x0 = InputHandler::getMouseX(); y0 = (float)portSize - InputHandler::getMouseY();
    auto np = findNearByPoint();
    if(np == pointList->end()) {
        genNewPoint();
    } else {
        selectedPoint = np;
        selected = true;
    }
}

void PolygenController::movePoint() {
    if(!selected) return;
    x1 = InputHandler::getMouseX(); y1 = (float)portSize - InputHandler::getMouseY();
    int dist_x= x1 - x0, dist_y = y1 - y0;
    if(selectedPoint->x + dist_x > (float)portSize - threshold) {
        selectedPoint->x = (float)portSize - threshold;
    } else if(selectedPoint->x + dist_x < threshold) {
        selectedPoint->x = threshold;
    } else {
        selectedPoint->x += dist_x;
    }

    if(selectedPoint->y + dist_y > (float)portSize - threshold) {
        selectedPoint->y = (float)portSize - threshold;
    } else if(selectedPoint->y + dist_y < threshold) {
        selectedPoint->y = threshold;
    } else {
        selectedPoint->y += dist_y;
    }

    x0 = x1; y0 = y1;
}

void PolygenController::erasePoint() {
    if(listSize <= 3) {
        return;
    }
    x0 = InputHandler::getMouseX(); y0 = (float)portSize - InputHandler::getMouseY();
    auto del = findNearByPoint();
    if(del != pointList->end()) {
        pointList->erase(del);
        --listSize;
        return;
    }
}

float PolygenController::dist(const glm::vec2& a, const glm::vec2& b, const glm::vec2& p) {
    glm::vec3 ap(p-a,0.0f),bp(p-b,0.0f);
    float area = glm::length(glm::cross(ap,bp));
    float len = glm::distance(a,b);
    return area / len;
}

std::string PolygenController::componentType() const {
    return "PolygenController";
}
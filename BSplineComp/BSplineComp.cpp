#include "BSplineComp.h"

#include "../core/RenderableObject.h"
#include "../util/InputHandler.h"

BSplineComp::BSplineComp(int inStep, int inK) :
    step(inStep), k(inK),
    Component(),ownerMesh(nullptr),workStat(false),changeStat(false)
{
    points = {
        Point(450, 800), 
        Point(200, 100), 
        Point(850, 100)
    };
}

BSplineComp::~BSplineComp() {}

void BSplineComp::BSplinePoint(float cur_t) {
    pointArr[0].clear(); pointArr[1].clear();
    int j = 0;
    for(int i=k; i<tArray.size(); ++i) {
        if(cur_t < tArray[i]) {
            j = i-1;
            break;
        }
    }
    int cur_index = 0, next_index = 1;
    auto it = points.begin();
    it = std::next(it,j-k+1);
    for(int p = j-k+1; p<=j; ++p, ++it) {
        pointArr[0].push_back(*it);
    }

    int r = 1;
    while(pointArr[cur_index].size() > 1) {
        next_index = cur_index ^ 1;
        pointArr[next_index].clear();
        int i = j - k + r + 1;
        for(int p = 1; p < pointArr[cur_index].size(); ++p, ++i) {
            float lp = (cur_t - tArray[i]) / (tArray[i+k-r] - tArray[i]);
            float rp = (tArray[i+k-r] - cur_t) / (tArray[i+k-r] - tArray[i]);
            pointArr[next_index].push_back(Point(
                lp * pointArr[cur_index][p].x + rp * pointArr[cur_index][p-1].x,
                lp * pointArr[cur_index][p].y + rp * pointArr[cur_index][p-1].y
            ));
        }
        ++r;
        cur_index = next_index;
    }
    Point n_point = pointArr[cur_index][0];
    ownerMesh->addVertex(n_point.x,n_point.y, 10);
}

void BSplineComp::spanArray() {
    tArray.clear();
    int num = 0;
    for(int i=0; i<points.size(); ++i) {
        tArray.push_back(num++);
    }
    for(int i=0; i<k; ++i) {
        tArray.push_back(num++);
    }
}

void BSplineComp::BSpline() {
    ownerMesh->clearVertex();
    float t = tArray[k-1];
    float t_end = tArray[points.size()];
    float s = (t_end-t) / (float)step;
    for(int i=0; i<step; ++i, t += s) {
        BSplinePoint(t);
    }
}

void BSplineComp::update() {
    if(InputHandler::getKeyDown(GLFW_KEY_C)) {
        workStat = !workStat;
        changeStat = true;
    } else if(InputHandler::getMouseR()) {
        workStat = false;
        ownerMesh->clearVertex();
    }

    float scrollY = InputHandler::getScrollOffsetY();
    if(scrollY != 0.0f) {
        if(scrollY < 0.0f && k > 2) {
            --k;
            changeStat = true;
        } else if(scrollY > 0.0f && k < 6 && k < points.size()) {
            ++k;
            changeStat = true;
        }
    }

    if(InputHandler::getMouseL() ||
       InputHandler::getMouseDownL() ||
       InputHandler::getKeyDown(GLFW_KEY_D))
    {
        changeStat = true;
    }

    if(workStat) {
        spanArray();
        BSpline();
        changeStat = false;
    }
}

void BSplineComp::onActivate() {
    ownerMesh = attachedObject->getMesh();
}

std::string BSplineComp::componentType() const {
    return "BSplineComp";
}

std::list<Point>* BSplineComp::getPointLoop() {
    return &points;
}




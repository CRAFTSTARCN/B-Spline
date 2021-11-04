#pragma once

#include "../core/Component.h"
#include "../core/Mesh.h"
#include "EdgeAndPoint.h"

#include <list>

class BSplineComp : public Component {
    
public:
    int step;
    int k;
    std::vector<float> tArray;
    std::list<Point> points;
    Mesh* ownerMesh;
    bool workStat;
    float changeStat;
    std::vector<Point> pointArr[2];

public:
    BSplineComp(int inStep, int inK);
    ~BSplineComp() override;

    void BSplinePoint(float cur_t);
    void spanArray();
    void BSpline();
    
    void update() override;
    void onActivate() override;
    std::string componentType() const override;

    std::list<Point>* getPointLoop();
};

#include "LineWidthComp.h"
#include "PixSizeComp.h"

#include "glad/glad.h"

LineWidthComp::LineWidthComp(int lw) : lineWidth(lw) {}

LineWidthComp::~LineWidthComp() {}

std::string LineWidthComp::componentType() const {
    return "LineWidthComp";
}

void LineWidthComp::update() {
    glLineWidth(lineWidth);
}

PixSizeComp::PixSizeComp(int ps) : pixSize(ps) {}

PixSizeComp::~PixSizeComp() {}

std::string PixSizeComp::componentType() const {
    return "PixSizeComp";
}

void PixSizeComp::update() {
    glPointSize(pixSize);
}
#ifndef PIXSIZECOMP_H
#define PIXSIZECOMP_H

#include "../core/Component.h"

class PixSizeComp : public Component {
    
    int pixSize;

    public:

    PixSizeComp(int ps);
    ~PixSizeComp();

    virtual void update() override;
    virtual std::string componentType() const override;
};

#endif
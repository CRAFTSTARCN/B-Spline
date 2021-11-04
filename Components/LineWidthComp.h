#ifndef LINEWIDTHCOMP_H
#define LINEWIDTHCOMP_H

#include "../core/Component.h"

class LineWidthComp : public Component {
    
    int lineWidth;

    public:

    LineWidthComp(int lW);
    ~LineWidthComp();

    virtual void update() override;
    virtual std::string componentType() const override;
};

#endif
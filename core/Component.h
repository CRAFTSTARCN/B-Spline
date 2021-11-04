#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

class RenderableObject;

class Component {
    
    protected:
    RenderableObject* attachedObject;
    bool activeStatus;

    public:
    Component();
    virtual ~Component();

    void setParent(RenderableObject* obj);

    virtual void onStart();
    virtual void onActivate();
    virtual void onDeactivate();

    void activate();
    void deactivate();

    bool isActive() const;

    virtual void update();

    virtual std::string componentType() const = 0;
};

#endif
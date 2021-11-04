#include "Component.h"
#include "RenderableObject.h"

Component::Component() {
    attachedObject = nullptr;
    activeStatus = false;
}

Component::~Component() {}

void Component::setParent(RenderableObject* obj) {
    attachedObject = obj;
}

void Component::onStart() {}

void Component::onActivate() {}

void Component::onDeactivate() {}

void Component::activate() {
    if(!activeStatus) {
        activeStatus = true;
        onActivate();
    }
}

void Component::deactivate() {
    if(activeStatus) {
        activeStatus = false;
        onDeactivate();
    }
}

bool Component::isActive() const {
    return activeStatus;
}

void Component::update() {}
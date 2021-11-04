#ifndef RENDERABLEOBJECT_H
#define RENDERABLEOBJECT_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "Mesh.h"

#include <functional>
#include <vector>
#include <map>
#include <string>

class Component;

class RenderableObject {
    
    protected:
    Mesh* mesh;
    GLenum  rendType;
    int shaderProgram;
    std::map<std::string, Component*> components;

    public:
    glm::vec3 translate;
    glm::vec3 rotate;
    glm::vec3 scale;

    glm::mat4 transfromMatrix;

    RenderableObject(GLenum drawT, GLenum rendT, int shader);
    ~RenderableObject();

    std::vector<float> vertices;
    std::vector<int> indexes;

    /* 
        Three method to initialize object
        By using a customized callable object
        By using a refactable init function
        By using a file to initialize data
    */
    virtual void init(const std::function<void(RenderableObject*)>& initFunc);
    virtual void init();
    virtual void init(const std::string& fromFile);

    void setComponent(Component* comp);


    void initData(int initVSpace, int initESpace);

    virtual void processComponents();

    virtual void Start();

    virtual void doTransform(const glm::mat4& project);
    virtual void rendering();

    virtual void renderPipline(const glm::mat4& project);

    Mesh* getMesh();
    Component* getComponent(const std::string& compClassName);
};

#endif
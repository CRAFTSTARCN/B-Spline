#include "RenderableObject.h"

#include "Component.h"
#include "../util/Exception.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

RenderableObject::RenderableObject(GLenum drawT, GLenum rendT, int shader) : 
    translate(0.0f,0.0f,0.0f), rotate(0.0f,0.0f,0.0f), scale(1.0f,1.0f,1.0f)
{
    rendType = rendT;
    shaderProgram = shader;
    mesh = new Mesh(drawT);
}

RenderableObject::~RenderableObject() {
    for(auto iter = components.begin(); iter != components.end(); ++iter) {
        delete iter->second;
    }
    delete mesh;
}

/*
WARNNING:
Asynchronous function
You cant run this function in to thread simultaneously
*/
void RenderableObject::initData(int initVSpace, int initESpace) {
    mesh->allocVertex(initVSpace);
    mesh->allocIndex(initESpace);
    mesh->setIndexBufferData();
    mesh->setVertexBufferData();

    mesh->setAttribute();
}

void RenderableObject::setComponent(Component* comp) {
    auto iter = components.find(comp->componentType());
    if( iter != components.end()) {
        Component *ori = iter->second;
        delete ori;
    }
    components.emplace(comp->componentType(),comp);
    comp->setParent(this);
    comp->onStart();
    comp->activate();
}



void RenderableObject::processComponents() {
    for(auto iter = components.begin(); iter != components.end(); ++iter) {
        if(iter->second->isActive()) {
            iter->second->update();
        }
    }
}

void RenderableObject::Start() {}

void RenderableObject::doTransform(const glm::mat4& project) {
    transfromMatrix = glm::mat4(1.0f);

    transfromMatrix = glm::translate(transfromMatrix,translate);
    transfromMatrix = glm::scale(transfromMatrix,scale);

    if(rotate.x != 0.0f)
        transfromMatrix = glm::rotate(transfromMatrix,glm::radians(rotate.x),glm::vec3(1.0f,0.0f,0.0f));
    if(rotate.y != 0.0f)
        transfromMatrix = glm::rotate(transfromMatrix,glm::radians(rotate.y),glm::vec3(0.0f,1.0f,0.0f));
    if(rotate.z != 0.0f)
        transfromMatrix = glm::rotate(transfromMatrix,glm::radians(rotate.z),glm::vec3(0.0f,0.0f,1.0f));

    transfromMatrix = project * transfromMatrix;
}

void RenderableObject::rendering() {
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram,"MATRIX_MVP"),1,GL_FALSE,glm::value_ptr(transfromMatrix));

    mesh->updateBufferData();
    mesh->drawCall([this](unsigned int vao) {
        glBindVertexArray(vao);
        if(this->getMesh()->useElement()) {
            glDrawElements(rendType,this->getMesh()->indicesSize(),GL_UNSIGNED_INT,0);
        } else {
            glDrawArrays(rendType,0,this->getMesh()->vertexSize() / 3);
        }
        glBindVertexArray(0);
    });
    
}

/*
WARNNING:
Asynchronous function
You cant run this function in to thread simultaneously
*/
void RenderableObject::renderPipline(const glm::mat4& project) {
    processComponents();
    doTransform(project);
    rendering();
}

void RenderableObject::init(const std::function<void(RenderableObject*)>& initFunc) {
    initFunc(this);
}

void RenderableObject::init() {
    
}

void RenderableObject::init(const std::string& fromFile) {
    mesh->importData(fromFile);
}


Mesh* RenderableObject::getMesh() {
    return mesh;
}

Component* RenderableObject::getComponent(const std::string& compClassName) {
    auto iter = components.find(compClassName);
    if(iter == components.end()) {
        return nullptr;
    }
    return iter->second;
}
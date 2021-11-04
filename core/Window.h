#ifndef WINDOW_H
#define WINDOW_H

#include "init.h"
#include "RenderableObject.h"

#include <vector> 

class Window {
    private:
    GLFWwindow* window;
    unsigned int windowWidth;
    unsigned int windowHeight;
    
    glm::mat4 projection;

    std::map<std::string,int> shaderAsserts;

    public:
    std::vector<RenderableObject*> objects;

    /*Set up window context and glad here*/
    Window(unsigned int wd, unsigned int ht, const std::string& name);
    ~Window();

    /*Binding callback here*/
    virtual void initWindow();

    /*Setup rended objects' properties here*/
    virtual void setUpRendProp();

    /*Rend here*/
    virtual void renderProcess();

    void mainLoop();
};

#endif
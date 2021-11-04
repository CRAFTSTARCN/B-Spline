#include "core/Window.h"

#include "BSplineComp/BSplineComp.h"
#include "BSplineComp/PolygenController.h"
#include "util/Logger.h"
#include "util/Exception.hpp"
#include "util/InputHandler.h"
#include "core/SimpleShaderCompiler.h"
#include "BSplineComp//Gizmos.h"
#include "Components/LineWidthComp.h"
#include "Components/PixSizeComp.h"

#include "glm/gtc/matrix_transform.hpp"

Window::Window(unsigned int wd, unsigned int ht, const std::string& name) {
    windowWidth = wd;
    windowHeight = ht;
    window = glfwCreateWindow(wd, ht, name.c_str(), NULL, NULL);

    if(window == nullptr) {
        Logger::ERROR.log("Fail to create window");
        exit(-1);
    }

    initGLAD(window);
    projection = glm::ortho(0.0f,900.0f,0.0f,900.0f,-10.0f,100.0f);
}

Window::~Window() {
    for(auto* obj : objects) {
        delete obj;
    }
    for(auto& shader : shaderAsserts) {
        glDeleteProgram(shader.second);
    }
    glfwTerminate();
}

void Window::initWindow() {
    InputHandler::init(window);

    auto defaultSizeCallback = [](GLFWwindow* window, int width, int height){
        glViewport(0, 0, width, height);
    };

    glfwSetFramebufferSizeCallback(window, defaultSizeCallback);
    glEnable(GL_DEPTH_TEST);
}

void Window::renderProcess() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glLineWidth(1);
    for(int i=0; i<objects.size(); ++i) {
        objects[i]->renderPipline(projection);
    }
    glfwSwapBuffers(window);
}

void Window::mainLoop() {
    for(int i=0; i<objects.size(); ++i) {
        objects[i]->Start();
    }
    while(!glfwWindowShouldClose(window)) {
        renderProcess();
        if(InputHandler::getKeyDown(GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window,true);
        }
        InputHandler::clearStatus();
        glfwPollEvents();
    }
}

void Window::setUpRendProp() {
    int vert, fragWindow, fragPix, fragGizmos, shaderWindow, shaderPix, shaderGizmos;
    try {
        vert = SimpleShaderCompiler::compile("./shaders/vert.vert",GL_VERTEX_SHADER);
        fragWindow = SimpleShaderCompiler::compile("./shaders/fragWindow.frag",GL_FRAGMENT_SHADER);
        fragPix = SimpleShaderCompiler::compile("./shaders/pixel.frag",GL_FRAGMENT_SHADER);
        fragGizmos = SimpleShaderCompiler::compile("./shaders/gizmos.frag",GL_FRAGMENT_SHADER);

        shaderWindow = SimpleShaderCompiler::link(vert, fragWindow);
        shaderPix = SimpleShaderCompiler::link(vert, fragPix);
        shaderGizmos = SimpleShaderCompiler::link(vert, fragGizmos);

        glDeleteShader(vert);
        glDeleteShader(fragPix);
        glDeleteShader(fragWindow);
        glDeleteShader(fragGizmos);
    }
    catch(Throwable& e) {
        Logger::ERROR.log(e.type());
        Logger::ERROR.log(e.what());
        throw e;
    }
    shaderAsserts.emplace("shaderWindow",shaderWindow);
    shaderAsserts.emplace("shaderPix",shaderPix);
    shaderAsserts.emplace("shaderGizmos", shaderGizmos);

    BSplineComp* algo = new BSplineComp(64,3);
    
    RenderableObject* bspline = new RenderableObject(GL_DYNAMIC_DRAW, GL_LINE_STRIP, shaderPix);
    bspline->init();
    bspline->initData(810000 * 3,0);
    bspline->setComponent(algo);
    bspline->setComponent(new PolygenController(6,900));
    bspline->setComponent(new LineWidthComp(2.0f));

    RenderableObject* gizmos_line = new RenderableObject(GL_DYNAMIC_DRAW, GL_LINE_STRIP, shaderGizmos);
    gizmos_line->init();
    gizmos_line->initData(100 * 3, 0);
    gizmos_line->setComponent(new Gizmos(algo->getPointLoop()));
    gizmos_line->setComponent(new LineWidthComp(2.0f));

    RenderableObject* gizmos_point = new RenderableObject(GL_DYNAMIC_DRAW, GL_POINTS, shaderGizmos);
    gizmos_point->init();
    gizmos_point->initData(100 * 3, 0);
    gizmos_point->setComponent(new Gizmos(algo->getPointLoop()));
    gizmos_point->setComponent(new PixSizeComp(12.0f));

    objects.push_back(bspline);
    objects.push_back(gizmos_line);
    objects.push_back(gizmos_point);
}

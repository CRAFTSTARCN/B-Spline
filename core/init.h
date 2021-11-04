#ifndef INIT_H
#define INIT_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <cstdlib>

#include "../util/Logger.h"

inline void initGLFW(int major, int minor) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

inline void initGLAD(GLFWwindow* window) {
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::ERROR.log("Fail to initialize GLAD");
        exit(-1);
    }
}

#endif
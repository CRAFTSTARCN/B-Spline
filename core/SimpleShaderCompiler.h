#ifndef SIMPLESHADERCOMPILER_H
#define SIMPLESHADERCOMPILER_H

#include "glad/glad.h"

#include "../util/Exception.hpp"

#include <string>

class SimpleShaderCompiler {
    public:
    static int compile(const std::string& filepath, GLenum shaderType);

    static int compileFromStr(const std::string& shaderSource, GLenum shaderType);

    static int link(int vertexShader, int fragmentShader);
};

#endif
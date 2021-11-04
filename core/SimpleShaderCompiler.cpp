#define  _CRT_SECURE_NO_WARNINGS

#include "SimpleShaderCompiler.h"

#include "../util/Logger.h"

#include <string>
#include <exception>

int SimpleShaderCompiler::compile(const std::string& filepath, GLenum shaderType) {
    FILE* shaderFile;
    shaderFile = fopen(filepath.c_str(),"rt");

    if(shaderFile == nullptr) {
        throw IOException("Cannot open file" + filepath);
    }

    char c;
    std::string str;

    while(fread(&c,sizeof(char),1,shaderFile)) {
        str.push_back(c);
    }
    fclose(shaderFile);

    return compileFromStr(str,shaderType);
}

int SimpleShaderCompiler::compileFromStr(const std::string& shaderSource, GLenum shaderType) {
    int shader = glCreateShader(shaderType);
    const char* source = shaderSource.c_str();

    glShaderSource(shader,1,&source,NULL);
    glCompileShader(shader);
    int success;

    glGetShaderiv(shader,  GL_COMPILE_STATUS, &success);
    if(!success) {
        char info[512] = {0};
        glGetShaderInfoLog(shader,512, NULL, info);
        throw ShaderCompileError(info);
    }
    return shader;
}

int SimpleShaderCompiler::link(int vertexShader, int fragmentShader) {
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512] = {0};
        glGetProgramInfoLog(shaderProgram, 512, NULL, info);
        throw ShaderLinkError(info);
    }
    return shaderProgram;
}
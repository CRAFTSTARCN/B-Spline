#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <functional>

#include "glad/glad.h"

class Mesh {

    int firstPosV, firstPosE, lastPosV, lastPosE;

    unsigned int VAO,VBO,EBO;
    int sizeVBO,sizeEBO,drawType;

    std::vector<float> vertices;
    std::vector<int> indices;

    void genBuffer();

    public:

    Mesh(GLenum meshDrawType);
    Mesh(const std::string &meshDataPath, GLenum meshDrawType);
    ~Mesh();

    void importData(const std::string &meshDataPath);

    void addVertex(float x, float y, float z);
    void addIndex(int index);

    void updateVertex(float x, float y, float z, int idx);
    void updateIndex(int indexVal, int idx);

    void popVertex();
    void popIndex();

    void clearVertex();

    /* to GPU Mem function is SYNC */
    void allocVertex(int minSize);
    void allocIndex(int minSize);
    void setAttribute();

    void setVertexBufferData();
    void setIndexBufferData();
    
    //specially, this shell be called for each frame;
    void updateBufferData();
    bool useElement();

    //draw
    void drawCall(std::function<void(unsigned int)> rendFunc);

    int vertexSize() const;
    int indicesSize() const;
};

#endif
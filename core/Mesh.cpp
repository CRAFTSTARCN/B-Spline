#define  _CRT_SECURE_NO_WARNINGS

#include "Mesh.h"

#include "../util/Exception.hpp"

#include "glad/glad.h"

void Mesh::genBuffer() {
    firstPosV = INT_MAX;firstPosE =INT_MAX; lastPosV = -1, lastPosE = -1;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO); 
    sizeEBO = 0;
    sizeVBO = 0;
}

Mesh::Mesh(GLenum meshDrawType) {
    drawType = meshDrawType;
    genBuffer();
}

Mesh::Mesh(const std::string &meshDataPath, GLenum meshDrawType) {
    drawType = meshDrawType;
    importData(meshDataPath);
    genBuffer();
}

Mesh::~Mesh() {
    glDeleteBuffers(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
}

void Mesh::importData(const std::string &meshDataPath) {
    FILE *file;
    file = fopen(meshDataPath.c_str(),"rt");
    if(file == nullptr) throw IOException("Cannot open file: " + meshDataPath);
    int num_of_vert_coord,num_of_index;
    fscanf(file,"%d",&num_of_vert_coord);
    float vert_coord;
    for(int i=0; i<num_of_vert_coord; ++i) {
        fscanf(file,"%f",&vert_coord);
        vertices.push_back(vert_coord);
    }
    int index;
    fscanf(file,"%d",&num_of_index);
    for(int i=0; i<num_of_index; ++i) {
        fscanf(file,"%d",&index);
        indices.push_back(index);
    }
    fclose(file);
}

void Mesh::addVertex(float x, float y, float z) {
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
    int change_idx = vertices.size() - 1;
    firstPosV = std::min(firstPosV,change_idx-2);
    lastPosV = std::max(lastPosV,change_idx);
}

void Mesh::addIndex(int index) {
    indices.push_back(index);
    int change_idx = indices.size() - 1;
    firstPosE = std::min(firstPosE,change_idx);
    lastPosE = std::max(lastPosE,change_idx);
}

void Mesh::updateVertex(float x, float y, float z, int idx) {
    if(idx * 3 >= vertices.size()) {
        throw OutOfBoundException("Vertex array out of bound");
    }
    idx *= 3;
    vertices[idx] = x; vertices[idx + 1] = y; vertices[idx + 2] = z;
    firstPosV = std::min(firstPosV,idx);
    lastPosV = std::max(lastPosV,idx + 3);
}

void Mesh::updateIndex(int indexVal, int idx) {
    if(idx >= indices.size()) {
        throw OutOfBoundException("Index array out of bound");
    }
    indices[idx] = indexVal;
    firstPosV = std::min(firstPosV,idx);
    lastPosV = std::max(lastPosV,idx);
}

void Mesh::popVertex() {
    if(vertices.empty()) {
        throw OutOfBoundException("Try pop on empty vertex array");
    }
    vertices.pop_back();vertices.pop_back(); vertices.pop_back();
    int change_idx = vertices.size();
    firstPosV = std::min(firstPosV,change_idx);
    lastPosV = std::max(lastPosV,change_idx + 2);
}

void Mesh::popIndex() {
    if(indices.empty()) {
        throw OutOfBoundException("Try pop on empty index array");
    }
    indices.pop_back();
    int change_index = indices.size();
    firstPosE = std::min(firstPosE,change_index);
    lastPosE = std::max(lastPosE,change_index);
}

void Mesh::clearVertex() {
    lastPosV = vertices.size() - 1;
    vertices.clear();
    firstPosV = 0;
}

void Mesh::allocVertex(int minSize) {
    int alloc = vertices.size();
    alloc = std::max(minSize,alloc);
    if(alloc == 0) return;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, alloc * sizeof(float), nullptr, drawType);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    sizeVBO = alloc;
}

void Mesh::allocIndex(int minSize) {
    int alloc = std::max(minSize,(int)indices.size());
    if(alloc == 0) return;
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, alloc * sizeof(int), nullptr, drawType);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    sizeEBO = alloc;
}

void Mesh::setAttribute() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Mesh::setVertexBufferData() {
    if(vertices.empty()) return;
    if((int)vertices.size() > sizeVBO) {
        allocVertex(vertices.size());
    }

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Mesh::setIndexBufferData() {
    if(indices.empty()) return;
    if((int)indices.size() > sizeEBO) {
        allocIndex(indices.size());
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(int), indices.data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void Mesh::updateBufferData() {
    if(drawType == GL_STATIC_DRAW) return;
    if(firstPosV < (int)vertices.size()) {
        if((int)vertices.size() > sizeVBO) {
            setVertexBufferData();
        } else {
            glBindBuffer(GL_ARRAY_BUFFER,VBO);
            int lp = std::min((int)vertices.size() - 1,lastPosV);
            int size = (lp - firstPosV + 1);
            glBufferSubData(GL_ARRAY_BUFFER, firstPosV, size * sizeof(float), vertices.data() + firstPosV);
            glBindBuffer(GL_ARRAY_BUFFER,0);
        }
    }

    if(!useElement()) return;
    if(firstPosE < (int)indices.size()) {
        if((int)indices.size() > sizeEBO) {
            setIndexBufferData();
        } else {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
            int lp = std::min((int)indices.size() - 1,lastPosE);
            int size = (lp - firstPosE + 1);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, firstPosE, size * sizeof(float), indices.data() + firstPosV);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        }
    }

    firstPosV = INT_MAX;firstPosE =INT_MAX; lastPosV = -1, lastPosE = -1;
}

bool Mesh::useElement() {
    return sizeEBO > 0;
}

void Mesh::drawCall(std::function<void(unsigned int)> rendFunc) {
    rendFunc(VAO);
}

int Mesh::vertexSize() const {
    return vertices.size();
}

int Mesh::indicesSize() const {
    return indices.size();
}
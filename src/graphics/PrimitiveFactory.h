#pragma once

#include <glad/glad.h>
#include <vector>

struct Mesh {
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    GLsizei count = 0;
    GLenum drawMode = GL_TRIANGLES;
    bool isIndexed = false;

    void Destroy() {
        if (ebo != 0) { glDeleteBuffers(1, &ebo); ebo = 0; }
        if (vbo != 0) { glDeleteBuffers(1, &vbo); vbo = 0; }
        if (vao != 0) { glDeleteVertexArrays(1, &vao); vao = 0; }
        count = 0;
    }
};

class PrimitiveFactory {
public:
    static Mesh CreateUnitQuad();
    static Mesh CreateCircle(float radius = 0.5f, int segments = 24);
};

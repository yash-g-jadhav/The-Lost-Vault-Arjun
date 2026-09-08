#include "graphics/PrimitiveFactory.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Mesh PrimitiveFactory::CreateUnitQuad() {
    Mesh mesh;
    mesh.drawMode = GL_TRIANGLES;
    mesh.isIndexed = true;
    mesh.count = 6;

    // Center-origin unit quad [-0.5, 0.5]
    // 4 vertices: x, y, u, v
    float vertices[] = {
        -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
         0.5f, -0.5f,  1.0f, 0.0f, // Bottom-right
         0.5f,  0.5f,  1.0f, 1.0f, // Top-right
        -0.5f,  0.5f,  0.0f, 1.0f  // Top-left
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attribute 0: Position (vec2)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // Attribute 1: TexCoord (vec2)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return mesh;
}

Mesh PrimitiveFactory::CreateCircle(float radius, int segments) {
    Mesh mesh;
    mesh.drawMode = GL_TRIANGLE_FAN;
    mesh.isIndexed = false;

    if (segments < 3) {
        segments = 24;
    }

    // Triangle fan needs: center vertex (1) + perimeter vertices (segments + 1)
    std::vector<float> vertices;
    vertices.reserve((segments + 2) * 4);

    // Center vertex (x, y, u, v)
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.5f);
    vertices.push_back(0.5f);

    for (int i = 0; i <= segments; ++i) {
        float angle = static_cast<float>(i) * 2.0f * static_cast<float>(M_PI) / static_cast<float>(segments);
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);

        float x = cosA * radius;
        float y = sinA * radius;
        float u = cosA * 0.5f + 0.5f;
        float v = sinA * 0.5f + 0.5f;

        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(u);
        vertices.push_back(v);
    }

    mesh.count = static_cast<GLsizei>(vertices.size() / 4);

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Attribute 0: Position (vec2)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

    // Attribute 1: TexCoord (vec2)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return mesh;
}

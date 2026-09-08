#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include "graphics/Shader.h"
#include "graphics/PrimitiveFactory.h"

class Renderer2D {
public:
    Renderer2D();
    ~Renderer2D();

    bool Init(const std::string& shaderDir = "assets/shaders");
    void Shutdown();

    void BeginScene(const glm::mat4& viewProjection);
    void EndScene();

    void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
    void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotationDeg = 0.0f);
    void DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color);

    void SetClearColor(const glm::vec4& color);
    void Clear();

private:
    bool initialized;
    glm::mat4 currentViewProj;

    std::unique_ptr<Shader> primitiveShader;
    Mesh quadMesh;
    Mesh circleMesh;

    // Batching data
    struct BatchVertex {
        glm::vec2 position;
        glm::vec2 texCoord;
        glm::vec4 color;
    };
    std::vector<BatchVertex> m_BatchVertices;
    std::vector<GLuint> m_BatchIndices;
    GLuint m_BatchVAO = 0;
    GLuint m_BatchVBO = 0;
    GLuint m_BatchEBO = 0;
    const size_t m_MaxVertices = 10000; // arbitrary large enough for many quads
    void Flush();
};

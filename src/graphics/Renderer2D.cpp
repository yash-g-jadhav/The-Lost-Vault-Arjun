#include "graphics/Renderer2D.h"
#include "core/Log.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <cstddef>
#include <cmath>

static const char* s_DefaultPrimitiveVert =
"#version 330 core\n"
"layout (location = 0) in vec2 a_Position;\n"
"layout (location = 1) in vec2 a_TexCoord;\n"
"layout (location = 2) in vec4 a_Color;\n"
"\n"
"uniform mat4 u_MVP;\n"
"\n"
"out vec2 v_TexCoord;\n"
"out vec4 v_Color;\n"
"\n"
"void main() {\n"
"    gl_Position = u_MVP * vec4(a_Position, 0.0, 1.0);\n"
"    v_TexCoord = a_TexCoord;\n"
"    v_Color = a_Color;\n"
"}\n";

static const char* s_DefaultPrimitiveFrag =
"#version 330 core\n"
"in vec4 v_Color;\n"
"out vec4 FragColor;\n"
"\n"
"void main() {\n"
"    FragColor = v_Color;\n"
"}\n";

Renderer2D::Renderer2D()
    : initialized(false), currentViewProj(1.0f) {}

Renderer2D::~Renderer2D() {
    if (initialized) {
        Shutdown();
    }
}

bool Renderer2D::Init(const std::string& shaderDir) {
    LOG_INFO("Renderer2D", "Initializing 2D Renderer...");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    primitiveShader = std::make_unique<Shader>();
    std::string vertPath = shaderDir + "/primitive.vert";
    std::string fragPath = shaderDir + "/primitive.frag";

    if (!primitiveShader->LoadFromFile(vertPath, fragPath)) {
        LOG_WARN("Renderer2D", "Failed to load shader files, falling back to embedded shaders");
        if (!primitiveShader->LoadFromSource(s_DefaultPrimitiveVert, s_DefaultPrimitiveFrag)) {
            LOG_ERROR("Renderer2D", "Failed to compile fallback shaders");
            return false;
        }
    }

    quadMesh = PrimitiveFactory::CreateUnitQuad();
    circleMesh = PrimitiveFactory::CreateCircle(0.5f, 24);

    // ---------- Batch setup ----------
    glGenVertexArrays(1, &m_BatchVAO);
    glGenBuffers(1, &m_BatchVBO);
    glGenBuffers(1, &m_BatchEBO);

    glBindVertexArray(m_BatchVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_BatchVBO);
    glBufferData(GL_ARRAY_BUFFER, m_MaxVertices * sizeof(BatchVertex), nullptr, GL_DYNAMIC_DRAW);

    // Position attribute (location 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), (void*)offsetof(BatchVertex, position));
    // TexCoord attribute (location 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), (void*)offsetof(BatchVertex, texCoord));
    // Color attribute (location 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(BatchVertex), (void*)offsetof(BatchVertex, color));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BatchEBO);
    glBindVertexArray(0);
    // ---------------------------------

    initialized = true;
    LOG_INFO("Renderer2D", "Renderer2D initialized successfully");
    return true;
}

void Renderer2D::Shutdown() {
    if (!initialized) return;

    quadMesh.Destroy();
    circleMesh.Destroy();
    primitiveShader.reset();

    // Delete batch buffers
    if (m_BatchVAO) glDeleteVertexArrays(1, &m_BatchVAO);
    if (m_BatchVBO) glDeleteBuffers(1, &m_BatchVBO);
    if (m_BatchEBO) glDeleteBuffers(1, &m_BatchEBO);

    initialized = false;
    LOG_INFO("Renderer2D", "Renderer2D shut down");
}

void Renderer2D::SetClearColor(const glm::vec4& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer2D::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer2D::BeginScene(const glm::mat4& viewProjection) {
    currentViewProj = viewProjection;
    m_BatchVertices.clear();
    m_BatchIndices.clear();
}

void Renderer2D::EndScene() {
    Flush();
    glBindVertexArray(0);
}

void Renderer2D::Flush() {
    if (m_BatchVertices.empty() || m_BatchIndices.empty()) return;

    primitiveShader->Bind();
    primitiveShader->SetMat4("u_MVP", currentViewProj);

    glBindVertexArray(m_BatchVAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_BatchVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_BatchVertices.size() * sizeof(BatchVertex), m_BatchVertices.data());

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BatchEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_BatchIndices.size() * sizeof(GLuint), m_BatchIndices.data(), GL_DYNAMIC_DRAW);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_BatchIndices.size()), GL_UNSIGNED_INT, nullptr);

    // Clear batch after drawing
    m_BatchVertices.clear();
    m_BatchIndices.clear();

    glBindVertexArray(0);
}

void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color) {
    if (!initialized) return;

    const glm::vec2 localPos[4] = {
        {-0.5f, -0.5f},
        { 0.5f, -0.5f},
        { 0.5f,  0.5f},
        {-0.5f,  0.5f}
    };
    const glm::vec2 texCoord[4] = {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {1.0f, 1.0f},
        {0.0f, 1.0f}
    };

    size_t baseIndex = m_BatchVertices.size();
    for (int i = 0; i < 4; ++i) {
        glm::vec2 worldPos = glm::vec2(transform * glm::vec4(localPos[i], 0.0f, 1.0f));
        BatchVertex v;
        v.position = worldPos;
        v.texCoord = texCoord[i];
        v.color = color;
        m_BatchVertices.push_back(v);
    }
    // Two triangles (0,1,2) and (2,3,0)
    m_BatchIndices.push_back(baseIndex + 0);
    m_BatchIndices.push_back(baseIndex + 1);
    m_BatchIndices.push_back(baseIndex + 2);
    m_BatchIndices.push_back(baseIndex + 2);
    m_BatchIndices.push_back(baseIndex + 3);
    m_BatchIndices.push_back(baseIndex + 0);

    if (m_BatchVertices.size() >= m_MaxVertices) {
        Flush();
    }
}

void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float rotationDeg) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
    if (rotationDeg != 0.0f) {
        transform = glm::rotate(transform, glm::radians(rotationDeg), glm::vec3(0.0f, 0.0f, 1.0f));
    }
    transform = glm::scale(transform, glm::vec3(size, 1.0f));
    DrawQuad(transform, color);
}

void Renderer2D::DrawCircle(const glm::vec2& position, float radius, const glm::vec4& color) {
    if (!initialized) return;

    // Generate triangle-fan circle vertices on the CPU and push into the batch.
    // This avoids needing a separate VAO that lacks the a_Color attribute.
    const int segments = 24;
    const float twoPi = 6.28318530717958647692f;

    // Build fan: (center, p0, p1), (center, p1, p2), ...
    glm::vec2 center = position;
    for (int i = 0; i < segments; ++i) {
        float angle0 = static_cast<float>(i)     * twoPi / static_cast<float>(segments);
        float angle1 = static_cast<float>(i + 1) * twoPi / static_cast<float>(segments);

        glm::vec2 p0 = center + glm::vec2(std::cos(angle0), std::sin(angle0)) * radius;
        glm::vec2 p1 = center + glm::vec2(std::cos(angle1), std::sin(angle1)) * radius;

        size_t base = m_BatchVertices.size();

        BatchVertex vc; vc.position = center; vc.texCoord = {0.5f, 0.5f}; vc.color = color;
        BatchVertex v0; v0.position = p0;     v0.texCoord = {std::cos(angle0)*0.5f+0.5f, std::sin(angle0)*0.5f+0.5f}; v0.color = color;
        BatchVertex v1; v1.position = p1;     v1.texCoord = {std::cos(angle1)*0.5f+0.5f, std::sin(angle1)*0.5f+0.5f}; v1.color = color;

        m_BatchVertices.push_back(vc);
        m_BatchVertices.push_back(v0);
        m_BatchVertices.push_back(v1);

        m_BatchIndices.push_back(base + 0);
        m_BatchIndices.push_back(base + 1);
        m_BatchIndices.push_back(base + 2);
    }

    if (m_BatchVertices.size() >= m_MaxVertices) {
        Flush();
    }
}

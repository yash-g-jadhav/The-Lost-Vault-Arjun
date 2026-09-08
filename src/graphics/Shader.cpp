#include "graphics/Shader.h"
#include "core/Log.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() : programId(0) {}

Shader::~Shader() {
    if (programId != 0) {
        glDeleteProgram(programId);
        programId = 0;
    }
}

bool Shader::LoadFromFile(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertexPath);
        std::stringstream vShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        vShaderFile.close();
        vertexCode = vShaderStream.str();

        fShaderFile.open(fragmentPath);
        std::stringstream fShaderStream;
        fShaderStream << fShaderFile.rdbuf();
        fShaderFile.close();
        fragmentCode = fShaderStream.str();
    } catch (const std::exception& e) {
        LOG_ERROR("Shader", "Failed to read shader file: " << e.what()
                  << " (Paths: " << vertexPath << ", " << fragmentPath << ")");
        return false;
    }

    return LoadFromSource(vertexCode, fragmentCode);
}

GLuint Shader::CompileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> infoLog(logLength > 0 ? logLength : 512);
        glGetShaderInfoLog(shader, static_cast<GLsizei>(infoLog.size()), nullptr, infoLog.data());
        const char* typeStr = (type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT";
        LOG_ERROR("Shader", typeStr << " compilation failed:\n" << infoLog.data());
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

bool Shader::LoadFromSource(const std::string& vertexSource, const std::string& fragmentSource) {
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0) {
        return false;
    }

    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (fragmentShader == 0) {
        glDeleteShader(vertexShader);
        return false;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> infoLog(logLength > 0 ? logLength : 512);
        glGetProgramInfoLog(program, static_cast<GLsizei>(infoLog.size()), nullptr, infoLog.data());
        LOG_ERROR("Shader", "Program link failed:\n" << infoLog.data());

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (programId != 0) {
        glDeleteProgram(programId);
    }

    programId = program;
    uniformLocationCache.clear();
    return true;
}

void Shader::Bind() const {
    if (programId != 0) {
        glUseProgram(programId);
    }
}

void Shader::Unbind() const {
    glUseProgram(0);
}

GLint Shader::GetUniformLocation(const std::string& name) {
    auto it = uniformLocationCache.find(name);
    if (it != uniformLocationCache.end()) {
        return it->second;
    }

    GLint location = glGetUniformLocation(programId, name.c_str());
    if (location == -1) {
        LOG_DEBUG("Shader", "Uniform '" << name << "' not found or unused in shader program " << programId);
    }
    uniformLocationCache[name] = location;
    return location;
}

void Shader::SetInt(const std::string& name, int value) {
    GLint loc = GetUniformLocation(name);
    if (loc != -1) {
        glUniform1i(loc, value);
    }
}

void Shader::SetFloat(const std::string& name, float value) {
    GLint loc = GetUniformLocation(name);
    if (loc != -1) {
        glUniform1f(loc, value);
    }
}

void Shader::SetVec2(const std::string& name, const glm::vec2& value) {
    GLint loc = GetUniformLocation(name);
    if (loc != -1) {
        glUniform4fv(loc, 1, glm::value_ptr(glm::vec4(value.x, value.y, 0.0f, 0.0f)));
    }
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) {
    GLint loc = GetUniformLocation(name);
    if (loc != -1) {
        glUniform4fv(loc, 1, glm::value_ptr(glm::vec4(value.x, value.y, value.z, 0.0f)));
    }
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) {
    GLint loc = GetUniformLocation(name);
    if (loc != -1) {
        glUniform4fv(loc, 1, glm::value_ptr(value));
    }
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value) {
    GLint loc = GetUniformLocation(name);
    if (loc != -1) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
    }
}

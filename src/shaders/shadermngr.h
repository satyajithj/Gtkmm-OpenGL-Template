#pragma once

#include <string>

#include <glm/glm.hpp>

class ShaderMngr {

private:
    unsigned int m_ID;
    void checkCompileErrors(unsigned int shader, std::string type);

public:
    ShaderMngr(const char* vertexPath, const char* fragmentPath);

    unsigned int getID() const;

    // Activate the shader
    void use();

    // Utility "uniform" functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    void setMat4(const std::string &name, const glm::mat4 &mat) const;
};
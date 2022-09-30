#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

class Crate {

public:

    Crate(glm::vec3 p_Position);
    ~Crate();

    void deallocate();

    void update();
    void draw();

    glm::mat4 getPosition()     const;
    glm::mat4 getModelMatrix()  const;

private:
    unsigned int m_Vao, m_Vbo;
    unsigned int m_Texture;

    glm::vec3 m_Position;
    glm::mat4 m_Model;
};
#pragma once

#include <memory>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common/basetype.h"

class BaseProjection {

public:

    virtual glm::mat4 getProjectionMatrix() = 0;
    virtual void zoom(double) = 0;
    virtual void reset() = 0;

    void updateViewDimensions(float p_Width, float p_Height) {
        m_ViewWidth = p_Width;
        m_ViewHeight = p_Height;
    }

    float getAspectRatio() const {
        return (m_ViewWidth / m_ViewHeight);
    }

protected:
    float m_ViewWidth;
    float m_ViewHeight;
};
#pragma once

#include "cameras/baseprojection.h"

#define MIN_ORTHO_MAGNIFY 100.0f
#define MAX_ORTHO_MAGNIFY 8000.0f

class OrthographicProjection : public BaseProjection, BaseType {

public:
    OrthographicProjection(std::shared_ptr<AppState>);
    ~OrthographicProjection();

    virtual glm::mat4 getProjectionMatrix();
    virtual void zoom(double yOffset);
    virtual void reset();

private:
    float m_Left;
    float m_Right;
    float m_Bottom;
    float m_Top;

    float m_Scale;

    void computeLRBT();
};
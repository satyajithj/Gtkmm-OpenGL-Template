#pragma once

#include "cameras/baseprojection.h"

class PerspectiveProjection : public BaseProjection, BaseType {

public:
    PerspectiveProjection(std::shared_ptr<AppState>);
    ~PerspectiveProjection();

    virtual glm::mat4 getProjectionMatrix();
    virtual void zoom(double yOffset);
    virtual void reset();

private:

    // In degrees
    float m_FovAngle;
};
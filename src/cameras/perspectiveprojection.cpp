#include "perspectiveprojection.h"

PerspectiveProjection::PerspectiveProjection(std::shared_ptr<AppState> p_AppState) {
    // m_ViewWidth & m_ViewHeight are set in the first call
    // to ProjectionBase::updateViewDimensions()
    _AppState = p_AppState;
    reset();
}

PerspectiveProjection::~PerspectiveProjection() { }

glm::mat4 PerspectiveProjection::getProjectionMatrix() {
    return glm::perspectiveFov(
        glm::radians(m_FovAngle), m_ViewWidth, m_ViewHeight,
        _AppState->projectionNearPlane, _AppState->projectionFarPlane);
}

void PerspectiveProjection::reset() {
    m_FovAngle = _AppState->perspProjectionFovAngle;
}

void PerspectiveProjection::zoom(double yOffset) { }
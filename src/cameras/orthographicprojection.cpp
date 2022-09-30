#include "orthographicprojection.h"

#include <iostream>

OrthographicProjection::OrthographicProjection(std::shared_ptr<AppState> p_AppState) {
    // m_ViewWidth & m_ViewHeight are set in the first call
    // to ProjectionBase::updateViewDimensions()
    _AppState = p_AppState;
    reset();
}

OrthographicProjection::~OrthographicProjection() { }

glm::mat4 OrthographicProjection::getProjectionMatrix() {
    computeLRBT();
    return glm::ortho(
        m_Left, m_Right, m_Bottom, m_Top,
        _AppState->projectionNearPlane, _AppState->projectionFarPlane
    );
}

void OrthographicProjection::reset() {
    m_Scale = 2.0f * MIN_ORTHO_MAGNIFY;
    computeLRBT();
}

void OrthographicProjection::computeLRBT() {
    m_Left = -(m_ViewWidth / 2.0) / m_Scale;
    m_Right = -m_Left;
    m_Bottom = -(m_ViewHeight / 2.0) / m_Scale;
    m_Top = -m_Bottom;
}

void OrthographicProjection::zoom(double yOffset) {
    const float newScale = m_Scale * (1 - yOffset * 0.1f);
    m_Scale = std::clamp(newScale, MIN_ORTHO_MAGNIFY, MAX_ORTHO_MAGNIFY);
}
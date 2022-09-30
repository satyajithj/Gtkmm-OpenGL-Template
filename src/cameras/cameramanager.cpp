#include "cameramanager.h"

#include <iostream>

CameraManager::CameraManager(std::shared_ptr<AppState> p_AppState) {

    _AppState = p_AppState;

    m_Position = _AppState->cameraHomePosition;
    m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Set defaults
    m_Yaw = -90.0f;
    m_Pitch = 0.0f;

    updateCameraVectors();

    // Initialize projections ----

    // NOTE: Load from user preferences?
    m_CurrentProjectionType = ProjectionType::ORTHOGRAPHIC;

    m_Projections.insert({ProjectionType::ORTHOGRAPHIC,std::make_unique<OrthographicProjection>(_AppState)});
    m_Projections.insert({ProjectionType::PERSPECTIVE, std::make_unique<PerspectiveProjection>(_AppState)});
}

CameraManager::~CameraManager() {
    m_Projections.clear();
}

void CameraManager::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    front.y = sin(glm::radians(m_Pitch));
    front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front = glm::normalize(front);

    // Re-calculate the 'right' and 'up' vectors
    // ( Normalize the vectors )
    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
}

void CameraManager::setProjectionType(ProjectionType p_CurrentProjectionType) {
    m_CurrentProjectionType = p_CurrentProjectionType;
}

void CameraManager::updateViewDimensions(float p_Width, float p_Height) {
    for (const auto& [key, value] : m_Projections) {
        value->updateViewDimensions(p_Width, p_Height);
    }
}

void CameraManager::zoom(float yOffset) {

    if (m_CurrentProjectionType == ProjectionType::ORTHOGRAPHIC) {
        /* Zoom computations for an OrthographicProjection */
        m_Projections.at(ProjectionType::ORTHOGRAPHIC)->zoom(yOffset);

        glm::vec4 updatedScenePoint = glm::inverse(getProjectionMatrix()) * _AppState->pointerNDC;
        updatedScenePoint /= updatedScenePoint.w;
        const glm::vec4 cameraTranslation = updatedScenePoint - _AppState->pointerScene;
        _AppState->pointerScene = updatedScenePoint;

        m_Position -= glm::vec3(cameraTranslation.x, cameraTranslation.y, 0.0);
    } 
    
    else if (m_CurrentProjectionType == ProjectionType::PERSPECTIVE) {
        m_Position += (0.1f * yOffset * glm::normalize(glm::vec3(_AppState->pointerScene)));
    }
    
    else {
        // What happened?
    }
}

void CameraManager::reset() {
    for (const auto& [key, value] : m_Projections)  value->reset();
    m_Position = _AppState->cameraHomePosition;
}

glm::mat4 CameraManager::getViewMatrix() const {
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 CameraManager::getProjectionMatrix() const {
    return m_Projections.at(m_CurrentProjectionType)->getProjectionMatrix();
}
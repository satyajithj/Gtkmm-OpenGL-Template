#pragma once

#include <map>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "enums.h"
#include "cameras/orthographicprojection.h"
#include "cameras/perspectiveprojection.h"

#include "common/basetype.h"

class CameraManager : public BaseType {

private:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
    glm::vec3 m_WorldUp;

    float m_Yaw;
    float m_Pitch;

    ProjectionType m_CurrentProjectionType;
    std::map<ProjectionType, std::unique_ptr<BaseProjection>> m_Projections;

    void updateCameraVectors();

public:
    CameraManager(std::shared_ptr<AppState>);
    ~CameraManager();

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void setProjectionType(ProjectionType);
    void updateViewDimensions(float, float);

    void zoom(float);
    void reset();
};

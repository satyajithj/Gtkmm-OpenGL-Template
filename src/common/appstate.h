#pragma once

#include <glm/glm.hpp>

struct _ViewPortSize {
    double width {1280.0};
    double height {720.0};
    double halfWidth {width / 2.0f};
    double halfHeight {height / 2.0f};
};

struct _PositionXY {
    double x;
    double y;
};

class AppState {

public:

    bool isFullscreen {false};

    const glm::vec3 cameraHomePosition {0.0f, 0.0f, 3.0f};
    float projectionNearPlane {0.10f};
    float projectionFarPlane {100.0f};

    float perspProjectionFovAngle {45.0f};

    _ViewPortSize viewPortSize;

    _PositionXY pointerViewPort;
    glm::vec4 pointerScene;
    glm::vec4 pointerNDC;

    bool pointerInGLArea {false};

    void setViewportPointerPos(double x, double y) {
        pointerViewPort.x = x;
        pointerViewPort.y = y;
    }

    void setViewPortSize(double width, double height) {
        viewPortSize.width = width;
        viewPortSize.halfWidth = width / 2.0;
        viewPortSize.height = height;
        viewPortSize.halfHeight = height / 2.0;
    }
};
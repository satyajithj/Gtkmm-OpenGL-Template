#include "customglarea.h"

#include <iostream>

#include <sigc++/sigc++.h>

#include <gdkmm-3.0/gdkmm.h>
#include <epoxy/gl.h>

#include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

CustomGLArea::CustomGLArea(bool p_EnableDepth, std::shared_ptr<AppState> p_AppState) {

    m_EnableDepth = p_EnableDepth;
    _AppState = p_AppState;

    // Initialize the camera
    m_CameraManager = std::make_unique<CameraManager>(_AppState);

    set_hexpand(true);
    set_size_request(
        _AppState->viewPortSize.width,
        _AppState->viewPortSize.height);
    set_auto_render(true);
    set_required_version(3, 3);

    if (p_EnableDepth)  set_has_depth_buffer(true);

    // Necessary to mark a widget as "for pointer/touch use" only
    set_can_focus(false);

    // Connect glArea signals
    signal_realize().connect(sigc::mem_fun(*this, &CustomGLArea::realize));
    signal_unrealize().connect(sigc::mem_fun(*this, &CustomGLArea::unrealize), false);
    signal_render().connect(sigc::mem_fun(*this, &CustomGLArea::render), false);
    signal_resize().connect(sigc::mem_fun(*this, &CustomGLArea::on_area_resize));

    // Pointer events ----
    add_events(Gdk::EventMask::ENTER_NOTIFY_MASK);
    add_events(Gdk::EventMask::LEAVE_NOTIFY_MASK);
    // Pointer signals ----
    signal_enter_notify_event().connect(sigc::mem_fun(*this, &CustomGLArea::on_pointer_enter_notify_event));
    signal_leave_notify_event().connect(sigc::mem_fun(*this, &CustomGLArea::on_pointer_leave_notify_event));

    // Scroll & pointer-motion events ----
    add_events(Gdk::EventMask::SCROLL_MASK);
    // Scroll & pointer-motion signals ----
    signal_scroll_event().connect(sigc::mem_fun(*this, &CustomGLArea::on_scroll_event));
}

CustomGLArea::~CustomGLArea() { }

void CustomGLArea::on_area_resize(int width, int height) {
    _AppState->setViewPortSize(width, height);
    m_CameraManager->updateViewDimensions(width, height);

    queue_render();
}

void CustomGLArea::realize() {
    make_current();
    try {
        throw_if_error();

        m_GLClearFlag = GL_COLOR_BUFFER_BIT;
        if (m_EnableDepth) {
            glEnable(GL_DEPTH_TEST);
            m_GLClearFlag |= GL_DEPTH_BUFFER_BIT;
        }

        m_Shader = std::make_unique<ShaderMngr>("src/shaders/vs.glsl", "src/shaders/fs.glsl");
        m_Crate = std::make_unique<Crate>(glm::vec3(0.0f, 0.0f, 0.0f));

    } catch(const Gdk::GLError& gle) {
        std::cerr << "An error occured making the context current during realize:" << std::endl;
        std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }
}

void CustomGLArea::unrealize() {
    make_current();
    try {
        throw_if_error();

    } catch(const Gdk::GLError& gle) {
        std::cerr << "An error occured making the context current during unrealize" << std::endl;
        std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }
}

bool CustomGLArea::render(const Glib::RefPtr<Gdk::GLContext>& /* context */) {

    // std::cout << "Render" << std::endl;

    try {
        throw_if_error();
        glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
        glClear(m_GLClearFlag);

        // Draw ----
        m_Shader->use();
        m_Shader->setMat4("model", m_Crate->getModelMatrix());
        m_Shader->setMat4("view", m_CameraManager->getViewMatrix());
        m_Shader->setMat4("projection", m_CameraManager->getProjectionMatrix());

        m_Crate->draw();

        glFlush();

        return true;
    } catch(const Gdk::GLError& gle) {
        std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
        std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
        return false;
    }
}

void CustomGLArea::set_projection(ProjectionType type) {
    m_CameraManager->setProjectionType(type);
    queue_render();
}

// Handle pointer motion events ----

bool CustomGLArea::on_pointer_enter_notify_event(GdkEventCrossing* event) {
    _AppState->pointerInGLArea = true;
    _AppState->setViewportPointerPos(event->x, event->y);
    viewport2Scene(event->x, event->y);
    return true;
}

bool CustomGLArea::on_pointer_leave_notify_event(GdkEventCrossing* event) {
    _AppState->pointerInGLArea = false;
    _AppState->setViewportPointerPos(-1.0, -1.0);
    return true;
}

// Scroll event controller callbacks ----

bool CustomGLArea::on_scroll_event(GdkEventScroll* event) {

    // For scroll deltas, in GTK3, there is a bug that will probably never be fixed
    // Link: https://stackoverflow.com/questions/11775161/gtk3-get-mouse-scroll-direction

    double dy = 0.0;

    switch (event->direction) {
        case GDK_SCROLL_LEFT:
        case GDK_SCROLL_RIGHT:
            return false;
        case GDK_SCROLL_UP:
            dy = -1.0;
            break;
        case GDK_SCROLL_DOWN:
            dy = +1.0;
            break;
        default:
            break;
    }

    _AppState->setViewportPointerPos(event->x, event->y);
    viewport2Scene(event->x, event->y);
    m_CameraManager->zoom(dy);

    queue_render();

    return true;
}

void CustomGLArea::cameraReset() {
    m_CameraManager->reset();
    queue_render();
}

// Transformations
void CustomGLArea::viewport2Scene(double x, double y) {

    /*
        - For an orthographic projection, the following computes the scene xy coordinates.
        - For a perspective projection, assuming fixed FoV angle, the following computes the direction
        starting from the scene xy coordinates on the near plane.
    */
    
    _AppState->pointerNDC = glm::vec4(
        -1.00 + (x / _AppState->viewPortSize.halfWidth),
        1.00 - (y / _AppState->viewPortSize.halfHeight),
        2.00 /* Difference of z NDC ( 1 - (-1) ) */,
        1.00
    );

    // - 'w' is 1.0 for an orthographic projection
    _AppState->pointerScene = glm::inverse(m_CameraManager->getProjectionMatrix()) * _AppState->pointerNDC;
    _AppState->pointerScene /= _AppState->pointerScene.w;
}
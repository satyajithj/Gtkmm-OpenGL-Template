#include "customglarea.h"

#include <iostream>

#include <sigc++/sigc++.h>
#include <epoxy/gl.h>

#include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>

CustomGLArea::CustomGLArea(bool p_EnableDepth, std::shared_ptr<AppState> p_AppState) {

    m_EnableDepth = p_EnableDepth;
    _AppState = p_AppState;

    // Initialize the camera
    m_CameraManager = new CameraManager(_AppState);

    set_expand(true);
    set_size_request(
        _AppState->viewPortSize.width,
        _AppState->viewPortSize.height);
    set_auto_render(true);
    set_required_version(3, 3);

    if (p_EnableDepth)  set_has_depth_buffer(true);

    // Necessary to mark a widget as "for pointer/touch use" only
    set_can_focus(false);
    set_can_target(true);

    // Connect glArea signals
    signal_realize().connect(sigc::mem_fun(*this, &CustomGLArea::realize));
    signal_unrealize().connect(sigc::mem_fun(*this, &CustomGLArea::unrealize), false);
    signal_render().connect(sigc::mem_fun(*this, &CustomGLArea::render), false);
    signal_resize().connect(sigc::mem_fun(*this, &CustomGLArea::on_area_resize));

    // Pointer motion event controller ----
    m_MotionController = Gtk::EventControllerMotion::create();
    m_MotionController->signal_enter().connect(sigc::mem_fun(*this, &CustomGLArea::on_pointer_enter));
    m_MotionController->signal_motion().connect(sigc::mem_fun(*this, &CustomGLArea::on_pointer_motion));
    m_MotionController->signal_leave().connect(sigc::mem_fun(*this, &CustomGLArea::on_pointer_leave));
    add_controller(m_MotionController);

    // Scroll event controller ----
    m_ScrollController = Gtk::EventControllerScroll::create();
    // The flag is set to "Gtk::EventControllerScroll::Flags::NONE" by default
    m_ScrollController->set_flags(Gtk::EventControllerScroll::Flags::VERTICAL);
    m_ScrollController->signal_scroll_begin().connect(sigc::mem_fun(*this, &CustomGLArea::on_scroll_begin), true);
    m_ScrollController->signal_scroll().connect(sigc::mem_fun(*this, &CustomGLArea::on_scroll), true);
    m_ScrollController->signal_scroll_end().connect(sigc::mem_fun(*this, &CustomGLArea::on_scroll_end), true);
    add_controller(m_ScrollController);

    // // Legacy event controller ----
    // legacyController = Gtk::EventControllerLegacy::create();
    // legacyController->signal_event().connect(sigc::mem_fun(*this, &CustomGLArea::on_legacy_event), true);
    // add_controller(legacyController);
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

        m_Shader = new ShaderMngr("src/shaders/vs.glsl", "src/shaders/fs.glsl");

        m_Crate = new Crate(glm::vec3(0.0f, 0.0f, 0.0f));

    } catch(const Gdk::GLError& gle) {
        std::cerr << "An error occured making the context current during realize:" << std::endl;
        std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
    }
}

void CustomGLArea::unrealize() {
    make_current();
    try {
        throw_if_error();

        delete m_Shader;
        delete m_CameraManager;

        delete m_Crate;

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

void CustomGLArea::on_pointer_enter(double x, double y) {
    _AppState->pointerInGLArea = true;
    _AppState->setViewportPointerPos(x, y);
    viewport2Scene(x, y);
}

void CustomGLArea::on_pointer_motion(double x, double y) {
    _AppState->setViewportPointerPos(x, y);
    viewport2Scene(x, y);
}

void CustomGLArea::on_pointer_leave() {
    _AppState->pointerInGLArea = false;
    _AppState->setViewportPointerPos(-1.0, -1.0);
}

// Scroll event controller callbacks ----

bool CustomGLArea::on_scroll(double x, double y) {
    if (y == 0) return false;

    // std::cout << "Scroll: " << y << std::endl;
    m_CameraManager->zoom(y);
    queue_render();

    return true;
}

void CustomGLArea::on_scroll_begin() { }

void CustomGLArea::on_scroll_end() { }

// // Legacy event controller callback ----
// bool CustomGLArea::on_legacy_event(const Glib::RefPtr<const Gdk::Event>& event) {
//     if (event.get()->get_event_type() == Gdk::Event::Type::SCROLL) {
//         ;
//     }
//     return false;
// }

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
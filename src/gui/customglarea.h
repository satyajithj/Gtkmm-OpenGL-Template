#pragma once

#include <memory>

#include <gtkmm-4.0/gtkmm/glarea.h>

#include <gtkmm-4.0/gtkmm/eventcontrollermotion.h>
#include <gtkmm-4.0/gtkmm/eventcontrollerscroll.h>
#include <gtkmm-4.0/gtkmm/eventcontrollerlegacy.h>

#include "enums.h"
#include "common/basetype.h"
#include "common/appstate.h"

#include "shaders/shadermngr.h"
#include "cameras/cameramanager.h"
#include "objects/crate.h"

class CustomGLArea : public Gtk::GLArea, BaseType {

public:
    CustomGLArea(bool p_EnableDepth, std::shared_ptr<AppState> p_AppState);
    ~CustomGLArea();

    void set_projection(ProjectionType);

    // GUI initiated procedures
    void cameraReset();

private:

    float m_ProjectionWidth;
    float m_ProjectionHeight;
    float m_AspectRatio;

    ProjectionType m_CurrentProjectionType;

    bool m_EnableDepth;
    unsigned int m_GLClearFlag;

    // Event controllers
    Glib::RefPtr<Gtk::EventControllerMotion> m_MotionController;
    Glib::RefPtr<Gtk::EventControllerScroll> m_ScrollController;
    // Glib::RefPtr<Gtk::EventControllerLegacy> legacyController;

    std::unique_ptr<ShaderMngr> m_Shader;
    std::unique_ptr<CameraManager> m_CameraManager;

    std::unique_ptr<Crate> m_Crate;

    // GLArea callbacks
    void realize();
    void unrealize();
    bool render(const Glib::RefPtr<Gdk::GLContext>& context);

    void on_area_resize(int, int);

    // Motion controller callbacks
    void on_pointer_enter(double x, double y);
    void on_pointer_motion(double x, double y);
    void on_pointer_leave();

    // Scroll controller callbacks
    bool on_scroll(double dx, double dy);
    void on_scroll_begin();
    void on_scroll_end();

    // Transformations
    void viewport2Scene(double x, double y);

    // // Legacy event controller callback
    // bool on_legacy_event(const Glib::RefPtr<const Gdk::Event>& event);
};
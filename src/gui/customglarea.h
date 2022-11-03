#pragma once

#include <memory>

#include <gtkmm-3.0/gtkmm/glarea.h>

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

    std::unique_ptr<ShaderMngr> m_Shader;
    std::unique_ptr<CameraManager> m_CameraManager;

    std::unique_ptr<Crate> m_Crate;

    // GLArea callbacks
    void realize();
    void unrealize();
    bool render(const Glib::RefPtr<Gdk::GLContext>& context);

    void on_area_resize(int, int);

    // Pointer enter & leave callbacks
    bool on_pointer_enter_notify_event(GdkEventCrossing*);
    bool on_pointer_leave_notify_event(GdkEventCrossing*);

    // Scroll controller callbacks ( Also handles pointer motion for now -> Scale )
    bool on_scroll_event(GdkEventScroll*);

    // Transformations
    void viewport2Scene(double x, double y);
};
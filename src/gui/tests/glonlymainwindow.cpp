#include "glonlymainwindow.h"

#include <sigc++/sigc++.h>

#include <gtkmm-4.0/gtkmm/eventcontrollerkey.h>

GLOnlyMainWindow::GLOnlyMainWindow() {

    // Init app state object (AppState)
    _AppState = std::make_shared<AppState>();

    set_title("GL only window");

    m_VBox.set_margin(12);
    m_VBox.set_spacing(6);
    set_child(m_VBox);

    // Boolean parameter enables/disables the depth buffer
    m_CGLArea = new CustomGLArea(true, _AppState);
    m_VBox.append(*m_CGLArea);

    // Events.
    auto controller = Gtk::EventControllerKey::create();
    controller->signal_key_pressed().connect(sigc::mem_fun(*this, &GLOnlyMainWindow::on_window_key_pressed), true);
    add_controller(controller);
}

GLOnlyMainWindow::~GLOnlyMainWindow() {
    delete m_CGLArea;
}

bool GLOnlyMainWindow::on_window_key_pressed(guint keyval, guint, Gdk::ModifierType state) {
    if(keyval == GDK_KEY_Escape) {
        hide();
        return true;
    } else if (keyval == GDK_KEY_space) {
        m_CGLArea->queue_render();
        return true;
    }

    // Event not handled
    return false;
}
#pragma once

#include <gtkmm-3.0/gtkmm/window.h>
#include <gtkmm-3.0/gtkmm/box.h>

#include "common/basetype.h"
#include "gui/customglarea.h"

class GLOnlyMainWindow : public Gtk::Window, public BaseType {

public:
    GLOnlyMainWindow();
    ~GLOnlyMainWindow() override;

private:

    Gtk::Box m_VBox {Gtk::ORIENTATION_VERTICAL, false};
    std::unique_ptr<CustomGLArea> m_CGLArea;

    bool on_key_press_event(GdkEventKey* keyEvent);
};
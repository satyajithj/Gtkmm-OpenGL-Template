#pragma once

#include <gtkmm/window.h>
#include <gtkmm/box.h>

#include "common/basetype.h"
#include "gui/customglarea.h"

class GLOnlyMainWindow : public Gtk::Window, public BaseType {

public:
    GLOnlyMainWindow();
    ~GLOnlyMainWindow() override;

private:

    Gtk::Box m_VBox {Gtk::Orientation::VERTICAL, false};
    CustomGLArea* m_CGLArea;

    bool on_window_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
};
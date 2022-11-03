#pragma once

#include <iostream>
#include <memory>

#include <gtkmm-3.0/gtkmm/window.h>
#include <gtkmm-3.0/gtkmm/box.h>
#include <gtkmm-3.0/gtkmm/button.h>
#include <gtkmm-3.0/gtkmm/togglebutton.h>

#include "common/basetype.h"
#include "gui/customglarea.h"

class MainWindow : public Gtk::Window, public BaseType {

public:
    MainWindow();
    ~MainWindow() override;

private:
    Gtk::Box m_HBox {Gtk::ORIENTATION_HORIZONTAL, false};
    Gtk::Box m_VBoxToggles {Gtk::ORIENTATION_VERTICAL, false};

    std::unique_ptr<CustomGLArea> m_CGLArea;

    Gtk::ToggleButton m_OrthographicButton {"Orthographic"};
    Gtk::ToggleButton m_PerspectiveButton {"Perspective"};

    Gtk::Button m_ResetButton {"Reset"};

    bool on_window_state_event(GdkEventWindowState* window_state_event);
    bool on_key_press_event(GdkEventKey* keyEvent);

    void on_orthographic_toggled();
    void on_perspective_toggled();
    void on_reset_clicked();
};
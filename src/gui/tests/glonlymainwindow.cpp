#include "glonlymainwindow.h"

#include <sigc++/sigc++.h>

GLOnlyMainWindow::GLOnlyMainWindow() {

    // Init app state object (AppState)
    _AppState = std::make_shared<AppState>();

    set_title("GL only window");

    m_VBox.set_margin_top(8);
    m_VBox.set_margin_left(8);
    m_VBox.set_margin_bottom(8);
    m_VBox.set_margin_right(8);
    m_VBox.set_spacing(8);
    add(m_VBox);

    // Boolean parameter enables/disables the depth buffer
    m_CGLArea = std::make_unique<CustomGLArea>(true, _AppState);
    m_VBox.pack_start(*m_CGLArea);

    // Event controllers & Signals
    signal_key_press_event().connect(sigc::mem_fun(*this, &GLOnlyMainWindow::on_key_press_event), true);
}

GLOnlyMainWindow::~GLOnlyMainWindow() { }

bool GLOnlyMainWindow::on_key_press_event(GdkEventKey* keyEvent) {
    // Handle key presses here ----

    switch (keyEvent->keyval) {

        case GDK_KEY_Escape:
            // Toggle fullscreen
            hide();
            return true;

        case GDK_KEY_space:
            m_CGLArea->queue_render();
            return true;

        default:
            break;
    }

    // Event not handled (propagates further?)
    return false;
}
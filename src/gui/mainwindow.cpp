#include "mainwindow.h"

#include <sigc++/sigc++.h>

#include "enums.h"

MainWindow::MainWindow() {

    // Init app state object (AppState)
    _AppState = std::make_shared<AppState>();

    set_title("GL area GTK template");

    m_HBox.set_margin_top(8);
    m_HBox.set_margin_left(8);
    m_HBox.set_margin_bottom(8);
    m_HBox.set_margin_right(8);
    m_HBox.set_spacing(8);
    add(m_HBox);

    // Boolean parameter enables/disables the depth buffer
    m_CGLArea = std::make_unique<CustomGLArea>(true, _AppState);

    // Vertical box with buttons
    m_VBoxToggles.set_margin_top(8);
    m_VBoxToggles.set_margin_left(8);
    m_VBoxToggles.set_margin_bottom(8);
    m_VBoxToggles.set_margin_right(8);
    m_VBoxToggles.set_spacing(8);
    m_VBoxToggles.set_size_request(160, -1);
    // m_VBoxToggles.compute_expand(Gtk::Orientation::VERTICAL);

    m_OrthographicButton.signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::on_orthographic_toggled), true);
    m_VBoxToggles.pack_start(m_OrthographicButton);

    m_PerspectiveButton.signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::on_perspective_toggled), true);
    m_VBoxToggles.pack_start(m_PerspectiveButton);

    m_ResetButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_reset_clicked), true);
    m_VBoxToggles.pack_start(m_ResetButton);

    // Add everything to the main box
    m_HBox.pack_start(*m_CGLArea);
    m_HBox.pack_start(m_VBoxToggles);

    // Event controllers & Signals
    signal_window_state_event().connect(sigc::mem_fun(*this, &MainWindow::on_window_state_event), true);
    signal_key_press_event().connect(sigc::mem_fun(*this, &MainWindow::on_key_press_event), true);

    // Set widget states
    m_OrthographicButton.set_active(true);

    show_all();

    std::cout << "(AppState) Shared pointer ref count: " << _AppState.use_count() << std::endl;
}

MainWindow::~MainWindow() { }

bool MainWindow::on_window_state_event(GdkEventWindowState* windowStateEvent) {
    // Check the fullscreen state
    _AppState->isFullscreen = windowStateEvent->new_window_state & GDK_WINDOW_STATE_FULLSCREEN;

    return false;
}

bool MainWindow::on_key_press_event(GdkEventKey* keyEvent) {
    // Handle key presses here ----

    switch (keyEvent->keyval) {

        case GDK_KEY_F11:
            // Toggle fullscreen
            _AppState->isFullscreen ? unfullscreen() : fullscreen();
            return true;
        default:
            break;
    }

    // Event not handled (propagates further?)
    return false;
}

void MainWindow::on_orthographic_toggled() {
    if (m_OrthographicButton.get_active()) {
        m_PerspectiveButton.set_active(false);
        m_CGLArea->set_projection(ProjectionType::ORTHOGRAPHIC);
        return;
    }

    if (!m_PerspectiveButton.get_active()) {
        m_OrthographicButton.set_active(true);
    }
}

void MainWindow::on_perspective_toggled() {
    if (m_PerspectiveButton.get_active()) {
        m_OrthographicButton.set_active(false);
        m_CGLArea->set_projection(ProjectionType::PERSPECTIVE);
        return;
    }

    if (!m_OrthographicButton.get_active()) {
        m_PerspectiveButton.set_active(true);
    }
}

void MainWindow::on_reset_clicked() {
    m_CGLArea->cameraReset();
}
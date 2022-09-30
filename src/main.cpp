#include <iostream>

#include <epoxy/gl.h>
#include <gtkmm/application.h>

#include <gui/mainwindow.h>

int main(int argc, char ** argv){
  auto app = Gtk::Application::create("com.placeholderxyz.openglarea");
  return app->make_window_and_run<MainWindow>(argc, argv);
}
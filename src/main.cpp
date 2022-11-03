#include <iostream>

#include <epoxy/gl.h>
#include <gtkmm-3.0/gtkmm/application.h>

#include "gui/mainwindow.h"

int main(int argc, char ** argv){
  auto app = Gtk::Application::create("com.placeholderxyz.openglarea");
  MainWindow mainWindow = MainWindow();

  return app->run(mainWindow, argc, argv);
}
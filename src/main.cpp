#include "window.hpp"
#include <gtkmm.h>

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(APP_ID);

    return app->make_window_and_run<MainWindow>(argc, argv, APP_NAME);
}

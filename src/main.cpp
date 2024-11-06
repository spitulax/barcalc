#include <gtkmm.h>

class MyWindow : public Gtk::Window {
public:
    MyWindow();
};

MyWindow::MyWindow() {
    set_title(APP_NAME);
    set_default_size(600, 800);
    set_resizable(false);
}

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(APP_ID);

    return app->make_window_and_run<MyWindow>(argc, argv);
}

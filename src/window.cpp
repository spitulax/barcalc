#include "window.hpp"
#include <giomm/liststore.h>
#include <glibmm/ustring.h>
#include <gtkmm/button.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/object.h>
#include <iostream>
#include <sigc++/functors/mem_fun.h>

MainWindow::MainWindow(Glib::ustring &&title)
    : m_main_box(Gtk::Orientation::VERTICAL, 8)
    , m_numpad() {
    set_title(title);
    set_resizable(false);
    set_default_size(600, 800);

    m_main_box.set_margin(12);
    m_main_box.set_expand();
    m_main_box.append(m_numpad);
    set_child(m_main_box);

    m_css_provider = Gtk::CssProvider::create();
#if HAS_STYLE_PROVIDER_ADD_PROVIDER_FOR_DISPLAY
    Gtk::StyleProvider::add_provider_for_display(
        get_display(), m_css_provider, GTK_STYLE_PROVIDER_PRIORITY);
#else
    Gtk::StyleContext::add_provider_for_display(
        get_display(), m_css_provider, GTK_STYLE_PROVIDER_PRIORITY);
#endif
    m_css_provider->signal_parsing_error().connect(
        sigc::mem_fun(*this, &MainWindow::on_css_parsing_error));
    m_css_provider->load_from_path(RESOURCES_DIR "/style.css");
};

MainWindow::~MainWindow() {
    //
}

void MainWindow::on_css_parsing_error(const Glib::RefPtr<const Gtk::CssSection> &section,
                                      const Glib::Error                         &error) {
    std::cerr << "Failed to parse CSS: " << error.what() << "\n";
    if (section) {
        const auto file = section->get_file();
        if (file) {
            std::cerr << "\tIn " << file->get_uri();
        }

        auto start_location = section->get_start_location();
        std::cerr << " at " << start_location.get_lines() + 1 << ":"
                  << start_location.get_line_chars() << "\n";
    }
}

#include "window.hpp"
#include <format>
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
    : m_main_box(Gtk::Orientation::VERTICAL, 8) {
    set_title(title);
    set_resizable(false);
    set_default_size(600, 800);

    for (size_t y = 0; y < numpad_height; ++y) {
        for (size_t x = 0; x < numpad_width; ++x) {
            size_t i = x + y * numpad_width;

            std::string label;
            const char *color = "#ffffff";
            bool        cont  = false;
            bool        bold  = true;
            // TODO: turn into a map
            switch (numpad_layout[i]) {
                case '^': {
                    label = "xⁿ";
                } break;
                case 'S': {
                    label = "√";
                } break;
                case 'B': {
                    color = "#ee6565";
                    label = "←";
                } break;
                case '/': {
                    label = "÷";
                } break;
                case '*': {
                    label = "×";
                } break;
                case ' ': {
                    cont = true;
                } break;
                case '=': {
                    color = "#75bfff";
                    label = "=";
                } break;
                case 'C': {
                    color = "#ee6565";
                    label = "C";
                } break;
                case '(':
                case ')':
                case '%':
                case '-':
                case '+': {
                    label = numpad_layout[i];
                    bold  = true;
                } break;
                default: {
                    label = numpad_layout[i];
                    bold  = false;
                }
            }
            if (cont) continue;

            auto button_label = Gtk::make_managed<Gtk::Label>();
            if (bold)
                button_label->set_markup(std::format(
                    "<span color=\"{}\" weight=\"1000\">{}</span>", color, std::move(label)));
            else
                button_label->set_markup(
                    std::format("<span color=\"{}\">{}</span>", color, std::move(label)));
            auto button = Gtk::make_managed<Gtk::Button>();
            button->set_child(*button_label);
            button->set_expand();
            button->signal_clicked().connect(
                sigc::bind(sigc::mem_fun(*this, &MainWindow::on_numpad_clicked),
                           Glib::ustring(1, numpad_layout[i])),
                false);
            m_numpad_grid.attach(*button, int(x), int(y), (numpad_layout[i] == '=') ? 2 : 1);
        }
    }
    m_numpad_grid.set_expand();

    m_main_box.set_margin(12);
    m_main_box.set_expand();
    m_main_box.append(m_numpad_grid);
    set_child(m_main_box);

    m_css_provider = Gtk::CssProvider::create();
#if HAS_STYLE_PROVIDER_ADD_PROVIDER_FOR_DISPLAY
    Gtk::StyleProvider::add_provider_for_display(
        get_display(), m_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
#else
    Gtk::StyleContext::add_provider_for_display(
        get_display(), m_css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
#endif
    m_css_provider->signal_parsing_error().connect(
        sigc::mem_fun(*this, &MainWindow::on_css_parsing_error));
    m_css_provider->load_from_path(RESOURCES_DIR "/style.css");
};

MainWindow::~MainWindow() {
    //
}

void MainWindow::on_numpad_clicked(const Glib::ustring &id) {
    std::cout << id << "\n";
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

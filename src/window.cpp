#include "window.hpp"
#include "numpad.hpp"
#include "utils.hpp"
#include <giomm/liststore.h>
#include <glibmm/ustring.h>
#include <gtkmm/button.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/enums.h>
#include <gtkmm/label.h>
#include <gtkmm/object.h>
#include <iostream>
#include <sigc++/functors/mem_fun.h>

extern unsigned char ___resources_style_css[];
extern unsigned int  ___resources_style_css_len;

MainWindow::MainWindow(Glib::ustring &&title)
    : m_main_box(Gtk::Orientation::VERTICAL, 8)
    , m_numpad()
    , m_entry() {
    set_title(title);
    set_resizable(false);
    set_default_size(600, 800);

    m_numpad.signal_clicked.connect(sigc::mem_fun(*this, &MainWindow::on_numpad_clicked));
    m_entry.signal_eval_time.connect(sigc::mem_fun(*this, &MainWindow::on_eval_time));

    m_main_box.set_margin(12);
    m_main_box.set_expand();
    m_main_box.append(m_entry);
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

#ifdef DEBUG
    m_css_provider->load_from_path(RESOURCES_DIR "/style.css");
#else
    m_css_provider->load_from_string(
        std::string(reinterpret_cast<char *>(___resources_style_css), ___resources_style_css_len));
#endif
};

MainWindow::~MainWindow() {
    //
}

void MainWindow::on_numpad_clicked(const NumpadButton &button) {
    m_entry.register_numpad(button);
}

void MainWindow::on_eval_time(const Glib::ustring &str) {
    const auto tokens_wrapped = Eval::parse(str);
    if (!tokens_wrapped) {
        std::cerr << "Failed to parse string: " << str << "\n";
        return;
    }
    const auto tokens = tokens_wrapped.value();

    const auto rpn_wrapped = Eval::shunting_yard(tokens);
    if (!rpn_wrapped) {
        std::cerr << "Failed to run Shunting Yard algorithm to token: ";
        print_range(tokens, [](const Token &x) {
            return '"' + x.str + '"';
        });
        return;
    }
    const auto rpn = rpn_wrapped.value();
    print_range(rpn, &decltype(rpn)::value_type::str);

    const auto result_wrapped = Eval::solve(rpn);
    if (!result_wrapped) {
        std::cerr << "Failed to solve expression\n";
        return;
    }
    const double result = result_wrapped.value();

    std::cout << "Result: " << result << "\n";
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

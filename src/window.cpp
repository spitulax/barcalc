#include "window.hpp"
#include "eval.hpp"
#include "numpad.hpp"
#include "utils.hpp"
#include <cmath>
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

extern unsigned char ___resources_style_css[];
extern unsigned int  ___resources_style_css_len;

MainWindow::MainWindow(Glib::ustring &&title)
    : m_main_box(Gtk::Orientation::VERTICAL, 8)
    , m_history()
    , m_numpad(m_history)
    , m_entry() {
    set_title(title);
    set_resizable(false);
    set_default_size(600, 800);

    m_entry.set_text("Enter an expression");

    m_history.signal_selected_history.connect(
        sigc::mem_fun(*this, &MainWindow::on_selected_history));
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

bool MainWindow::on_close_request() {
    m_history.serialise_expressions();
    return Gtk::ApplicationWindow::on_close_request();
};

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

    const auto result_wrapped = Eval::solve(rpn);
    if (!result_wrapped) {
        std::cerr << "Failed to solve expression\n";
        return;
    }
    const double result = result_wrapped.value();

    m_entry.continue_typing = false;
    std::string result_str;
    if (std::isnan(result)) {
        result_str = "NaN";
    } else if (std::isinf(result)) {
        result_str = "∞";
    } else {
        result_str = std::format("{:f}", result);
        result_str.erase(result_str.find_last_not_of('0') + 1, std::string::npos);
        if (result_str.back() == '.') {
            result_str.pop_back();
        }
        m_entry.continue_typing = true;
    }
    m_entry.set_text(result_str);

    m_history.append_expression(str);
}

void MainWindow::on_selected_history(const Glib::ustring &expression) {
    m_entry.set_text(expression);
    m_entry.continue_typing = true;
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

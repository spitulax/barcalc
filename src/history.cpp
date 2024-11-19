#include "history.hpp"
#include <cassert>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <giomm/simpleactiongroup.h>
#include <gtkmm/label.h>
#include <gtkmm/object.h>
#include <gtkmm/popovermenu.h>
#include <iostream>
#include <sigc++/functors/mem_fun.h>
#include <string>

static constexpr std::string history_filename = "barcalc.hist";

History::History()
    : m_button()
    , m_menu()
    , m_model(Gio::Menu::create()) {
#ifdef __unix__
    std::string data_home = std::getenv("XDG_DATA_HOME");
    if (data_home.length() <= 0) {
        const std::string home_dir = std::getenv("HOME");
        assert(home_dir.length() > 0);
        data_home = "{}/.local/share" + home_dir;
    }
    m_history_filepath = data_home;
    m_history_filepath /= history_filename;
#else
    history_filepath = std::filesystem::path();
#endif

    if (m_history_filepath.empty()) return;

    if (std::filesystem::exists(m_history_filepath) &&
        !std::filesystem::is_regular_file(m_history_filepath)) {
        std::cerr << "`" << m_history_filepath << "` already exists and it's not a regular file\n";
        m_history_filepath.clear();
        return;
    }

    deserialise_expressions();

    m_button.signal_clicked().connect(sigc::mem_fun(*this, &History::on_button_clicked));

    m_model   = Gio::Menu::create();
    m_actions = Gio::SimpleActionGroup::create();
    m_actions->add_action_with_parameter(
        "select", Glib::VariantType("s"), sigc::mem_fun(*this, &History::on_history_selected));
    m_actions->add_action("clear", sigc::mem_fun(*this, &History::on_history_cleared));
    m_button.insert_action_group("history", m_actions);
    m_model->append("CLEAR", "history.clear");
    for (const auto &x : m_expressions)
        m_model->append(x, "history.select::" + x);

    m_menu.set_has_arrow(false);
    m_menu.set_offset(0, 16);
    m_menu.set_menu_model(m_model);
    m_menu.set_parent(m_button);
}

History::~History() {
    m_menu.unparent();
}

Gtk::Button *History::button() {
    return &m_button;
}

void History::append_expression(Glib::ustring expression) {
    m_expressions.push_back(std::move(expression));
    if (m_expressions.size() >= 2 && *(m_expressions.end() - 2) == m_expressions.back()) return;
    m_model->append(m_expressions.back(), "history.select::" + m_expressions.back());
}

void History::serialise_expressions() const {
    if (m_history_filepath.empty()) return;
    std::ofstream file(m_history_filepath, std::ios::out | std::ios::trunc);
    for (const auto &x : m_expressions)
        file << x << "\n";
}

void History::deserialise_expressions() {
    if (m_history_filepath.empty() || !std::filesystem::exists(m_history_filepath)) return;
    std::ifstream file(m_history_filepath);
    std::string   expression;
    while (std::getline(file, expression))
        m_expressions.push_back(expression);
}

void History::on_button_clicked() {
    m_menu.popup();
}

void History::on_history_selected(const Glib::VariantBase &parameter) {
    auto expression =
        Glib::VariantBase::cast_dynamic<Glib::Variant<Glib::ustring>>(parameter).get();
    signal_selected_history.emit(expression);
}

void History::on_history_cleared() {
    m_expressions.clear();
    while (m_model->get_n_items() > 1)
        m_model->remove(m_model->get_n_items() - 1);
}

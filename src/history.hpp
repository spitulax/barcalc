#pragma once

#include <filesystem>
#include <giomm/actionmap.h>
#include <giomm/menu.h>
#include <giomm/simpleactiongroup.h>
#include <glibmm/refptr.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/popovermenu.h>
#include <sigc++/signal.h>
#include <vector>

class History {
public:
    History();
    virtual ~History();

    Gtk::Button *button();

    void append_expression(Glib::ustring expression);
    void serialise_expressions() const;

    using type_signal_selected_history = sigc::signal<void(const Glib::ustring &expression)>;
    type_signal_selected_history signal_selected_history;

private:
    std::filesystem::path      m_history_filepath;
    std::vector<Glib::ustring> m_expressions;

    Gtk::Button                          m_button;
    Gtk::PopoverMenu                     m_menu;
    Glib::RefPtr<Gio::Menu>              m_model;
    Glib::RefPtr<Gio::SimpleActionGroup> m_actions;

    void deserialise_expressions();

    void on_button_clicked();
    void on_history_selected(const Glib::VariantBase &parameter);
    void on_history_cleared();
};

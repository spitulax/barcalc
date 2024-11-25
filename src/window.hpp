#pragma once

#include "entry.hpp"
#include "history.hpp"
#include "numpad.hpp"
#include <glibmm/error.h>
#include <glibmm/refptr.h>
#include <glibmm/ustring.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/entry.h>
#include <gtkmm/enums.h>
#include <gtkmm/grid.h>
#include <gtkmm/gridview.h>
#include <gtkmm/listitem.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/selectionmodel.h>
#include <gtkmm/singleselection.h>
#include <gtkmm/styleprovider.h>

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow(Glib::ustring &&title);
    virtual ~MainWindow();

private:
    Gtk::Box m_main_box;

    History                        m_history;
    Numpad                         m_numpad;
    Entry                          m_entry;
    Glib::RefPtr<Gtk::CssProvider> m_css_provider;

    void on_numpad_clicked(const NumpadButton &button);
    void on_eval_time(const Glib::ustring &str);
    void on_selected_history(const Glib::ustring &expression);
    void on_css_parsing_error(const Glib::RefPtr<const Gtk::CssSection> &section,
                              const Glib::Error                         &error);

    bool on_close_request() override;
};

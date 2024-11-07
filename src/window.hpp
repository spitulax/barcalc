#pragma once

#include "numpad.hpp"
#include <glibmm/error.h>
#include <glibmm/refptr.h>
#include <glibmm/ustring.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/enums.h>
#include <gtkmm/grid.h>
#include <gtkmm/gridview.h>
#include <gtkmm/listitem.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/selectionmodel.h>
#include <gtkmm/singleselection.h>
#include <gtkmm/styleprovider.h>

#ifdef DEBUG
#define GTK_STYLE_PROVIDER_PRIORITY GTK_STYLE_PROVIDER_PRIORITY_USER
#else
#define GTK_STYLE_PROVIDER_PRIORITY GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
#endif

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow(Glib::ustring &&title);
    virtual ~MainWindow();

protected:
    Gtk::Box m_main_box;

    Numpad                         m_numpad;
    Glib::RefPtr<Gtk::CssProvider> m_css_provider;

    void on_css_parsing_error(const Glib::RefPtr<const Gtk::CssSection> &section,
                              const Glib::Error                         &error);
};
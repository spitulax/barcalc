#pragma once

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

class MainWindow : public Gtk::ApplicationWindow {
public:
    MainWindow(Glib::ustring &&title);
    virtual ~MainWindow();

protected:
    Gtk::Box m_main_box;

    static constexpr size_t                                         numpad_width  = 4;
    static constexpr size_t                                         numpad_height = 6;
    static constexpr std::array<char, numpad_width * numpad_height> numpad_layout{
        '^', 'S', '(', ')',    //
        'C', 'B', '%', '/',    //
        '7', '8', '9', '*',    //
        '4', '5', '6', '-',    //
        '1', '2', '3', '+',    //
        '0', '.', '=', ' ',    // '=' takes up 2 spaces horizontally
    };
    Gtk::Grid                      m_numpad_grid;
    Glib::RefPtr<Gtk::CssProvider> m_css_provider;

    void on_numpad_clicked(const Glib::ustring &label);

    void on_css_parsing_error(const Glib::RefPtr<const Gtk::CssSection> &section,
                              const Glib::Error                         &error);
};

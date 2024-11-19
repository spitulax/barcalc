#pragma once

#include "history.hpp"
#include "symbols.hpp"
#include <glibmm/ustring.h>
#include <gtkmm/grid.h>
#include <sigc++/signal.h>

struct NumpadButton {
    ButtonKind    kind;
    Glib::ustring label     = "";
    bool          bold      = false;
    const char   *color     = "#ffffff";
    int           width     = 1;
    Glib::ustring css_class = "";
};

class Numpad : public Gtk::Grid {
public:
    Numpad(History &history);
    virtual ~Numpad();

    using type_signal_clicked = sigc::signal<void(const NumpadButton &)>;
    type_signal_clicked signal_clicked;

private:
    void on_numpad_clicked(const NumpadButton &button);
};

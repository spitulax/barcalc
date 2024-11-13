#pragma once

#include "symbols.hpp"
#include <glibmm/ustring.h>
#include <gtkmm/grid.h>
#include <sigc++/signal.h>

struct NumpadButton {
    ButtonKind                         kind;
    std::optional<const Glib::ustring> label     = std::nullopt;
    bool                               bold      = false;
    const char                        *color     = "#ffffff";
    int                                width     = 1;
    std::optional<const Glib::ustring> css_class = std::nullopt;
};

class Numpad : public Gtk::Grid {
public:
    Numpad();
    virtual ~Numpad();

    using type_signal_clicked = sigc::signal<void(const NumpadButton &)>;
    type_signal_clicked signal_clicked;

protected:
    void on_numpad_clicked(const NumpadButton &button);
};

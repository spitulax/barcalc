#pragma once

#include "numpad.hpp"
#include <glibmm/ustring.h>
#include <gtkmm/entry.h>
#include <sigc++/signal.h>

class Entry : public Gtk::Entry {
public:
    Entry();
    virtual ~Entry();

    bool continue_typing = false;

    void register_numpad(const NumpadButton &button);

    using type_signal_eval_time = sigc::signal<void(const Glib::ustring &str)>;
    type_signal_eval_time signal_eval_time;

private:
    void insert_text(const Glib::ustring &text);

    void on_eval_time(const Glib::ustring &str);
};

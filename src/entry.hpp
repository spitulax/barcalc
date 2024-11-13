#pragma once

#include "numpad.hpp"
#include <glibmm/ustring.h>
#include <gtkmm/entry.h>

class Entry : public Gtk::Entry {
public:
    Entry();
    virtual ~Entry();

    void register_numpad(const NumpadButton &button);

private:
    void insert_text(const Glib::ustring &text);
};

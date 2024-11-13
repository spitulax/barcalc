#include "entry.hpp"
#include "numpad.hpp"
#include "symbols.hpp"
#include <cassert>
#include <cmath>
#include <gtkmm/enums.h>
#include <sys/types.h>

Entry::Entry() {
    set_editable(false);
    set_alignment(Gtk::Align::END);
    set_margin(12);
    set_enable_undo(false);
}

Entry::~Entry() {
    //
}

void Entry::register_numpad(const NumpadButton &button) {
    set_position(-1);

    auto buf = get_buffer();
    switch (button.kind) {
        case ButtonKind::NUMBERS: {
            insert_text(button.label.value());
        } break;
        case ButtonKind::PERIOD: {
            insert_text(".");
        } break;
        case ButtonKind::BACKSPACE: {
            buf->delete_text(buf->get_length() - 1, 1);
        } break;
        case ButtonKind::CLEAR: {
            buf->set_text("");
        } break;
        case ButtonKind::EQUALS: {
            assert(false && "unimplemented");
        } break;
        default: {
            insert_text(symbols_str.at(Symbols(button.kind)));
        };
    }
}

void Entry::insert_text(const Glib::ustring &text) {
    auto buf = get_buffer();
    buf->insert_text(buf->get_length(), text);
}

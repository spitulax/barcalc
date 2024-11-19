#include "entry.hpp"
#include "numpad.hpp"
#include "symbols.hpp"
#include <cassert>
#include <cmath>
#include <glibmm/ustring.h>
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
    auto buf = get_buffer();

    if (!continue_typing) {
        buf->set_text("");
        continue_typing = true;
    }

    switch (button.kind) {
        case ButtonKind::NUMBERS: {
            insert_text(button.label);
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
            on_eval_time(buf->get_text());
        } break;
        default: {
            Glib::ustring str;
            str += symbols_str.at(Symbols(button.kind));
            insert_text(std::move(str));
        };
    }

    set_position(-1);
}

void Entry::insert_text(const Glib::ustring &text) {
    auto buf = get_buffer();
    buf->insert_text(buf->get_length(), text);
}

void Entry::on_eval_time(const Glib::ustring &str) {
    signal_eval_time.emit(str);
}

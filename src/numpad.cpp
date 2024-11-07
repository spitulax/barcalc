#include "numpad.hpp"
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/object.h>
#include <iostream>
#include <sstream>
#include <string>

Numpad::Numpad() {
    set_expand();

    for (size_t y = 0; y < numpad_height; ++y) {
        for (size_t x = 0; x < numpad_width; ++x) {
            size_t              i      = x + y * numpad_width;
            const NumpadButton &button = numpad_layout[i];
            if (button.id == ButtonId::EMPTY) continue;

            std::string       label;
            auto              button_label = Gtk::make_managed<Gtk::Label>();
            std::stringstream label_format;
            label_format << "<span color=\"" << button.color << "\"";
            if (button.bold) label_format << " weight=\"1000\"";
            label_format << ">" << button.label.value() << "</span>";
            button_label->set_markup(label_format.str());

            auto gtk_button = Gtk::make_managed<Gtk::Button>();
            gtk_button->set_child(*button_label);
            gtk_button->set_expand();
            if (button.css_class) gtk_button->add_css_class(button.css_class.value());
            gtk_button->signal_clicked().connect(
                sigc::bind(sigc::mem_fun(*this, &Numpad::on_numpad_clicked), button), false);
            attach(*gtk_button, int(x), int(y), button.width);
        }
    }
}

Numpad::~Numpad() {
    //
}

void Numpad::on_numpad_clicked(const NumpadButton &button) {
    std::cout << button.label.value() << "\n";
}

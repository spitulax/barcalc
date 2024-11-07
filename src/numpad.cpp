#include "numpad.hpp"
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/object.h>
#include <sstream>
#include <string>

static constexpr size_t                                             numpad_width  = 4;
static constexpr size_t                                             numpad_height = 6;
static const std::array<NumpadButton, numpad_width * numpad_height> numpad_layout{
    {
     { ButtonKind::POWER, "xⁿ", true },
     { ButtonKind::SQRT, "√", true },
     { ButtonKind::LPAREN, "(", true },
     { ButtonKind::RPAREN, ")", true },
     { ButtonKind::CLEAR, "C", true, "#ee6565", 1, "delete" },
     { ButtonKind::BACKSPACE, "←", true, "#ee6565", 1, "delete" },
     { ButtonKind::PERCENT, "%", true },
     { ButtonKind::DIVIDE, "÷", true },
     { ButtonKind::NUMBERS, "7" },
     { ButtonKind::NUMBERS, "8" },
     { ButtonKind::NUMBERS, "9" },
     { ButtonKind::MULTIPLY, "×", true },
     { ButtonKind::NUMBERS, "4" },
     { ButtonKind::NUMBERS, "5" },
     { ButtonKind::NUMBERS, "6" },
     { ButtonKind::MINUS, "-", true },
     { ButtonKind::NUMBERS, "1" },
     { ButtonKind::NUMBERS, "2" },
     { ButtonKind::NUMBERS, "3" },
     { ButtonKind::PLUS, "+", true },
     { ButtonKind::NUMBERS, "0" },
     { ButtonKind::PERIOD, "." },
     { ButtonKind::EQUALS, "=", true, "#75bfff", 2, "equals" },
     { ButtonKind::EMPTY },
     }
};

Numpad::Numpad() {
    set_expand();

    for (size_t y = 0; y < numpad_height; ++y) {
        for (size_t x = 0; x < numpad_width; ++x) {
            size_t              i      = x + y * numpad_width;
            const NumpadButton &button = numpad_layout[i];
            if (button.kind == ButtonKind::EMPTY) continue;

            std::string        label;
            auto               button_label = Gtk::make_managed<Gtk::Label>();
            std::ostringstream label_format;
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
    signal_clicked.emit(button);
}

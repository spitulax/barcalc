#pragma once

#include <array>
#include <glibmm/ustring.h>
#include <gtkmm/grid.h>

class Numpad : public Gtk::Grid {
protected:
    enum class ButtonId {
        NUMBERS,
        PERIOD,
        EQUALS,
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,
        PERCENT,
        BACKSPACE,
        CLEAR,
        LPAREN,
        RPAREN,
        POWER,
        SQRT,
        EMPTY,
    };

    struct NumpadButton {
        ButtonId                           id;
        std::optional<const Glib::ustring> label     = std::nullopt;
        bool                               bold      = false;
        const char                        *color     = "#ffffff";
        int                                width     = 1;
        std::optional<const Glib::ustring> css_class = std::nullopt;
    };

    static constexpr size_t numpad_width  = 4;
    static constexpr size_t numpad_height = 6;
    // TODO: Add two sides
    const std::array<NumpadButton, numpad_width * numpad_height> numpad_layout = {
        {
         { ButtonId::POWER, "xⁿ", true },
         { ButtonId::SQRT, "√", true },
         { ButtonId::LPAREN, "(", true },
         { ButtonId::RPAREN, ")", true },
         { ButtonId::CLEAR, "C", true, "#ee6565", 1, "delete" },
         { ButtonId::BACKSPACE, "←", true, "#ee6565", 1, "delete" },
         { ButtonId::PERCENT, "%", true },
         { ButtonId::DIVIDE, "÷", true },
         { ButtonId::NUMBERS, "7" },
         { ButtonId::NUMBERS, "8" },
         { ButtonId::NUMBERS, "9" },
         { ButtonId::MULTIPLY, "×", true },
         { ButtonId::NUMBERS, "4" },
         { ButtonId::NUMBERS, "5" },
         { ButtonId::NUMBERS, "6" },
         { ButtonId::MINUS, "-", true },
         { ButtonId::NUMBERS, "1" },
         { ButtonId::NUMBERS, "2" },
         { ButtonId::NUMBERS, "3" },
         { ButtonId::PLUS, "+", true },
         { ButtonId::NUMBERS, "0" },
         { ButtonId::PERIOD, "." },
         { ButtonId::EQUALS, "=", true, "#75bfff", 2, "equals" },
         { ButtonId::EMPTY },
         }
    };

public:
    Numpad();
    virtual ~Numpad();

protected:
    void on_numpad_clicked(const NumpadButton &button);
};

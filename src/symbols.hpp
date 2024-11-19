#pragma once

#include <glib.h>
#include <glibmm/ustring.h>
#include <unordered_map>
#include <utility>

enum class Symbols {
    UNKNOWN,
    LPAREN = 1 << 8,
    RPAREN,
    ADD,
    SUB,
    MUL,
    DIV,
    POWER,
    SQRT,
};

enum class ButtonKind {
    EMPTY,
    NUMBERS,
    PERIOD,
    EQUALS,
    BACKSPACE,
    CLEAR,
    HISTORY,
    LPAREN = 1 << 8,    // Corresponds to `Symbols`
    RPAREN,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    POWER,
    SQRT,
};

enum class Operators {
    UNKNOWN,
    ADD = (1 << 8) + 2,    // Corresponds to `Symbols`
    SUB,
    MUL,
    DIV,
    POWER,
    SQRT,
};

static const std::unordered_map<Symbols, char32_t> symbols_str{
    { Symbols::LPAREN, U'('   },
    { Symbols::RPAREN, U')'   },
    { Symbols::ADD,    U'+'   },
    { Symbols::SUB,    U'-'   },
    { Symbols::MUL,    U'×'  },
    { Symbols::DIV,    U'÷'  },
    { Symbols::POWER,  U'^'   },
    { Symbols::SQRT,   U'√' },
};

static inline std::pair<bool, Symbols> find_symbols_str(char32_t ch) {
    for (const auto &[k, v] : symbols_str) {
        if (ch == v) {
            return std::make_pair(true, k);
        }
    }
    return std::make_pair(false, Symbols::UNKNOWN);
}

#pragma once

#include <unordered_map>

enum class Symbols {
    LPAREN = 1 << 8,
    RPAREN,
    ADD,
    SUB,
    MUL,
    DIV,
    PERCENT,
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
    LPAREN = 1 << 8,    // Corresponds to `Symbols`
    RPAREN,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    PERCENT,
    POWER,
    SQRT,
};

enum class Operators {
    ADD = (1 << 8) + 2,    // Corresponds to `Symbols`
    SUB,
    MUL,
    DIV,
    PERCENT,
    POWER,
    SQRT,
};

static const std::unordered_map<Symbols, const char *> symbols_str{
    { Symbols::LPAREN,  "("   },
    { Symbols::RPAREN,  ")"   },
    { Symbols::ADD,     "+"   },
    { Symbols::SUB,     "-"   },
    { Symbols::MUL,     "×"  },
    { Symbols::DIV,     "÷"  },
    { Symbols::PERCENT, "%"   },
    { Symbols::POWER,   "^"   },
    { Symbols::SQRT,    "√" },
};

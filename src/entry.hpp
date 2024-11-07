#pragma once

#include "numpad.hpp"
#include <cstddef>
#include <gtkmm/entry.h>
#include <optional>
#include <sstream>
#include <vector>

enum class TokenKind {
    NUMBERS,
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

struct Token {
    TokenKind             kind;
    std::optional<double> num = std::nullopt;

    Token(TokenKind t_kind, std::optional<double> t_num)
        : kind(t_kind)
        , num(t_num) {
    }
};

class Entry : public Gtk::Entry {
public:
    Entry();
    virtual ~Entry();

    void register_numpad(const NumpadButton &button);

private:
    std::ostringstream num_buf;
    size_t             num_buf_len;
    std::ostringstream frac_buf;
    size_t             frac_buf_len;
    std::vector<Token> tokens;
    bool               fraction;

    void        add_token(TokenKind kind);
    void        resize_buf(size_t n, bool frac);
    void        tokenise_buf();
    std::string print_tokens();
};

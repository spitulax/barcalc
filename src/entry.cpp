#include "entry.hpp"
#include "numpad.hpp"
#include <cassert>
#include <cmath>
#include <exception>
#include <format>
#include <ios>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

static constexpr int max_digit = 15;

Entry::Entry()
    : num_buf_len(0)
    , frac_buf_len(0)
    , fraction(false) {
    set_editable(false);
    set_alignment(Gtk::Align::END);
    set_margin(12);
}

Entry::~Entry() {
    //
}

void Entry::add_token(TokenKind kind) {
    // TODO: handle error
    tokens.emplace_back(kind, std::nullopt);
}

void Entry::resize_buf(size_t n, bool frac) {
    std::ostringstream *buf;
    size_t              len;
    if (!frac) {
        buf         = &num_buf;
        len         = num_buf_len;
        num_buf_len = n;
    } else {
        buf          = &frac_buf;
        len          = frac_buf_len;
        frac_buf_len = n;
        if (n == 0) fraction = false;
    }
    if (n == 0) {
        buf->str("");
    } else {
        buf->str(std::move(*buf).str().substr(0, len + (n - len)));
    }
}

void Entry::tokenise_buf() {
    resize_buf(max_digit, false);
    resize_buf(max_digit, true);

    try {
        tokens.emplace_back(
            TokenKind::NUMBERS,
            std::stold(std::format("{}.{}", std::move(num_buf).str(), std::move(frac_buf).str())));
    } catch (const std::invalid_argument &e) {
        assert(false && "This should never happen");
    } catch (const std::out_of_range &e) {
        std::cout << "Overflow\n";
    }

    resize_buf(0, false);
    resize_buf(0, true);
}

void Entry::register_numpad(const NumpadButton &button) {
    switch (button.kind) {
        case ButtonKind::NUMBERS: {
            int digit;
            try {
                digit = std::stoi(button.label->c_str());
            } catch (const std::exception &e) {
                assert(false && "This should never happen");
            }
            if (digit < 0 || digit > 9) return;
            if (!fraction) {
                num_buf.seekp(std::streamoff(num_buf_len));
                num_buf << digit;
                ++num_buf_len;
            } else {
                frac_buf.seekp(std::streamoff(frac_buf_len));
                frac_buf << digit;
                ++frac_buf_len;
            }
        } break;
        case ButtonKind::PERIOD: {
            fraction = true;
        } break;
        case ButtonKind::EQUALS: {
            assert(false && "Unimplemented");
        } break;
        case ButtonKind::BACKSPACE: {
            if (!fraction)
                resize_buf(num_buf_len - 1, false);
            else
                resize_buf(frac_buf_len - 1, true);
        } break;
        case ButtonKind::CLEAR: {
            resize_buf(0, false);
            resize_buf(0, true);
        } break;
        case ButtonKind::EMPTY: {
            //
        } break;
        default: {
            add_token(static_cast<TokenKind>(button.kind));
        }
    }
    if (static_cast<int>(button.kind) >= 1 << 8 && num_buf_len + frac_buf_len > 0) tokenise_buf();
}

std::string Entry::print_tokens() {
    std::ostringstream ss;
    ss << "[";
    for (const auto &x : tokens) {
        ss << " (" << int(x.kind) << "," << x.num.value_or(0) << ")";
    }
    ss << " ]";
    return std::move(ss).str();
}

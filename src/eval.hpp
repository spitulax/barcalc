#pragma once

#include "symbols.hpp"
#include <deque>
#include <glibmm/ustring.h>
#include <optional>
#include <unordered_map>
#include <vector>

enum class TokenType {
    UNKNOWN,
    NUMERIC,
    OPERATOR,
    LPAREN,
    RPAREN,
};

enum class OperatorArg {
    UNKNOWN,
    BINARY,
    UNARY_PREFIX,
    UNARY_SUFFIX,
};

struct Operator {
    int         precedence = -1;
    OperatorArg arg        = OperatorArg::UNKNOWN;
    Operators   type       = Operators::UNKNOWN;
};

struct Token {
    TokenType     type  = TokenType::UNKNOWN;
    Glib::ustring str   = "";
    double        value = 0.0;
    Operator      op    = {};
};

class Eval {
private:
    enum class TokeniserState {
        NEW_TOKEN,
        NUMERIC,
        LPAREN,
        RPAREN,
        OPERATOR,
        COMPLETE,
        END,
    };

    static const inline std::unordered_map<Operators, int> binary_operators{
        { Operators::ADD,   1 },
        { Operators::SUB,   1 },
        { Operators::MUL,   2 },
        { Operators::DIV,   2 },
        { Operators::POWER, 3 },
    };

    static const inline std::unordered_map<Operators, OperatorArg> unary_operators{
        { Operators::ADD,     OperatorArg::UNARY_PREFIX },
        { Operators::SUB,     OperatorArg::UNARY_PREFIX },
        { Operators::PERCENT, OperatorArg::UNARY_SUFFIX },
        { Operators::SQRT,    OperatorArg::UNARY_PREFIX },
    };

public:
    Eval()          = delete;
    virtual ~Eval() = delete;

    static std::optional<std::vector<Token>> parse(const Glib::ustring &str);
    static std::optional<std::deque<Token>>  shunting_yard(const std::vector<Token> &tokens);
    static double                            solve(std::deque<Token> rpn);
};

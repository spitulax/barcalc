#include "eval.hpp"
#include "symbols.hpp"
#include "utils.hpp"
#include <cassert>
#include <cctype>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <optional>
#include <string>
#include <utility>
#include <vector>

// TODO: handle errors. Return the error string to be displayed inside a popup?
std::optional<std::vector<Token>> Eval::parse(const Glib::ustring &str) {
    if (str.empty()) return std::nullopt;

    std::vector<Token> tokens;

    TokeniserState cur_state  = TokeniserState::NEW_TOKEN;
    TokeniserState next_state = TokeniserState::NEW_TOKEN;
    Token          cur_token{};
    Glib::ustring  cur_token_str = "";

    bool   found_decimal_mark  = false;
    size_t parenthesis_counter = 0;

    auto it   = str.begin();
    bool next = true;
    while (next) {
        switch (cur_state) {
            case TokeniserState::NEW_TOKEN: {
                cur_token = { TokenType::UNKNOWN, "" };
                cur_token_str.clear();

                if (std::isdigit(static_cast<unsigned char>(*it))) {
                    cur_token_str = *it;
                    next_state    = TokeniserState::NUMERIC;
                    ++it;
                } else if (*it == '(') {
                    next_state = TokeniserState::LPAREN;
                } else if (*it == ')') {
                    next_state = TokeniserState::RPAREN;
                } else if (find_symbols_str(*it).first) {
                    next_state = TokeniserState::OPERATOR;
                } else if (it == str.end()) {
                    next_state = TokeniserState::END;
                } else {
                    std::cerr << "Invalid token\n";
                    return std::nullopt;
                }
            } break;

            case TokeniserState::NUMERIC: {
                if (std::isdigit(static_cast<unsigned char>(*it)) || *it == '.') {
                    if (*it == '.') {
                        if (found_decimal_mark) {
                            std::cerr << "More than one decimal marks in a single number\n";
                            return std::nullopt;
                        }
                        found_decimal_mark = true;
                    }
                    cur_token_str += *it;
                    ++it;
                    next_state = TokeniserState::NUMERIC;
                } else {
                    cur_token       = {};
                    cur_token.type  = TokenType::NUMERIC;
                    cur_token.value = std::stod(cur_token_str);
                    cur_token.str   = std::move(cur_token_str);
                    next_state      = TokeniserState::COMPLETE;
                }
            } break;

            case TokeniserState::LPAREN: {
                cur_token_str = *it;
                ++it;
                ++parenthesis_counter;
                cur_token  = { TokenType::LPAREN, std::move(cur_token_str) };
                next_state = TokeniserState::COMPLETE;
            } break;

            case TokeniserState::RPAREN: {
                cur_token_str = *it;
                ++it;
                --parenthesis_counter;
                cur_token  = { TokenType::RPAREN, std::move(cur_token_str) };
                next_state = TokeniserState::COMPLETE;
            } break;

            case TokeniserState::OPERATOR: {
                cur_token_str = *it;
                ++it;
                cur_token      = {};
                cur_token.type = TokenType::OPERATOR;
                cur_token.op   = {
                      .type = Operators(find_symbols_str(cur_token_str[0]).second),
                };
                cur_token.str = std::move(cur_token_str);
                next_state    = TokeniserState::COMPLETE;
            } break;

            case TokeniserState::COMPLETE: {
                tokens.push_back(std::move(cur_token));
                next_state = TokeniserState::NEW_TOKEN;
            } break;

            case TokeniserState::END: {
                if (!cur_token_str.empty()) {
                    std::cout << "Unknown trailing token\n";
                    return std::nullopt;
                }
                next = false;
            } break;
        }

        cur_state = next_state;
    }

    if (parenthesis_counter != 0) {
        std::cerr << "Parentheses are not balanced\n";
        return std::nullopt;
    }

    return tokens;
}

std::optional<std::deque<Token>> Eval::shunting_yard(const std::vector<Token> &tokens) {
    std::deque<Token> holding_stack;
    std::deque<Token> output_stack;

    Token prev_token = { TokenType::NUMERIC };

    int i = 0;
    for (const auto &token : tokens) {
        switch (token.type) {
            case TokenType::NUMERIC: {
                output_stack.push_back(token);
                prev_token = output_stack.back();
            } break;
            case TokenType::LPAREN: {
                holding_stack.push_front(token);
                prev_token = holding_stack.front();
            } break;
            case TokenType::RPAREN: {
                if (holding_stack.empty()) {
                    std::cerr << "Unexpected closing parenthesis\n";
                    return std::nullopt;
                }

                while (!holding_stack.empty() && holding_stack.front().type != TokenType::LPAREN) {
                    output_stack.push_back(holding_stack.front());
                    holding_stack.pop_front();
                }

                if (holding_stack.empty()) {
                    std::cerr << "Unexpected parenthesis\n";
                    return std::nullopt;
                }
                if (!holding_stack.empty() && holding_stack.front().type == TokenType::LPAREN)
                    holding_stack.pop_front();

                prev_token = token;
            } break;
            case TokenType::OPERATOR: {
                Operator op = token.op;

                bool        maybe_unary  = false;
                bool        maybe_binary = false;
                OperatorArg arg          = OperatorArg::UNKNOWN;
                if (binary_operators.find(op.type) != binary_operators.end()) {
                    maybe_binary = true;
                }
                if (auto it = unary_operators.find(op.type); it != unary_operators.end()) {
                    maybe_unary = true;
                    arg         = it->second;
                }

                if (maybe_unary && maybe_binary) {
                    bool cond;
                    if (arg == OperatorArg::UNARY_PREFIX) {
                        cond = (prev_token.type != TokenType::NUMERIC &&
                                prev_token.type != TokenType::RPAREN) ||
                               i == 0;
                    } else if (arg == OperatorArg::UNARY_SUFFIX) {
                        cond = prev_token.type == TokenType::NUMERIC ||
                               prev_token.type == TokenType::RPAREN;
                    } else {
                        std::cerr << "Unknown unary operator\n";
                        return std::nullopt;
                    }
                    maybe_binary = !cond;
                    maybe_unary  = cond;
                }

                if (!(maybe_unary || maybe_binary)) {
                    std::cerr << "Operator unrecongnised by solver\n";
                    return std::nullopt;
                } else if (maybe_unary) {
                    op.arg = arg;
                } else if (maybe_binary) {
                    op.arg        = OperatorArg::BINARY;
                    op.precedence = binary_operators.at(op.type);
                }

                while (!holding_stack.empty() && holding_stack.front().type != TokenType::LPAREN) {
                    if (holding_stack.front().type == TokenType::OPERATOR) {
                        const auto &holding_op = holding_stack.front().op;
                        if (op.arg == OperatorArg::BINARY) {
                            const int holding_precedence =
                                (holding_op.arg == OperatorArg::UNARY_PREFIX ||
                                 holding_op.arg == OperatorArg::UNARY_SUFFIX)
                                    ? INT_MAX
                                    : binary_operators.at(holding_op.type);
                            if (holding_precedence >= op.precedence) {
                                output_stack.push_back(holding_stack.front());
                                holding_stack.pop_front();
                            } else {
                                break;
                            }
                        } else {
                            break;
                        }
                    }
                }

                holding_stack.push_front({
                    .type = TokenType::OPERATOR,
                    .str  = token.str,
                    .op   = op,
                });
                prev_token = holding_stack.front();
            } break;
            case TokenType::UNKNOWN: {
                std::cerr << "Expression contains unrecongnised token\n";
                return std::nullopt;
            } break;
        }

        ++i;
    }

    while (!holding_stack.empty()) {
        output_stack.push_back(holding_stack.front());
        holding_stack.pop_front();
    }

    return output_stack;
}
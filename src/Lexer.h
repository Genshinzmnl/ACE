#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Token.h"

class Lexer {
public:
    Lexer(const std::string& input);
    std::vector<Token> Tokenize();

private:
    std::string _input;
    size_t _position;
    static const std::unordered_map<std::string, TokenType> Keywords;

    Token ReadWord();
    Token ReadNumber();
    Token ReadString();
    Token ReadSymbol();
};

#endif // LEXER_H
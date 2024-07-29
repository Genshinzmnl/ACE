#include "Lexer.h"
#include <cctype>

const std::unordered_map<std::string, TokenType> Lexer::Keywords = {
    { "run", TokenType::Keyword },
    { "do", TokenType::Keyword },
    { "if", TokenType::Keyword },
    { "else", TokenType::Keyword },
    { "play", TokenType::Keyword },
    { "input", TokenType::Keyword },
    { "main", TokenType::Identifier },
    { "build", TokenType::Identifier } // Add build as identifier
};

Lexer::Lexer(const std::string& input) : _input(input), _position(0) {}

std::vector<Token> Lexer::Tokenize() {
    std::vector<Token> tokens;
    while (_position < _input.length()) {
        char current = _input[_position];
        if (isspace(current)) {
            _position++;
        }
        else if (isalpha(current)) {
            tokens.push_back(ReadWord());
        }
        else if (isdigit(current)) {
            tokens.push_back(ReadNumber());
        }
        else if (current == '"') {
            tokens.push_back(ReadString());
        }
        else {
            tokens.push_back(ReadSymbol());
        }
    }
    tokens.emplace_back(TokenType::EndOfFile, "");
    return tokens;
}

Token Lexer::ReadWord() {
    size_t start = _position;
    while (_position < _input.length() && isalpha(_input[_position])) {
        _position++;
    }
    std::string value = _input.substr(start, _position - start);
    auto it = Keywords.find(value);
    if (it != Keywords.end()) {
        return Token(it->second, value);
    }
    else {
        return Token(TokenType::Identifier, value);
    }
}

Token Lexer::ReadNumber() {
    size_t start = _position;
    while (_position < _input.length() && isdigit(_input[_position])) {
        _position++;
    }
    return Token(TokenType::Number, _input.substr(start, _position - start));
}

Token Lexer::ReadString() {
    _position++; // skip opening quote
    size_t start = _position;
    while (_position < _input.length() && _input[_position] != '"') {
        _position++;
    }
    std::string value = _input.substr(start, _position - start);
    _position++; // skip closing quote
    return Token(TokenType::StringLiteral, value);
}

Token Lexer::ReadSymbol() {
    char symbol = _input[_position];
    _position++;
    return Token(TokenType::Symbol, std::string(1, symbol));
}

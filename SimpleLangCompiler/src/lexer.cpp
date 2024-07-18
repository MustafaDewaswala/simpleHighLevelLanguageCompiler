#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>

enum class TokenType {
    KEYWORD, IDENTIFIER, NUMBER, OPERATOR, DELIMITER, UNKNOWN, END_OF_FILE
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};

class Lexer {
public:
    Lexer(const std::string& source) : src(source), pos(0) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        Token token = nextToken();
        while (token.type != TokenType::END_OF_FILE) {
            tokens.push_back(token);
            token = nextToken();
        }
        return tokens;
    }

private:
    std::string src;
    size_t pos;

    char peek() {
        return pos < src.size() ? src[pos] : '\0';
    }

    char advance() {
        return pos < src.size() ? src[pos++] : '\0';
    }

    void skipWhitespace() {
        while (std::isspace(peek())) advance();
    }

    Token nextToken() {
        skipWhitespace();
        char current = peek();

        if (std::isalpha(current)) {
            std::string identifier;
            while (std::isalnum(peek())) {
                identifier += advance();
            }
            if (isKeyword(identifier)) {
                return Token(TokenType::KEYWORD, identifier);
            }
            return Token(TokenType::IDENTIFIER, identifier);
        }

        if (std::isdigit(current)) {
            std::string number;
            while (std::isdigit(peek())) {
                number += advance();
            }
            return Token(TokenType::NUMBER, number);
        }

        if (isOperator(current)) {
            std::string op(1, advance());
            return Token(TokenType::OPERATOR, op);
        }

        if (isDelimiter(current)) {
            std::string delimiter(1, advance());
            return Token(TokenType::DELIMITER, delimiter);
        }

        if (current == '\0') {
            return Token(TokenType::END_OF_FILE, "");
        }

        return Token(TokenType::UNKNOWN, std::string(1, advance()));
    }

    bool isKeyword(const std::string& str) {
        static std::unordered_map<std::string, bool> keywords = {
            {"int", true}, {"if", true}, {"else", true}
        };
        return keywords.find(str) != keywords.end();
    }

    bool isOperator(char ch) {
        return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '=' || ch == '>' || ch == '<';
    }

    bool isDelimiter(char ch) {
        return ch == ';' || ch == '{' || ch == '}';
    }
};

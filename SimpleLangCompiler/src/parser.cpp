#include "lexer.cpp"
#include <memory>
#include <stdexcept>
#include <iostream>
#include <vector>
using namespace std;
enum class ASTNodeType {
    PROGRAM, STATEMENT, EXPRESSION, VARIABLE_DECL, ASSIGNMENT, BINARY_OP, IF_STATEMENT
};

struct ASTNode {
    ASTNodeType type;
    std::vector<std::unique_ptr<ASTNode>> children;
    std::string value;

    ASTNode(ASTNodeType t, const std::string& v = "") : type(t), value(v) {}
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens), pos(0) {}

    std::unique_ptr<ASTNode> parse() {
        auto root = std::make_unique<ASTNode>(ASTNodeType::PROGRAM);
        while (pos < tokens.size()) {
            root->children.push_back(parseStatement());
        }
        return root;
    }

private:
    std::vector<Token> tokens;
    size_t pos;

    Token currentToken() {
        return pos < tokens.size() ? tokens[pos] : Token(TokenType::END_OF_FILE, "");
    }

    Token advance() {
        return pos < tokens.size() ? tokens[pos++] : Token(TokenType::END_OF_FILE, "");
    }

    std::unique_ptr<ASTNode> parseStatement() {
        auto token = currentToken();
        if (token.type == TokenType::KEYWORD && token.value == "int") {
            return parseVariableDeclaration();
        } else if (token.type == TokenType::IDENTIFIER) {
            return parseAssignment();
        } else if (token.type == TokenType::KEYWORD && token.value == "if") {
            return parseIfStatement();
        } else {
            throw std::runtime_error("Unexpected token: " + token.value);
        }
    }

    std::unique_ptr<ASTNode> parseVariableDeclaration() {
        advance(); // consume 'int'
        std::string varName = advance().value;
        advance(); // consume '='
        auto expr = parseExpression();
        advance(); // consume ';'
        auto node = std::make_unique<ASTNode>(ASTNodeType::VARIABLE_DECL, varName);
        node->children.push_back(std::move(expr));
        return node;
    }

    std::unique_ptr<ASTNode> parseAssignment() {
        std::string varName = advance().value;
        advance(); // consume '='
        auto expr = parseExpression();
        advance(); // consume ';'
        auto node = std::make_unique<ASTNode>(ASTNodeType::ASSIGNMENT, varName);
        node->children.push_back(std::move(expr));
        return node;
    }

    std::unique_ptr<ASTNode> parseIfStatement() {
        advance(); // consume 'if'
        advance(); // consume '('
        auto condition = parseExpression();
        advance(); // consume ')'
        advance(); // consume '{'
        auto thenBranch = parseStatement();
        advance(); // consume '}'
        auto node = std::make_unique<ASTNode>(ASTNodeType::IF_STATEMENT);
        node->children.push_back(std::move(condition));
        node->children.push_back(std::move(thenBranch));
        if (currentToken().value == "else") {
            advance(); // consume 'else'
            advance(); // consume '{'
            auto elseBranch = parseStatement();
            advance(); // consume '}'
            node->children.push_back(std::move(elseBranch));
        }
        return node;
    }

    std::unique_ptr<ASTNode> parseExpression() {
        auto left = parsePrimary();
        while (currentToken().type == TokenType::OPERATOR) {
            auto op = advance().value;
            auto right = parsePrimary();
            auto node = std::make_unique<ASTNode>(ASTNodeType::BINARY_OP, op);
            node->children.push_back(std::move(left));
            node->children.push_back(std::move(right));
            left = std::move(node);
        }
        return left;
    }

    std::unique_ptr<ASTNode> parsePrimary() {
        auto token = advance();
        if (token.type == TokenType::NUMBER) {
            return std::make_unique<ASTNode>(ASTNodeType::EXPRESSION, token.value);
        } else if (token.type == TokenType::IDENTIFIER) {
            return std::make_unique<ASTNode>(ASTNodeType::EXPRESSION, token.value);
        } else {
            throw std::runtime_error("Unexpected token: " + token.value);
        }
    }
};

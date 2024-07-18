#include "parser.cpp"
#include <map>

class CodeGenerator {
public:
    CodeGenerator() {}

    std::vector<std::string> generateCode(ASTNode* root) {
        code.clear();
        visit(root);
        return code;
    }

private:
    std::vector<std::string> code;
    std::map<std::string, int> variables;

    void visit(ASTNode* node) {
        switch (node->type) {
        case ASTNodeType::PROGRAM:
            for (auto& child : node->children) {
                visit(child.get());
            }
            break;
        case ASTNodeType::VARIABLE_DECL:
            handleVariableDecl(node);
            break;
        case ASTNodeType::ASSIGNMENT:
            handleAssignment(node);
            break;
        case ASTNodeType::IF_STATEMENT:
            handleIfStatement(node);
            break;
        case ASTNodeType::BINARY_OP:
            handleBinaryOperation(node);
            break;
        case ASTNodeType::EXPRESSION:
            handleExpression(node);
            break;
        default:
            throw std::runtime_error("Unknown AST node type");
        }
    }

    void handleVariableDecl(ASTNode* node) {
        std::string varName = node->value;
        variables[varName] = 0; // Initialize variable to zero
        visit(node->children[0].get());
        code.push_back("STORE " + varName);
    }

    void handleAssignment(ASTNode* node) {
        std::string varName = node->value;
        visit(node->children[0].get());
        code.push_back("STORE " + varName);
    }

    void handleIfStatement(ASTNode* node) {
        visit(node->children[0].get()); // Condition
        std::string elseLabel = generateLabel("ELSE");
        std::string endIfLabel = generateLabel("END_IF");
        code.push_back("JZ " + elseLabel);
        visit(node->children[1].get()); // Then branch
        code.push_back("JMP " + endIfLabel);
        code.push_back(elseLabel + ":");
        if (node->children.size() > 2) {
            visit(node->children[2].get()); // Else branch
        }
        code.push_back(endIfLabel + ":");
    }

    void handleBinaryOperation(ASTNode* node) {
        visit(node->children[0].get()); // Left operand
        code.push_back("PUSH ACC"); // Save the result of the left operand
        visit(node->children[1].get()); // Right operand
        code.push_back("POP B"); // Retrieve the left operand result
        std::string op = node->value;
        if (op == "+") {
            code.push_back("ADD B");
        } else if (op == "-") {
            code.push_back("SUB B");
        } else if (op == "*") {
            code.push_back("MUL B");
        } else if (op == "/") {
            code.push_back("DIV B");
        } else if (op == ">") {
            code.push_back("CMP B");
            code.push_back("JGT SET_FLAG");
            code.push_back("CLR ACC");
            code.push_back("JMP END_CMP");
            code.push_back("SET_FLAG:");
            code.push_back("SET ACC");
            code.push_back("END_CMP:");
        } else if (op == "<") {
            code.push_back("CMP B");
            code.push_back("JLT SET_FLAG");
            code.push_back("CLR ACC");
            code.push_back("JMP END_CMP");
            code.push_back("SET_FLAG:");
            code.push_back("SET ACC");
            code.push_back("END_CMP:");
        }
    }

    void handleExpression(ASTNode* node) {
        if (std::isdigit(node->value[0])) {
            code.push_back("LOAD " + node->value);
        } else if (variables.find(node->value) != variables.end()) {
            code.push_back("LOAD " + node->value);
        } else {
            throw std::runtime_error("Unknown identifier: " + node->value);
        }
    }

    std::string generateLabel(const std::string& base) {
        static int labelCounter = 0;
        return base + "_" + std::to_string(labelCounter++);
    }
};

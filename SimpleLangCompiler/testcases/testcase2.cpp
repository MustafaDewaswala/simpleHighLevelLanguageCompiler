#include <iostream>
#include "..\src\assembly.cpp"
int main() {
    std::string sourceCode = R"(
    int a = 10;
    int b = 20;
    int c = a + b;
    if (a > b) {
        c = a;
    } else {
        c = b;
    }
    )";

    Lexer lexer(sourceCode);
    std::vector<Token> tokens = lexer.tokenize();

    Parser parser(tokens);
    std::unique_ptr<ASTNode> ast = parser.parse();

    CodeGenerator codeGen;
    std::vector<std::string> assemblyCode = codeGen.generateCode(ast.get());

    for (const auto& line : assemblyCode) {
        std::cout << line << std::endl;
    }

    return 0;
}

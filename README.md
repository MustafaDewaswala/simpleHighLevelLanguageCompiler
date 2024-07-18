# simpleHighLevelLanguageCompiler
# SimpleLang Compiler

## Overview

The SimpleLang Compiler is a project aimed at translating a simple high-level language (SimpleLang) into assembly code for an 8-bit CPU. This project helps understand basic compiler construction and 8-bit CPU architecture.

## Project Structure

- `lexer.cpp`: Contains the lexer that tokenizes SimpleLang code.
- `parser.cpp`: Contains the parser that generates an Abstract Syntax Tree (AST) from the tokens.
- `code_generator.cpp`: Contains the code generator that traverses the AST and generates assembly code.
- `main.cpp`: The main file that integrates the lexer, parser, and code generator to form the compiler.
- `README.md`: This file, providing an overview and instructions for the project.

## SimpleLang Syntax

SimpleLang supports variable declarations, assignments, arithmetic operations, and conditionals. Here are some examples:

```simplelang
int a = 10;
int b = 20;
int c = a + b;
if (a > b) {
    c = a;
} else {
    c = b;
}

converted to :
LOAD 10
STORE a
LOAD 20
STORE b
LOAD a
ADD b
STORE c
LOAD a
SUB b
JZ ELSE_0
STORE a
JMP END_IF_0
ELSE_0:
LOAD b
STORE c
END_IF_0:



Code Overview : 
Lexer
The lexer tokenizes SimpleLang code into tokens representing keywords, operators, identifiers, literals, and delimiters.

Parser
The parser generates an Abstract Syntax Tree (AST) from the tokens, ensuring the syntax is correct and handling errors gracefully.

Code Generator
The code generator traverses the AST and generates corresponding assembly code for the 8-bit CPU.

Main File
The main file integrates the lexer, parser, and code generator into a single compiler program.

Contributions are welcome! Please open an issue or submit a pull request with your changes.

Thanks

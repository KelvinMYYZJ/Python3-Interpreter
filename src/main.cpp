#include "Evalvisitor.h"
#include "Python3Lexer.h"
#include "Python3Parser.h"
#include "antlr4-runtime.h"
#include <iostream>
using namespace antlr4;
//todo: regenerating files in directory named "generated" is dangerous.
//       if you really need to regenerate,please ask TA for help.
int main(int argc, const char* argv[])
{
    if (argc > 1 && !strcmp(argv[1], "ffile")) {
        freopen("!input.txt", "r", stdin);
        // freopen("!output.txt","w",stdout);
    }
    //todo:please don't modify the code below the construction of ifs if you want to use visitor mode
    try {
        ANTLRInputStream input(std::cin);
        Python3Lexer lexer(&input);
        CommonTokenStream tokens(&lexer);
        tokens.fill();
        Python3Parser parser(&tokens);
        tree::ParseTree* tree = parser.file_input();
        EvalVisitor visitor;
        visitor.visit(tree);
    } catch (const char* exp) {
        std::cout << "error!!" << std::endl
                  << exp << std::endl;
    }
    return 0;
}

//
//  main.cpp
//  LexicalAnalysis
//
//  Created by Cody Thompson on 1/28/20.
//  Copyright © 2020 Cody Thompson. All rights reserved.
//

#include "Lexer.h"
#include "ParseTree.h"

int main( int argc, const char * argv[] ) {
    std::string srcFile = "/Users/codythompson/Documents/Code/Syntax Analyzer/Syntax Analyzer/myfile.txt";              // This will eventually be passed into the program as an arguement
    Lexer lex(srcFile);
    
    lex.output();
    
    return 0;
}

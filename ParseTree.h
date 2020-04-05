//
//  ParseTree.h
//  Syntax Analyzer
//
//  Created by Cody Thompson on 4/1/20.
//  Copyright © 2020 Cody Thompson. All rights reserved.
//

#include "Lexer.h"
#include <queue>
#include "globals.h"

#ifndef ParseTree_h
#define ParseTree_h

class ParseTree {
public:
    ParseTree(Lexer lex);
    bool SyntaxAnalysis(std::queue< std::pair< std::string, std::string >> statement);
    
private:
    void Add(leaf* p, leaf);
    
    leaf* root;
};


ParseTree::ParseTree(Lexer lex) {
    std::queue< std::pair< std::string, std::string >> statement;
    std::pair<std::string, std::string> tmp;
    do {
        do {
            tmp = lex.getNextLexeme();                                  // get the next lexeme (pair of lexeme and token)
            statement.push(tmp);                                        // push into the queue
        } while( std::count(sep.begin(), sep.end(), tmp.first) );       // check if lexeme is a separator ? break out of while; get next lexeme
    } while( SyntaxAnalysis(statement) || !lex.isEmpty() );                      // loop until syntactical error is found or we've reached the end of the lexer queue
}


bool ParseTree::SyntaxAnalysis(std::queue< std::pair< std::string, std::string >> statement) {
    
    return false;
}
#endif /* ParseTree_h */

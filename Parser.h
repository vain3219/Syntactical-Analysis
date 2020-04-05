//
//  Parser.h
//  Syntax Analyzer
//
//  Created by Cody Thompson on 4/4/20.
//  Copyright © 2020 Cody Thompson. All rights reserved.
//

#include "Lexer.h"
#include <queue>
#include "globals.h"

#ifndef Parser_h
#define Parser_h

class Parser {
public:
    Parser(Lexer lex);
    
private:
    bool SyntaxAnalysis(std::vector< std::pair< std::string, std::string >> statement);
    bool DeclarationRule(std::vector< std::pair< std::string, std::string >> statement);
    bool AssignRule(std::vector< std::pair< std::string, std::string >> statement);
    bool ExpressionRule();
    bool TermRule();
    bool FactorRule();
};


Parser::Parser(Lexer lex) {
    std::vector< std::pair< std::string, std::string >> statement;
    std::pair<std::string, std::string> tmp;
    do {
        do {
            tmp = lex.getNextLexeme();                                  // get the next lexeme (pair of lexeme and token)
            statement.push_back(tmp);                                   // push into the queue
        } while( std::count(sep.begin(), sep.end(), tmp.first) );       // check if lexeme is a separator ? break out of while; get next lexeme
        if( !SyntaxAnalysis(statement) )
            std::cerr << "Syntactical Error\n";
    } while( !lex.isEmpty() );                                          // loop until syntactical error is found or we've reached the end of the lexer queue
}

bool Parser::SyntaxAnalysis(std::vector< std::pair< std::string, std::string >> statement) {
    DeclarationRule(statement);
    
    AssignRule(statement);
    
    ExpressionRule();
    
    TermRule();
    
    FactorRule();
    
    return true;
}

bool Parser::DeclarationRule(std::vector< std::pair< std::string, std::string >> statement) {
    std::pair<std::string, std::string> tmp = statement.at(0);
    
    if( std::count(sep.begin(), sep.end(), tmp.first) ) {
        tmp = statement.at(1);
        if( tmp.second == "Identifier")
            return true;
    }
    
    return false;
}

bool Parser::AssignRule(std::vector<std::pair<std::string, std::string> > statement) {
    std::pair<std::string, std::string> tmp = statement.at(0);
    
    if(tmp.second == "Identifier") {
        tmp = statement.at(1);
        if(tmp.first == "=") {
            tmp = statement.at(2);
            // call ExpressionRule()
        }
    }
    return false;
}

#endif /* Parser_h */

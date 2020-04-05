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
    bool SyntaxAnalysis( std::vector< std::pair< std::string, std::string >> statement );
    bool DeclarationRule( std::vector< std::pair< std::string, std::string >> statement );
    bool AssignRule( std::vector< std::pair< std::string, std::string >> statement );
    bool ExpressionRule( std::vector< std::pair< std::string, std::string >> statement, int i );
    bool TermRule();
    bool FactorRule();
};


Parser::Parser( Lexer lex ) {
    std::vector< std::pair< std::string, std::string >> statement;
    std::pair<std::string, std::string> tmp;
    do {
        do {
            tmp = lex.getNextLexeme();                                  // get the next lexeme (pair of lexeme and token)
            statement.push_back( tmp );                                   // push into the queue
        } while( std::count( sep.begin(), sep.end(), tmp.first ) );       // check if lexeme is a separator ? break out of while; get next lexeme
        if( !SyntaxAnalysis(statement) )
            std::cerr << "Syntactical Error\n";
    } while( !lex.isEmpty() );                                          // loop until syntactical error is found or we've reached the end of the lexer queue
}

bool Parser::SyntaxAnalysis( std::vector< std::pair< std::string, std::string >> statement ) {
    
    if( DeclarationRule(statement) ) {
        return true;
    } else if ( AssignRule(statement) ) {
        return true;
    } else if ( ExpressionRule(statement, 0) ) {
        return true;
    } else if ( TermRule() ) {
        return true;
    } else if ( FactorRule() ) {
        return true;
    }

    return false;
}

bool Parser::DeclarationRule(std::vector< std::pair< std::string, std::string >> statement) {
    std::pair<std::string, std::string> tmp = statement.at(0);
    if( std::count(sep.begin(), sep.end(), tmp.first) ) {
        tmp = statement.at(1);
        if( tmp.first == "Identifier")
            return true;
    }
    return false;
}

bool Parser::AssignRule(std::vector<std::pair<std::string, std::string> > statement) {
    std::pair<std::string, std::string> tmp = statement.at(0);
    if( tmp.first == "Identifier" ) {
        tmp = statement.at(1);
        if( tmp.second == "=" ) {
            ExpressionRule( statement, 2 );
        }
    }
    return false;
}

bool Parser::ExpressionRule( std::vector< std::pair< std::string, std::string >> statement, int i ) {
        std::pair<std::string, std::string> tmp = statement.at(i);
    
    return false;
}
#endif /* Parser_h */

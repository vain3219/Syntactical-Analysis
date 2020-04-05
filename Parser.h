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
    bool AssignRule( std::vector< std::pair< std::string, std::string >> statement, int &i );
    bool ExpressionRule( std::vector< std::pair< std::string, std::string >> statement, int &i );
    bool ExpPrime( std::vector< std::pair< std::string, std::string >> statement, int &i );
    bool TermRule( std::vector< std::pair< std::string, std::string >> statement, int &i );
    bool TermPrime( std::vector< std::pair< std::string, std::string >> statement, int &i );
    bool FactorRule( std::vector< std::pair< std::string, std::string >> statement, int &i );
};


Parser::Parser( Lexer lex ) {
    std::vector< std::pair< std::string, std::string >> statement;
    std::pair<std::string, std::string> tmp;
    do {
        do {
            tmp = lex.getNextLexeme();                                       // get the next lexeme (pair of lexeme and token)
            statement.emplace_back( tmp );                                   // push into the queue
        } while( tmp.first != "Separator");                                  // check if lexeme is a separator ? break out of while; get next lexeme
        if( !SyntaxAnalysis(statement) )
            std::cerr << "Syntactical Error\n";
    } while( !lex.isEmpty() );                                                // loop until syntactical error is found or we've reached the end of the lexer queue
}

bool Parser::SyntaxAnalysis( std::vector< std::pair< std::string, std::string >> statement ) {
    int i = 0;
    if( DeclarationRule(statement) ) {
        return true;
    } else if ( AssignRule(statement, i) ) {
        return true;
    } else if ( ExpressionRule(statement, i) ) {
        return true;
    } else if ( TermRule(statement, i) ) {
        return true;
    } else if ( FactorRule(statement, i) ) {
        return true;
    }

    return false;
}

bool Parser::DeclarationRule(std::vector< std::pair< std::string, std::string >> statement) {
    std::pair<std::string, std::string> tmp = statement.at(0);
    if( std::count(type.begin(), type.end(), tmp.second) ) {
        tmp = statement.at(1);
        if( tmp.first == "Identifier")
            return true;
    }
    return false;
}

bool Parser::AssignRule(std::vector<std::pair<std::string, std::string> > statement, int &i ) {
    std::pair<std::string, std::string> tmp = statement.at(i++);
    if( tmp.first == "Identifier" ) {
        tmp = statement.at(i++);
        if( tmp.second == "=" ) {
            if( ExpressionRule( statement, i ) )
                return true;
        }
    }
    return false;
}

bool Parser::ExpressionRule( std::vector< std::pair< std::string, std::string >> statement, int &i ) {
    return( TermRule(statement, i) && ExpPrime(statement, i) );
}

bool Parser::ExpPrime( std::vector< std::pair< std::string, std::string >> statement, int &i ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    // if "+"
    // else if "-"
    // else
    return false;
}

bool Parser::TermRule( std::vector< std::pair< std::string, std::string >> statement, int &i ) {
    return ( FactorRule(statement, i) && TermPrime(statement, i) );
}

bool Parser::TermPrime( std::vector< std::pair< std::string, std::string >> statement, int &i ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    // if "*"
    // else if "/"
    // else
    return false;
}

bool Parser::FactorRule( std::vector< std::pair< std::string, std::string >> statement, int &i ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    // if "("
    //      ExpressionRule(statement, i);
    // else if(tmp.first == "Identifier")
    // else if(tmp.first == "Integer")
    return false;
}
#endif /* Parser_h */


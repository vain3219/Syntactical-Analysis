//
//  Parser.h
//  Syntax Analyzer
//
//  Created by Cody Thompson on 4/4/20.
//  Copyright © 2020 Cody Thompson. All rights reserved.
//

#include "Lexer.h"
#include <stack>
#include "globals.h"

#ifndef Parser_h
#define Parser_h

class Parser {
public:
    Parser(Lexer lex);
    
private:
    bool SyntaxAnalysis( std::vector< std::pair< std::string, std::string > > statement );
    bool DeclarationRule( std::vector< std::pair< std::string, std::string > > statement );
    bool AssignRule( std::vector< std::pair< std::string, std::string > > statement );
    bool ExpressionRule( std::vector< std::pair< std::string, std::string > > statement );
    bool ExpPrime( std::vector< std::pair< std::string, std::string > > statement );
    bool TermRule( std::vector< std::pair< std::string, std::string > > statement );
    bool TermPrime( std::vector< std::pair< std::string, std::string > > statement );
    bool FactorRule( std::vector< std::pair< std::string, std::string > > statement );
    void PrintProductions();
    int i;
};


Parser::Parser( Lexer lex ) {
    std::vector< std::pair< std::string, std::string > > statement;
    std::pair<std::string, std::string> tmp;
    do {
        i = 0;
        do {
            tmp = lex.getNextLexeme();                                          // get the next lexeme (pair of lexeme and token)
            statement.emplace_back( tmp );                                      // push into the queue
        } while( tmp.first != "Separator");                                     // check if lexeme is a separator ? break out of while; get next lexeme
        statement.pop_back();                                                   // delete the separator ( ';' is not included in any productions )
        statement.emplace_back( std::make_pair("$", "$") );                     // add the '$' to the end of the statement to signify the end
        for(int i = 0; i < statement.size(); i++) {                             // print the statement being processed
            tmp = statement.at(i);
            std::cout << tmp.second << " ";
        }
        std::cout << "\n-------------" << std::endl;
        if( !SyntaxAnalysis(statement) )                                        // begin the analysis
            std::cerr << "Syntactical Error\n\n";                               // illegal
        else
            std::cout << "No error detected\n\n";                               // legal
        statement.clear();                                                      // clear statement vector
    } while( !lex.isEmpty() );                                                  // loop until syntactical error is found or we've reached the end of the lexer queue
}

bool Parser::SyntaxAnalysis( std::vector< std::pair< std::string, std::string > > statement ) {
    if( DeclarationRule( statement ) ) {                                        // check declaration grammar
        std::cout << " \tS -> D\n";
        return true;
    } else if ( AssignRule( statement ) ) {                                     // check assignation grammar
        std::cout << " \tS -> A\n";
        return true;
    }
    
    return false;
}

bool Parser::DeclarationRule(std::vector< std::pair< std::string, std::string > > statement) {
    std::pair<std::string, std::string> tmp = statement.at(0);
    if( std::count(type.begin(), type.end(), tmp.second) ) {                    // First(D)
        tmp = statement.at(1);
        if( tmp.first == "Identifier")
            std::cout << tmp.second + "\tD -> Type id\n";
        return true;
    }
    return false;
}

bool Parser::AssignRule(std::vector<std::pair<std::string, std::string> > statement ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    if( tmp.first == "Identifier" ) {                                           // First(A)
        tmp = statement.at(++i);
        if( tmp.second == "=" ) {
            i++;
            if( ExpressionRule( statement ) )                                   // check expression grammar
                std::cout << tmp.second + "\tA -> id = E\n";
            return true;
        }
    }
    return false;
}

bool Parser::ExpressionRule( std::vector< std::pair< std::string, std::string > > statement ) {
    if( TermRule( statement ) ) {                                               // First(E)
        if( ExpPrime( statement ) ) {
            std::cout << " \tE -> TE'\n";
            return true;
        }
    }
    return false;
}

bool Parser::ExpPrime( std::vector< std::pair< std::string, std::string > > statement ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    if( tmp.second == "+" || tmp.second == "-" ) {
        i++;
        if( TermRule( statement ) ) {                                           // First(E')
            if( ExpPrime( statement ) ) {
                std::cout << tmp.second + "\tE' -> +TE' | -TE'\n";
                return true;
            }
        }
    } else {                                                                    // Follow(E')
        if( tmp.second == ")" || tmp.second == "$" ) {
            std::cout << tmp.second + "\tE' -> e\n";
            return true;
        }
    }
    return false;
}

bool Parser::TermRule( std::vector< std::pair< std::string, std::string > > statement ) {
    if( FactorRule( statement ) ) {
        if( TermPrime( statement ) ) {                                          // First(T)
            std::cout << " \tT -> FT'\n";
            return true;
        }
    }
    return false;
}

bool Parser::TermPrime( std::vector< std::pair< std::string, std::string > > statement ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    if( tmp.second == "*" || tmp.second == "/" ) {                              // First(T')
        i++;
        if( FactorRule( statement ) ) {
            if( TermPrime( statement ) ) {
                std::cout << tmp.second + "\tT' -> *FT' | /FT'\n";
                return true;
            }
        }
    } else {                                                                    // Follow(T')
        if( tmp.second == ")" || tmp.second == "+" || tmp.second == "-" || tmp.second == "$" ) {
            std::cout << tmp.second + "\tT' -> e\n";
            return true;
        }
    }
    return false;
}

bool Parser::FactorRule( std::vector< std::pair< std::string, std::string > > statement ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    if( tmp.second == "(" ) {
        i++;
        if( ExpressionRule( statement ) ) {
            tmp = statement.at(i);
            if( tmp.second == ")" )
                std::cout << tmp.second + "\tF -> (E)\n";
            return true;
        }
    } else if( tmp.first == "Identifier" ) {
        i++;
        std::cout << tmp.second + "\tF -> id\n";
        return true;
    } else if( tmp.first == "Integer" ) {
        i++;
        std::cout << tmp.second + "\tF -> num\n";
        return true;
    }
    return false;
}

#endif /* Parser_h */

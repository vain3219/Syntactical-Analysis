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
        statement.emplace_back( std::make_pair("$", "$") );                     // add the '$' to the end of the statement to signify the statements end
        for(int i = 0; i < statement.size(); i++) {                             // print the statement being processed
            tmp = statement.at(i);
            std::cout << tmp.second << " ";
        }
        std::cout << "\n---------------------";
        std::cout << "\n| tkn | Productions |";
        std::cout << "\n---------------------\n";
        if( SyntaxAnalysis(statement) )                                         // begin the analysis
            std::cout << "No error detected\n\n";
        statement.clear();                                                      // clear statement vector
    } while( !lex.isEmpty() );                                                  // loop until syntactical error is found or we've reached the end of the lexer queue
}

bool Parser::SyntaxAnalysis( std::vector< std::pair< std::string, std::string > > statement ) {
    if( DeclarationRule( statement ) ) {                                        // check declaration grammar
        return true;
    } else if ( AssignRule( statement ) ) {                                     // check assignation grammar
        return true;
    }
    std::cerr << "\tSyntax Error\n\n";
    return false;
}

bool Parser::DeclarationRule(std::vector< std::pair< std::string, std::string > > statement) {
    std::pair<std::string, std::string> tmp = statement.at(0);
    std::cout << tmp.second + "\t\tS -> D\n";
    if( std::count(type.begin(), type.end(), tmp.second) ) {                    // First(D)
        tmp = statement.at(1);
        if( tmp.first == "Identifier")
            std::cout << tmp.second + "\t\tD -> Type id\n";
        return true;
    }
    return false;
}

bool Parser::AssignRule(std::vector<std::pair<std::string, std::string> > statement ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    std::cout << tmp.second + " \t\tS -> A\n";
    if( tmp.first == "Identifier" ) {                                           // First(A)
        tmp = statement.at(++i);
        if( tmp.second == "=" ) {
            std::cout << tmp.second + "\t\tA -> id = E\n";
            i++;
            if( ExpressionRule( statement ) )                                   // check expression grammar
            return true;
        }
    }
    std::cerr << "\tAssignation rule error\n";
    return false;
}

bool Parser::ExpressionRule( std::vector< std::pair< std::string, std::string > > statement ) {
    std::cout << " \t\tE -> TE'\n";
    if( TermRule( statement ) ) {                                               // First(E)
        if( ExpPrime( statement ) ) {
            return true;
        }
    }
    std::cerr << "\tExpression rule error\n";
    return false;
}

bool Parser::ExpPrime( std::vector< std::pair< std::string, std::string > > statement ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    if( tmp.second == "+" || tmp.second == "-" ) {
        std::cout << tmp.second + "\t\tE' -> +TE' | -TE'\n";
        i++;
        if( TermRule( statement ) ) {                                           // First(E')
            if( ExpPrime( statement ) ) {
                return true;
            }
        }
    } else {                                                                    // Follow(E')
        if( tmp.second == ")" || tmp.second == "$" ) {
            std::cout << tmp.second + "\t\tE' -> e\n";
            return true;
        }
    }
    std::cerr << "\tExpression Prime rule error\n";
    return false;
}

bool Parser::TermRule( std::vector< std::pair< std::string, std::string > > statement ) {
    std::cout << " \t\tT -> FT'\n";
    if( FactorRule( statement ) ) {
        if( TermPrime( statement ) ) {                                          // First(T)
            return true;
        }
    }
    std::cerr << "\tTerm rule error\n";
    return false;
}

bool Parser::TermPrime( std::vector< std::pair< std::string, std::string > > statement ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    if( tmp.second == "*" || tmp.second == "/" ) {                              // First(T')
        std::cout << tmp.second + "\t\tT' -> *FT' | /FT'\n";
        i++;
        if( FactorRule( statement ) ) {
            if( TermPrime( statement ) ) {
                return true;
            }
        }
    } else {                                                                    // Follow(T')
        if( tmp.second == ")" || tmp.second == "+" || tmp.second == "-" || tmp.second == "$" ) {
            std::cout << tmp.second + "\t\tT' -> e\n";
            return true;
        }
    }
    std::cerr << "\tTerm Prime rule error\n";
    return false;
}

bool Parser::FactorRule( std::vector< std::pair< std::string, std::string > > statement ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    if( tmp.second == "(" ) {
        i++;
        if( ExpressionRule( statement ) ) {
            tmp = statement.at(i);
            if( tmp.second == ")" ) {
                std::cout << tmp.second + "\t\tF -> (E)\n";
                return true;
            }
        }
    } else if( tmp.first == "Identifier" ) {
        i++;
        std::cout << tmp.second + "\t\tF -> id\n";
        return true;
    } else if( tmp.first == "Integer" ) {
        i++;
        std::cout << tmp.second + "\t\tF -> num\n";
        return true;
    }
    std::cerr << "\tFactor rule error\n";
    return false;
}

#endif /* Parser_h */

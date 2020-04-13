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
    bool SyntaxAnalysis( std::vector< std::pair< std::string, std::string > > statement );
    bool DeclarationRule( std::vector< std::pair< std::string, std::string > > statement );
    bool AssignRule( std::vector< std::pair< std::string, std::string > > statement, int &i );
    bool ExpressionRule( std::vector< std::pair< std::string, std::string > > statement, int &i );
    bool ExpPrime( std::vector< std::pair< std::string, std::string > > statement, int &i );
    bool TermRule( std::vector< std::pair< std::string, std::string > > statement, int &i );
    bool TermPrime( std::vector< std::pair< std::string, std::string > > statement, int &i );
    bool FactorRule( std::vector< std::pair< std::string, std::string > > statement, int &i );
    void PrintProductions();
    
    // Attributes
    std::stack<std::string> productions;
};


Parser::Parser( Lexer lex ) {
    int x = 1;
    std::vector< std::pair< std::string, std::string > > statement;
    std::pair<std::string, std::string> tmp;
    do {
        std::cout << "Statement #: " << x++ << std::endl;
        do {
            tmp = lex.getNextLexeme();                                       // get the next lexeme (pair of lexeme and token)
            statement.emplace_back( tmp );                                   // push into the queue
        } while( tmp.first != "Separator");                                  // check if lexeme is a separator ? break out of while; get next lexeme
        statement.emplace_back("$");
        for(int i = 0; i < statement.size(); i++) {
            tmp = statement.at(i);
            std::cout << tmp.second << " ";
        }
        std::cout << std::endl;
        if( !SyntaxAnalysis(statement) )
            std::cerr << "\nSyntactical Error\n\n";
        else
            std::cout << "\nNo error detected\n\n";
        PrintProductions();
        statement.clear();
    } while( !lex.isEmpty() );                                                // loop until syntactical error is found or we've reached the end of the lexer queue
}

bool Parser::SyntaxAnalysis( std::vector< std::pair< std::string, std::string > > statement ) {
    int i = 0;
    
    if( DeclarationRule(statement) ) {
        productions.push("S -> D\n");
    } else if (AssignRule(statement, i )) {
        productions.push("S -> A\n");
    }

    return false;
}

bool Parser::DeclarationRule(std::vector< std::pair< std::string, std::string >> statement) {
    std::pair<std::string, std::string> tmp = statement.at(0);
    if( std::count(type.begin(), type.end(), tmp.second) ) {        // First(D)
        tmp = statement.at(1);
        if( tmp.first == "Identifier")
            productions.push("D -> Type id\n");
            return true;
    }
    return false;
}

bool Parser::AssignRule(std::vector<std::pair<std::string, std::string> > statement, int &i ) {
    std::pair<std::string, std::string> tmp = statement.at(i++);
    int x = i;
    if( tmp.first == "Identifier" ) {                               // First(A)
        tmp = statement.at(i++);
        if( tmp.second == "=" ) {
            if( ExpressionRule( statement, i ) )
                productions.push("A -> id = E\n");
                return true;
        }
    }
    i = x;
    return false;
}

bool Parser::ExpressionRule( std::vector< std::pair< std::string, std::string > > statement, int &i ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    if( TermRule(statement, i) ) {                                  // First(E)
        if( ExpPrime(statement, i) ) {
            productions.push("E -> TE'\n");
            return true;
        }
    }
    return false;
}

bool Parser::ExpPrime( std::vector< std::pair< std::string, std::string > > statement, int &i ) {
    std::pair<std::string, std::string> tmp = statement.at(i++);
    if( tmp.second == "+" || tmp.second == "-" ) {
        if( TermRule(statement, i) ) {                              // First(E')
            if( ExpPrime(statement, i) ) {
                productions.push("E' -> +TE' | -TE'\n");
                return true;
            }
        }
    } else {                                                        // Follow(E')
        if( tmp.second == ")" || tmp.second == "$" ) {              // Follow(E') book pg104
            i--;
            productions.push("E' -> e\n");
            return true;
        }
    }
    return false;
}

bool Parser::TermRule( std::vector< std::pair< std::string, std::string > > statement, int &i ) {
    if( FactorRule(statement, i) ) {
        if( TermPrime(statement, i) ) {                             // First(T)
            productions.push("T -> FT'\n");
            return true;
        }
    }
    return false;
}

bool Parser::TermPrime( std::vector< std::pair< std::string, std::string > > statement, int &i ) {
    std::pair<std::string, std::string> tmp = statement.at(i);
    if( tmp.second == "*" || tmp.second == "/" ) {                  // First(T')
        if( FactorRule(statement, i) ) {
            if( TermPrime(statement, i) ) {
                productions.push("T' -> *FT' | /FT'\n");
                return true;
            }
        }
    } else {                                                        // Follow(T')
        if( tmp.second == ")" || tmp.second == "+" || tmp.second == "-" || tmp.second == "$" ) {
           i--;
           productions.push("T' -> e\n");
           return true;
       }
    }
    return false;
}

bool Parser::FactorRule( std::vector< std::pair< std::string, std::string > > statement, int &i ) {
    std::pair<std::string, std::string> tmp = statement.at(i++);
    if( tmp.second == "(" ) {                                       // First(F)
        if( ExpressionRule(statement, i) ) {
            tmp = statement.at(i++);
            if( tmp.second == ")" )
                productions.push("F -> (E)\n");
                return true;
        }
    } else if( tmp.first == "Identifier" ) {
        productions.push("F -> id\n");
        return true;
    } else if( tmp.first == "Integer" ) {
        productions.push("F -> num\n");
        return true;
    }
    return false;
}

void Parser::PrintProductions() {
    std::string e;
    while( !productions.empty() ) {
        e = productions.top();
        std::cout << e << std::endl;
        productions.pop();
    }
}
#endif /* Parser_h */


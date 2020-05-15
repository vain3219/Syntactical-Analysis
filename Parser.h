//
//  Parser.h
//  Syntax Analyzer
//
//  Created by Cody Thompson on 4/4/20.
//  Copyright © 2020 Cody Thompson. All rights reserved.
//

/*
DOCUMENTATION FOR INHERITED LIBRARIES in: lexer.h
FILE: parser.h
CLASS PROVIDED: Parser
CLASS BEHAVIOR:     Reads lexemes and tokens stored in the lexer and analyzes input based on provided grammars
             Provides one contructor (parameter: Lexer)
             Includes 9 member functions that will sort through statements broken into tokens and lexemes, analyze the statement based on the provided grammars
             and print the statement being processed.
        
CONSTRUCTORS for the Parser class:
    Parser(Lexer lex)
        Precondition: Textfile has been successfully read and analyzed by the Lexer class; statements broken down into tokens and lexemes are passed to through
            to the Parser as a parameter
        Postcondition: Parser object has been initialized and statements of lexer and token pairs will be analyzed
FUNCTIONS for the Parser Class:
    bool SyntaxAnalysis()
        Postcondition: Checks statement against grammars for declarations and assignments and returns true if statement is valid; else it returns false
    bool DeclarationRule()
        Postcondition: Returns true if passed statement is a valid declaration
    bool AssignRule()
        Postcondition: Calls on ExpressionRule function; if ExpressionRule returns True, will return true if statement is a valid asignation
    bool ExpressionRule()
        Postcondition: If TermRule and ExpPrime functions return true, will resturn true if statement is a valid expression
    bool ExpPrime()
        Postcondition: Handles end of statement for expression rule; if TermRule and ExpPrime functions return true, if end of statement is found, will return true
    bool TermRule()
        Postcondition: If FactorRule and TermPrime functions return true, will return true if statement is a valid term
    bool TermPrime()
        Postcondition: Handles end of statement for term rule; if FactorRule and TermPrime functions return true or if end of statement is found, will return true
    bool FactorRule()
        Postcondition: if ExpressionRule function, end of statement return true, will return true
    void PrintProductions()
        Postcondition: Prints productions and lexemes and tokens
*/

#include "Lexer.h"
#include "globals.h"
#include <stack>

#ifndef Parser_h
#define Parser_h

class Parser {
public:
    Parser(Lexer lex);
    
private:
    bool SyntaxAnalysis( );
    bool DeclarationRule( );
    bool AssignRule( );
    bool ExpressionRule( );
    bool ExpPrime( );
    bool TermRule( );
    bool TermPrime( );
    bool FactorRule( );
    
    std::stack< std::pair< std::string, std::string > > statement;
    std::pair<std::string, std::string> tmp;
};


Parser::Parser( Lexer lex )
// Instantiates an object of the Parser class using values from
// Stores the lexeme and token pairs in a vector
// Library facilities used: string, vector, iostream
{
    std::vector< std::pair< std::string, std::string > > helper;
    do {
        statement.push( std::make_pair("$", "$") );                             // add the '$' to the end of the statement to signify the statements end
        do {
            tmp = lex.getNextLexeme();                                          // get the next lexeme (pair of lexeme and token)
            helper.emplace_back( tmp );                                         // push into the queue
        } while( tmp.first != "Separator");                                     // check if lexeme is a separator ? break out of while; get next lexeme
        for(size_t x = helper.size() - 2; x > 0; x--) {
            statement.push(helper.at(x));
            std::cout << helper.at(x).second << " ";
        }
        std::cout << helper.at(0).second;
        statement.push(helper.at(0));                                           // delete the separator ( ';' is not included in any productions )
        std::cout << "\n---------------------";
        std::cout << "\n| tkn | Productions |";
        std::cout << "\n---------------------\n";
        if( SyntaxAnalysis() )                                         // begin the analysis
            std::cout << "No error detected\n\n";
        helper.clear();
    } while( !lex.isEmpty() );                                                  // loop until syntactical error is found or we've reached the end of the lexer queue
}

bool Parser::SyntaxAnalysis(  )
// Parse statement vector to DeclarationRule and AssignRule for validation
// Library facilities used: vector, iostream
{
    if( DeclarationRule( ) ) {                                        // check declaration grammar
        return true;
    } else if ( AssignRule( ) ) {                                     // check assignation grammar
        return true;
    }
    std::cerr << "\tSyntax Error\n\n";
    return false;
}

bool Parser::DeclarationRule( )
// Determines if passed statement vector is a valid declarative based on given production rules
// Library facilities used: vector, iostream
{
    tmp = statement.top();
    std::cout << tmp.second + "\t\tS -> D\n";
    if( std::count(type.begin(), type.end(), tmp.second) ) {                    // First(D)
        statement.pop();
        tmp = statement.top();
        if( tmp.first == "Identifier")
            std::cout << tmp.second + "\t\tD -> Type id\n";
        return true;
    }
    return false;
}

bool Parser::AssignRule( )
// Determines if passed statement vector is a valid assignment based on given production rules
// Calls on ExpressionRule function
// Library facilities used: vector, iostream
{
    tmp = statement.top();
    std::cout << tmp.second + " \t\tS -> A\n";
    if( tmp.first == "Identifier" ) { // First(A)
        statement.pop();
        tmp = statement.top();
        if( tmp.second == "=" ) {
            std::cout << tmp.second + "\t\tA -> id = E\n";
            statement.pop();
            if( ExpressionRule( ) )                                   // check expression grammar
            return true;
        }
    }
    std::cerr << "\tAssignation rule error: " << tmp.second << std::endl;
    return false;
}

bool Parser::ExpressionRule( )
// Determines if passed statement is a valid expression based on given production rules
// Calls on TermRule and ExpPrime (expression prime) functions
// Library facilities used: vector, iostream
{
    std::cout << " \t\tE -> TE'\n";
    if( TermRule( ) ) {                                               // First(E)
        if( ExpPrime( ) ) {
            return true;
        }
    }
    std::cerr << "\tExpression rule error: " << statement.top().second << std::endl;
    return false;
}

bool Parser::ExpPrime( )
// Handles end case for expression rule based on given production rules
// Calls on TermRule and ExpPrime (expression prime) functions
// Library facilities used: vector, iostream
{
    tmp = statement.top();
    if( tmp.second == "+" || tmp.second == "-" ) {
        std::cout << tmp.second + "\t\tE' -> +TE' | -TE'\n";
        statement.pop();
        if( TermRule( ) ) {                                           // First(E')
            if( ExpPrime( ) ) {
                return true;
            }
        }
    } else {                                                                    // Follow(E')
        if( tmp.second == ")" || tmp.second == "$" ) {
            std::cout << tmp.second + "\t\tE' -> e\n";
            return true;
        }
    }
    std::cerr << "\tExpression Prime rule error: " << tmp.second << std::endl;
    return false;
}

bool Parser::TermRule( )
// Determines if passed statement is a valid term based on given productions
// Calls on FactorRule and TermPrime functions
// Library facilities used: vector, iostream
{
    std::cout << " \t\tT -> FT'\n";
    if( FactorRule( ) ) {
        if( TermPrime( ) ) {                                          // First(T)
            return true;
        }
    }
    std::cerr << "\tTerm rule error: " << statement.top().second << std::endl;
    return false;
}

bool Parser::TermPrime( )
// Handles end case for Term rule based on given productions
// Calls FactorRule TermPrime functions
// Library facilities used: vector, iostream
{
    tmp = statement.top();
    if( tmp.second == "*" || tmp.second == "/" ) {                              // First(T')
        std::cout << tmp.second + "\t\tT' -> *FT' | /FT'\n";
        statement.pop();
        if( FactorRule( ) ) {
            statement.pop();
            tmp = statement.top();
            if( TermPrime( ) ) {
                return true;
            }
        }
    } else {                                                                    // Follow(T')
        if( tmp.second == ")" || tmp.second == "+" || tmp.second == "-" || tmp.second == "$" ) {
            std::cout << tmp.second + "\t\tT' -> e\n";
            return true;
        }
    }
    std::cerr << "\tTerm Prime rule error: " << tmp.second << std::endl;
    return false;
}

bool Parser::FactorRule( )
// Determines if passed statement is a valid factor based on given productions
// Calls ExpressionRule function
// Library facilities used: vector, iostream
{
    tmp = statement.top();
    if( tmp.second == "(" ) {
        statement.pop();
        if( ExpressionRule( ) ) {
            tmp = statement.top();
            if( tmp.second == ")" ) {
                std::cout << tmp.second + "\t\tF -> (E)\n";
                return true;
            }
        }
    } else if( tmp.first == "Identifier" ) {
        statement.pop();
        std::cout << tmp.second + "\t\tF -> id\n";
        tmp = statement.top();
        return true;
    } else if( tmp.first == "Integer" ) {
        statement.pop();
        std::cout << tmp.second + "\t\tF -> num\n";
        return true;
    }
    std::cerr << "\tFactor rule error: " << tmp.second << std::endl;
    return false;
}

#endif /* Parser_h */

//
//  globals.h
//  Syntax Analyzer
//
//  Created by Cody Thompson on 4/4/20.
//  Copyright © 2020 Cody Thompson. All rights reserved.
//

#ifndef globals_h
#define globals_h

enum state{NONE, ENTRY, IDENTIFIER, INTEGER, IN_FLOAT, FLOAT, UNKNOWN, OPERATOR, SEPARATOR};
std::string keyAr[] = {"int", "float", "bool", "true", "false", "if", "else", "then", "endif", "while", "whileend", "do", "doend", "for", "forend", "input", "output", "and", "or", "not"};
std::vector<std::string> op{"*", "+", "-", "=", "/", ">", "<", "%"};
std::vector<std::string> sep{"'", "(" ,")" ,"{" ,"}" ,"[" ,"]" ,"," ,"." ,":" ,";"};


struct leaf {
    leaf* left;
    leaf* right;
    std::pair< std::string , std::string > e;
};

#endif /* globals_h */

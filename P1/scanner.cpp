//
//  scanner.cpp
//  P1
//
//  Created by Eric Goodwin on 3/28/19.
//  Copyright © 2019 umsl. All rights reserved.
//

#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "scanner.h"
#include "token.h"

#define nl \n

using namespace std;

string operators = "+-*/%<>=";
string lowerCase = "abcdefghijklmnopqrstuvwxyz";
string upperCase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string reservedWords[] = {"iter", "void", "var", "return", "scan", "print", "program", "cond", "then", "let", "int"};
string delimiters = ":.();{}[]";
string digits = "1234567890";

token_t tokenPartial;
token_t tokenComplete;
token_t tokenNext;


token_t checkCharacter(char c, int lineNumber){
    
    
    if (lowerCase.find(c)){
        tokenPartial.tokenInstance = tokenPartial.tokenInstance + c;
        tokenPartial.tokenID = identiferToken;
    }
    else if (upperCase.find(c)){
        if (tokenPartial.tokenInstance.length() == 0){
        // error
        } else {
            tokenPartial.tokenInstance = tokenPartial.tokenInstance + c;
        }
    }
    else if (digits.find(c)){
        if (tokenPartial.tokenInstance.length() == 0){
            tokenPartial.tokenID = digitToken;
        }
        tokenPartial.tokenInstance = tokenPartial.tokenInstance + c;

    }
    else if (c == '&') {
        //ignore until nl
    }
    else if (delimiters.find(c)) {
        if (tokenPartial.tokenInstance.length() == 0){
            tokenPartial.tokenID = delimiterToken;
        }
        tokenPartial.tokenInstance = tokenPartial.tokenInstance + c;
    }
    else if (operators.find(c)) {
        if (tokenPartial.tokenInstance.length() == 0){
            tokenPartial.tokenID = operatorToken;
        }
        tokenPartial.tokenInstance = tokenPartial.tokenInstance + c;
    }
    else if (c == EOF){
        tokenComplete.tokenID = EOFToken;
        tokenComplete.tokenInstance = EOF;
        tokenComplete.lineNumber = lineNumber;
        
        return tokenComplete;
    }
    else {
        //error
    }
    
    tokenPartial.lineNumber = lineNumber;
    
    
    return tokenPartial;
}

token_t filter1(char c, int lineNumber){
    return tokenPartial;
}

bool filter2(token_t token){
    if (1){
        return true;
    } else {
        return false;
    }



}




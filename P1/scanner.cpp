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

char delimiters[] = {':' , '.' , ';', '(' , ')' , '{', '}' , '[' , ']'};
char operators[] = {'+', '-', '*', '/', '%', '<', '>', '=' };
char lowerCase[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm' , 'n', 'o' , 'p','q', 'r', 's' , 't' , 'u', 'v', 'w', 'x', 'y', 'z' };
char upperCase[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' , 'H', 'I' , 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};


string reservedWords[] = {"iter", "void", "var", "return", "scan", "print", "program", "cond", "then", "let", "int"};



token_t tokenPartial;
token_t tokenComplete;
token_t tokenNext;


token_t checkCharacter(char c, int lineNumber){
    
    if (find(begin(lowerCase), end(lowerCase), c)){
        tokenPartial.tokenInstance = tokenPartial.tokenInstance + c;
        tokenPartial.tokenID = identiferToken;
    }
    else if (find(begin(upperCase), end(upperCase), c)){
        if (tokenPartial.tokenInstance.length() == 0){
        // error
        } else {
            tokenPartial.tokenInstance = tokenPartial.tokenInstance + c;
        }
    }
    else if (isdigit(c)){
        if (tokenPartial.tokenInstance.length() == 0){
            tokenPartial.tokenID = digitToken;
        }
        tokenPartial.tokenInstance = tokenPartial.tokenInstance + c;
        
    }
    else if (c == '&') {
        //ignore until nl
    }
    else if (find(begin(delimiters), end(delimiters), c)) {
        if (tokenPartial.tokenInstance.length() == 0){
            tokenPartial.tokenID = delimiterToken;
        }
        tokenPartial.tokenInstance = tokenPartial.tokenInstance + c;
    }
    else if (find(begin(operators), end(operators), c )) {
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
    if (find(begin(reservedWords), end(reservedWords), token)) {
        return true;
    } else {
        return false;
    }
    
}




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

const string operators = "+-*/%<>=";
const string lowerCase = "abcdefghijklmnopqrstuvwxyz";
const string upperCase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string reservedWords[] = {"iter", "void", "var", "return", "scan", "print", "program", "cond", "then", "let", "int"};
const string delimiters = ":.();{}[] ";
const string digits = "1234567890";


// columns (valid characters)  as follows:
// uppercase    lowercase    digit     =    <      >     :    +    -    *    /    %    .    (    )    ;    {    }    ,    [    ]    WS    &
//rows (states) as follows: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24

const int stateTable[24][23] = {
    //       UC,   LC,     D,    =,       <,      >,     :,      +,       -,      *,      /,     %,       .,      (,     ),       ;,       {,    },       ,,      [,      ],    WS,   &
/*1*/     {   2,    3,    10,    21,     23,     20,     24,     19,     19,     19,     19,     19,     24,     24,     24,     24,      24,    24,     24,      24,     24,    1,  18  },
/*2*/     {   3,    3,    3,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*3*/     {   4,    4,    4,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*4*/     {   5,    5,    5,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*5*/     {   6,    6,    6,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*6*/     {   7,    7,    7,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*7*/     {   8,    8,    8,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*8*/     {   9,    9,    9,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*9*/     {1000, 1000, 1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*10*/    {1001, 1001,   11,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*11*/    {1001, 1001,   12,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*12*/    {1001, 1001,   13,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*13*/    {1001, 1001,   14,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*14*/    {1001, 1001,   15,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*15*/    {1001, 1001,   16,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*16*/    {1001, 1001,   17,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*17*/    {1001, 1001, 1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*18*/    {  18,   18,   18,     18,     18,     18,     18,     18,     18,     18,     18,     18,     18,     18,     18,     18,      18,    18,     18,      18,     18,   18,   18 },
/*19*/    {1002, 1002, 1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*20*/    {1002, 1002, 1002,     22,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*21*/    {1002, 1002, 1002,     22,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*22*/    {1002, 1002, 1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*23*/    {1002, 1002, 1002,     22,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*24*/    {1003, 1003, 1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,    1003,  1003,   1003,    1003,   1003, 1003, 1003 },
    
};



token_t tokenCurrent;
token_t tokenNext;

token_t checkCharacter(char c, int lineNumber){
    if (lowerCase.find(c)){
        tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + c;
        tokenCurrent.tokenID = identiferToken;
    }
    else if (upperCase.find(c)){
        if (tokenCurrent.tokenInstance.length() == 0){
        // error
        } else {
            tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + c;
        }
    }
    else if (digits.find(c)){
        if (tokenCurrent.tokenInstance.length() == 0){
            tokenCurrent.tokenID = digitToken;
        }
        tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + c;
    }
    else if (c == '&') {
        //ignore until nl
    }
    else if (delimiters.find(c)) {
        if (tokenCurrent.tokenInstance.length() == 0){
            tokenCurrent.tokenID = delimiterToken;
        }
        tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + c;
    }
    else if (operators.find(c)) {
        if (tokenCurrent.tokenInstance.length() == 0){
            tokenCurrent.tokenID = operatorToken;
        }
        tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + c;
    }
    else if (c == EOF){
//        tokenComplete.tokenID = EOFToken;
//        tokenComplete.tokenInstance = EOF;
//        tokenComplete.lineNumber = lineNumber;
//
//        return tokenComplete;
    }
    else {
        //error
    }
    
    tokenCurrent.lineNumber = lineNumber;
    
    
    return tokenCurrent;
}

token_t filter1(char c, int lineNumber){
    tokenCurrent = checkCharacter(c, lineNumber);
    
    return tokenCurrent;
}

bool filter2(token_t token){
    if (1){
        return true;
    } else {
        return false;
    }



}




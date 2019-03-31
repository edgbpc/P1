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

#define error -1

using namespace std;

const string operatorss = "+-*/%<>=";
//const string lowerCase = "abcdefghijklmnopqrstuvwxyz";
//const string upperCase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string reservedWords[] = {"iter", "void", "var", "return", "scan", "print", "program", "cond", "then", "let", "int"};//
const string delimiters = ":.();{}[] ";
//const string digits = "1234567890";


// columns (valid characters)  as follows:
// uppercase    lowercase    digit     =    <      >     :    +    -    *    /    %    .    (    )    ;    {    }    ,    [    ]    WS    &
//rows (states) as follows: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24

const int stateTable[][23] = {
    //         0        1      2    3       4       5       6       7       8       9       10      11      12      13      14      15      16     17       18      19      20      21  22
    //          UC,   LC,    D,    =,       <,      >,     :,      +,       -,      *,      /,     %,       .,      (,     ),       ;,       {,    },       ,,      [,      ],    WS,   &
/*s1|0*/     {  -1,    2,   10,    21,     23,     20,     24,     19,     19,     19,     19,     19,     24,     24,     24,     24,      24,    24,     24,      24,     24,    1,  18  },
/*s2|1*/     {   3,    3,    3,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*s3|2*/     {   4,    4,    4,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*s4|3*/     {   5,    5,    5,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*s5|4*/     {   6,    6,    6,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*s6|5*/     {   7,    7,    7,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*s7|6*/     {   8,    8,    8,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*s8|7*/     {   9,    9,    9,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*s9|8*/     {1000, 1000, 1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,   1000,    1000,  1000,   1000,    1000,   1000, 1000, 1000 },
/*s10|9*/    {1001, 1001,   11,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s11|0*/    {1001, 1001,   12,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s12|11*/   {1001, 1001,   13,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s13|12*/   {1001, 1001,   14,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s14|13*/   {1001, 1001,   15,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s15|14*/   {1001, 1001,   16,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s16|15*/   {1001, 1001,   17,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s17|16*/   {1001, 1001, 1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s18|17*/   {  18,   18,   18,     18,     18,     18,     18,     18,     18,     18,     18,     18,     18,     18,     18,     18,      18,    18,     18,      18,     18,   18,   18 },
/*s19|18*/   {1002, 1002, 1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*s20|19*/   {1002, 1002, 1002,     22,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*s21|20*/   {1002, 1002, 1002,     22,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*s22|21*/   {1002, 1002, 1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*s23|22*/   {1002, 1002, 1002,     22,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*s24|23*/   {1003, 1003, 1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,   1003,    1003,  1003,   1003,    1003,   1003, 1003, 1003 },
    
    // key: <0 = Error, < 1000 = go to new state, > 1000 final state as follows: 1000 = idToken, 1001 = numToken, 1002 = operatorToken, 1003 = delimiterToken
    
};

char workingCharacter;
char lookaheadCharacter;
bool newToken = false;


token_t tokenCurrent;
partialToken_t tokenNextFragment;
partialToken_t tokenFragment;

int stateIndex = 0; //start at index 0/state 1
int previousStateIndex;

void checkCharacter(partialToken_t tokenFragment){
   // int index = lowerCase.find(tokenFragment.characterToCheck);
    
    if (tokenFragment.charType == lower){
        if (stateTable[stateIndex][1] != error){
            tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + tokenFragment.characterToCheck;
            tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
            stateIndex = (stateTable[stateIndex][1] - 1);
        } else {
            //error
        }
        if (stateTable[stateIndex][1] == 1000){
            tokenCurrent.tokenID = identifierToken;
            // need to retain the character that was being processed when the state is in a complete state
            tokenNextFragment.characterToCheck = tokenFragment.characterToCheck;
            tokenNextFragment.lineNumberCharacterOn = tokenFragment.lineNumberCharacterOn;
        }
    }
    else if (tokenFragment.charType == upper){
        if (stateTable[stateIndex][0] != error){
            tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + tokenFragment.characterToCheck;
            tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
            stateIndex = stateTable[stateIndex][0];
        } else {
            //error
        }
        if (stateTable[stateIndex][0] == 1000){
            tokenCurrent.tokenID = identifierToken;
            // need to retain the character that was being processed when the state is in a complete state
            tokenNextFragment.characterToCheck = tokenFragment.characterToCheck;
            tokenNextFragment.lineNumberCharacterOn = tokenFragment.lineNumberCharacterOn;
        }
    }
    else if (tokenFragment.charType == digit){
        if (stateTable[stateIndex][2] != error){
            tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + tokenFragment.characterToCheck;
            tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
            stateIndex = stateTable[stateIndex][2-1];
        } else {
            //error
        }
        if (stateTable[stateIndex][2] == 1001){
            tokenCurrent.tokenID = digitToken;
            // need to retain the character that was being processed when the state is in a complete state
            tokenNextFragment.characterToCheck = tokenFragment.characterToCheck;
            tokenNextFragment.lineNumberCharacterOn = tokenFragment.lineNumberCharacterOn;
        }
    }
    //TODO - fix next state algorithm for delimiters/operators
    else if (tokenFragment.charType == delimiter) {
        int delimiterIndex = int(delimiters.find(tokenFragment.characterToCheck));
        if (stateTable[stateIndex][delimiterIndex]  != error) {
            tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + tokenFragment.characterToCheck;
            tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
            previousStateIndex = stateIndex;
            stateIndex = stateTable[stateIndex][delimiterIndex - 1];
        } else {
            //error
        }
        if (stateTable[stateIndex][delimiterIndex]  >= 1000) {
            determineTokenType(stateIndex);
            tokenCurrent.tokenID = delimiterToken;
            // need to retain the character that was being processed when the state is in a complete state
            tokenNextFragment.characterToCheck = tokenFragment.characterToCheck;
            tokenNextFragment.lineNumberCharacterOn = tokenFragment.lineNumberCharacterOn;
        }
    }
    else if (tokenFragment.charType == operators) {
        int operatorIndex = int(operatorss.find(tokenFragment.characterToCheck));
        if (stateTable[stateIndex][operatorIndex] != error) {
            tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + tokenFragment.characterToCheck;
            tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
            stateIndex = stateTable[stateIndex][operatorIndex - 1];
        } else {
            //error
        }
        if (stateIndex >= 1000) {
            determineTokenType(stateIndex);
            tokenCurrent.tokenID = operatorToken;
                // need to retain the character that was being processed when the state is in a complete state
            tokenCurrent.tokenInstance.pop_back();
            tokenNextFragment.characterToCheck = tokenFragment.characterToCheck;
            tokenNextFragment.lineNumberCharacterOn = tokenFragment.lineNumberCharacterOn;
            
            
        }
    }
    else if (tokenFragment.characterToCheck == EOF){
        tokenCurrent.tokenID = EOFToken;
        tokenCurrent.tokenInstance = EOF;
        tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;

    }
    else {
        //error
    }
    
 //   tokenCurrent.lineNumber = lineNumber;
   
}

void determineTokenType(int stateIndex){
    switch (stateIndex){
        case 1000:
            tokenCurrent.tokenID = identifierToken;
            break;
        case 1001:
            tokenCurrent.tokenID = digitToken;
            break;
        case 1002:
            tokenCurrent.tokenID = operatorToken;
            break;
        case 1003:
            tokenCurrent.tokenID = delimiterToken;
            break;
        default:
            break;
    }
}


void scanner(partialToken_t tokenFragment){
    cout << tokenFragment.characterToCheck << '\t' << tokenFragment.lineNumberCharacterOn << endl;
    checkCharacter(tokenFragment);
    
}

void filter1(char workingCharacter, int lineNumber){
    // filter found a line starting with a comment
    
    if (workingCharacter == '&'){
        tokenFragment.isPartOfComment = true;
    }
    
    if (tokenFragment.isPartOfComment == true){
        // do nothing, keep reading until \n is read
        if (workingCharacter == '\n'){
            tokenFragment.isPartOfComment = false;
        }
    } else {
        tokenFragment.characterToCheck = workingCharacter;
        tokenFragment.lineNumberCharacterOn = lineNumber;
        if (islower(workingCharacter)){
            tokenFragment.charType = lower;
        }
        if (isupper(workingCharacter)){
            tokenFragment.charType = upper;
        }
        if (isdigit(workingCharacter)){
            tokenFragment.charType = digit;
        }
        if (workingCharacter == '+' || workingCharacter == '-' || workingCharacter == '/' || workingCharacter == '*' || workingCharacter == '%' || workingCharacter == '=' || workingCharacter == '>' || workingCharacter == '<'){
            tokenFragment.charType = operators;
        }
        if (workingCharacter == ':' || workingCharacter == '.' || workingCharacter == '(' || workingCharacter == ')' || workingCharacter == ';' || workingCharacter == '{' || workingCharacter == '}' || workingCharacter == ',' || workingCharacter == '[' || workingCharacter == ']'){
            tokenFragment.charType = delimiter;
        }
        if (workingCharacter == '\n'){
            tokenFragment.charType = newLine;
        }
        if (workingCharacter == EOF){
            tokenFragment.charType = eof;
        }
        scanner(tokenFragment);
        
    }
    
}

bool filter2(token_t token){
    if (1){
        return true;
    } else {
        return false;
    }



}




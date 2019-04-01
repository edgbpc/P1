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
#include <stdlib.h>
#include "scanner.h"
#include "token.h"

#define error -1

using namespace std;

const string operatorss = "+-*/%<>=";
const string reservedWords[11] = {"iter", "void", "var", "return", "scan", "print", "program", "cond", "then", "let", "int"};
const string delimiters = ":.();{},[]";
string tokenTypes[] = { "identifierToken", "digitToken", "delimiterToken", "operatorToken", "EOFToken", "reservedWordToken" };
int delimiterIndex;
int operatorIndex;



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
/*s10|9*/    {  -1,   -1,   11,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s11|20*/   {  -1,   -1,   12,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s12|11*/   {  -1,   -1,   13,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s13|12*/   {  -1,   -1,   14,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s14|13*/   {  -1,   -1,   15,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s15|14*/   {  -1,   -1,   16,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s16|15*/   {  -1,   -1,   17,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s17|16*/   {  -1,   -1, 1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,   1001,    1001,  1001,   1001,    1001,   1001, 1001, 1001 },
/*s18|17*/   {  18,   18,   18,     18,     18,     18,     18,     18,     18,     18,     18,     18,     18,     18,     18,     18,      18,    18,     18,      18,     18,   18,   18 },
/*s19|18*/   {1002, 1002, 1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*s20|19*/   {1002, 1002, 1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*s21|20*/   {1002, 1002, 1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*s22|21*/   {1002, 1002, 1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
/*s23|22*/   {1002, 1002, 1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,   1002,    1002,  1002,   1002,    1002,   1002, 1002, 1002 },
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

void checkCharacter(partialToken_t token){
  

   // int index = lowerCase.find(tokenFragment.characterToCheck);
    
    //check if we have a character still needing processed
    //copy data from tokenNextFragment to tokenFragment so rest of logic can execute the same
    if (tokenNextFragment.needToProcess == true){
        tokenFragment.characterToCheck = tokenNextFragment.characterToCheck;
        tokenFragment.lineNumberCharacterOn = tokenNextFragment.lineNumberCharacterOn;
        tokenFragment.charType = tokenNextFragment.charType;
        tokenNextFragment.needToProcess = false;
    }

    if (tokenFragment.charType == lower){
        if (stateTable[stateIndex][1] != error){
            tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + tokenFragment.characterToCheck;
            tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
            stateIndex = (stateTable[stateIndex][1]);
        } else {
            cout << "Error: " << tokenCurrent.tokenInstance + tokenFragment.characterToCheck << " is not valid." << endl;
            exit(EXIT_FAILURE);
        }
        if (stateIndex >= 1000){
            processFinalTokenState();
            checkCharacter(tokenNextFragment);
        } else {
            stateIndex = stateIndex - 1;
        }
    }
    else if (tokenFragment.charType == upper){
        if (stateTable[stateIndex][0] != error){
            tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + tokenFragment.characterToCheck;
            tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
            stateIndex = (stateTable[stateIndex][0]);
        } else {
            cout << "Error Expected lowercase, digit, delimiter, or operator.  Received uppercase." << endl;
            exit(EXIT_FAILURE);
        }
        if (stateIndex >= 1000){
            processFinalTokenState();
            checkCharacter(tokenNextFragment);
        } else {
            stateIndex = stateIndex - 1;
        }
    }
    else if (tokenFragment.charType == digit){
        if (stateTable[stateIndex][2] != error){
            tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + tokenFragment.characterToCheck;
            tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
            stateIndex = (stateTable[stateIndex][2]);
        } else {
            cout << "Error: Expected digit, delimiter or operator.  Received letter." << endl;
            exit(EXIT_FAILURE);
        }
        if (stateIndex >= 1000){
            processFinalTokenState();
            checkCharacter(tokenNextFragment);
        } else {
            stateIndex = stateIndex - 1;
        }
    }
    else if (tokenFragment.charType == delimiter) {
      //  int delimiterIndex = int(delimiters.find(tokenFragment.characterToCheck));
        switch (tokenFragment.characterToCheck){
                //:.();{},[]
            case ':':
                delimiterIndex = 6;
                break;
            case '.':
                delimiterIndex = 12;
                break;
            case '(':
                delimiterIndex = 13;
                break;
            case ')':
                delimiterIndex = 14;
                break;
            case ';':
                delimiterIndex = 15;
                break;
            case '{':
                delimiterIndex = 16;
                break;
            case '}':
                delimiterIndex = 17;
                break;
            case ',':
                delimiterIndex = 18;
                break;
            case '[':
                delimiterIndex = 19;
                break;
            case ']':
                delimiterIndex = 20;
                break;
        }
        if (stateTable[stateIndex][delimiterIndex]  != error) {
            tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + tokenFragment.characterToCheck;
            tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
            previousStateIndex = stateIndex;
            stateIndex = stateTable[stateIndex][delimiterIndex];
        } else {
            //error
        }
        if (stateIndex  >= 1000) {
            processFinalTokenState();
            checkCharacter(tokenNextFragment);
        } else {
            stateIndex = stateIndex - 1;
        }
    }
    else if (tokenFragment.charType == operators) {
     //   int operatorIndex = int(operatorss.find(tokenFragment.characterToCheck));
        switch (tokenFragment.characterToCheck){
                // +-*/%<>=
            case '+':
                operatorIndex = 7;
                break;
            case '-':
                operatorIndex = 8;
                break;
            case '*':
                operatorIndex = 9;
                break;
            case '/':
                operatorIndex = 10;
                break;
            case '%':
                operatorIndex = 11;
                break;
            case '<':
                operatorIndex = 4;
                break;
            case '>':
                operatorIndex = 5;
                break;
            case '=':
                operatorIndex = 3;
                break;
                
        }
        if (stateTable[stateIndex][operatorIndex] != error) {
            tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + tokenFragment.characterToCheck;
            tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
            stateIndex = stateTable[stateIndex][operatorIndex];
        } else {
            //error
        }
        if (stateIndex >= 1000) {
            processFinalTokenState();
            checkCharacter(tokenNextFragment);

        } else {
            stateIndex = stateIndex - 1;
        }
    }
    else if (tokenFragment.charType == whitespace){
        if (stateTable[stateIndex][21] != error){
            tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + tokenFragment.characterToCheck;
            tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
            stateIndex = (stateTable[stateIndex][21]);
        } else {
            //error
        }
        if (stateIndex >= 1000) {
            processFinalTokenState();
            checkCharacter(tokenNextFragment);
        } else {
            stateIndex = stateIndex - 1;
        }
    }
    else if (tokenFragment.characterToCheck == EOF){
        tokenCurrent.tokenID = EOFToken;
        tokenCurrent.tokenInstance = "EOF ";
        tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
        processFinalTokenState();


    }
    else {
        //error
    }
   
}
void processFinalTokenState (){
    determineTokenType(stateIndex);
    // need to retain the character that was being processed when the state is in a complete state
    // restore the tokenInstance before the endd
    if (tokenCurrent.tokenInstance.length() > 1){
        tokenCurrent.tokenInstance.pop_back();
        if (!isspace(tokenNextFragment.characterToCheck)){
            tokenNextFragment.characterToCheck = tokenFragment.characterToCheck;
            tokenNextFragment.lineNumberCharacterOn = tokenFragment.lineNumberCharacterOn;
            tokenNextFragment.charType = tokenFragment.charType;
            tokenNextFragment.needToProcess = true;
        }
    }
    // since we have a finished token, next time a character is checked, its the start of a new token
    stateIndex = 0;
    
    filter2();
    printToken();
    clearTokenCurrent();
}

void clearTokenCurrent(){
    tokenCurrent.lineNumber = 0;
    tokenCurrent.tokenInstance = "";
}

void printToken(){
    cout << "[ " << tokenTypes[tokenCurrent.tokenID] << "|" << tokenCurrent.tokenInstance << "|" << tokenCurrent.lineNumber << " ]" << endl;
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


void scanner(partialToken_t token){
//    cout << tokenFragment.characterToCheck << '\t' << tokenFragment.lineNumberCharacterOn << endl;
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
        else if (isupper(workingCharacter)){
            tokenFragment.charType = upper;
        }
        else if (isdigit(workingCharacter)){
            tokenFragment.charType = digit;
        }
        else if (workingCharacter == '+' || workingCharacter == '-' || workingCharacter == '/' || workingCharacter == '*' || workingCharacter == '%' || workingCharacter == '=' || workingCharacter == '>' || workingCharacter == '<'){
            tokenFragment.charType = operators;
        }
        else if (workingCharacter == ':' || workingCharacter == '.' || workingCharacter == '(' || workingCharacter == ')' || workingCharacter == ';' || workingCharacter == '{' || workingCharacter == '}' || workingCharacter == ',' || workingCharacter == '[' || workingCharacter == ']'){
            tokenFragment.charType = delimiter;
        }
        else if (isspace(workingCharacter)){
            tokenFragment.charType = whitespace;
        }
        else if (workingCharacter == EOF){
            tokenFragment.charType = eof;
        } else {
            cout << "Error: " << workingCharacter << " is not a valid character." << endl;
            exit(EXIT_FAILURE);
        }
        
        scanner(tokenFragment);
        
    }
    
}

void executeScanner(char workingCharacter, int lineNumber){
    filter1(workingCharacter, lineNumber);
    
}

void filter2(){
    //remove leading space that sometimes is included
    if (tokenCurrent.tokenInstance[0] == ' ' || tokenCurrent.tokenInstance[0] == '\n'){
        tokenCurrent.tokenInstance.replace(0,1,"");
    }
    //remove trailing \n that sometimes is included
    if ((tokenCurrent.tokenInstance.length()) == '\n'){
        tokenCurrent.tokenInstance.pop_back();
    }
    if (tokenCurrent.tokenID == identifierToken){
        for (int i = 0; i <= 11; i++){
            if (tokenCurrent.tokenInstance == reservedWords[i]) {
                tokenCurrent.tokenID = reservedWordToken;
            }
        }
    } else {
        //nothing
    }



}




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

const string reservedWords[11] = {"iter", "void", "var", "return", "scan", "print", "program", "cond", "then", "let", "int"};
int delimiterIndex;
int operatorIndex;

// columns (valid characters)  as follows:
// orignal table contined column for every operator and delimiter. However, I do not think that the scanner cares WHICH delimiter/operator it sees on that it does see one
// uppercase    lowercase    digit     =    <      >     :    +    -    *    /    %    .    (    )    ;    {    }    ,    [    ]    WS    &
//rows (states) as follows: 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24
// specs say assume no digit or identifer is longer then 8 characters.  I was not sure if I needed to control for that.  my FSA does.
// state 18 as originally planned to be used for comments.  Unused in this version
const int stateTable[][7] = {
    
    
    //         0        1      2    3       4       5       6
    //          UC,   LC,    D,    opr,    del  ,      WS,
 /*s1|0*/     {  -1,    2,   10,    19,        20,      1 },
/*s2|1*/     {   3,    3,    3,   1000,      1000,   1000, },
/*s3|2*/     {   4,    4,    4,   1000,      1000,   1000, },
/*s4|3*/     {   5,    5,    5,   1000,      1000,   1000, },
/*s5|4*/     {   6,    6,    6,   1000,      1000,   1000, },
/*s6|5*/     {   7,    7,    7,   1000,      1000,   1000, },
/*s7|6*/     {   8,    8,    8,   1000,      1000,   1000, },
/*s8|7*/     {   9,    9,    9,   1000,      1000,   1000, },
/*s9|8*/     {  -1,   -1,   -1,   1000,      1000,   1000, },  //state 9 when another character receives is an error.  specs say assume inditifiers no longer then 8
/*s10|9*/    {  -1,   -1,   11,   1001,      1001,   1001, },
/*s11|20*/   {  -1,   -1,   12,   1001,      1001,   1001, },
/*s12|11*/   {  -1,   -1,   13,   1001,      1001,   1001, },
/*s13|12*/   {  -1,   -1,   14,   1001,      1001,   1001, },
/*s14|13*/   {  -1,   -1,   15,   1001,      1001,   1001, },
/*s15|14*/   {  -1,   -1,   16,   1001,      1001,   1001, },
/*s16|15*/   {  -1,   -1,   17,   1001,      1001,   1001, },
/*s17|16*/   {  -1,   -1,   -1,   1001,      1001,   1001, },  // state 18 when another digit receives is an error.  specs say assume digits no longer then 8
/*s18|17*/   {  }, //UNUSED.  Originally for comments
/*s19|18*/   {1002, 1002, 1002,   1002,      1002,   1002, },
/*s20|19*/   {1003, 1003, 1003,   1003,      1003,   1003, }
    
    // key: <0 = Error, < 1000 = go to new state, > 1000 final state as follows: 1000 = idToken, 1001 = numToken, 1002 = operatorToken, 1003 = delimiterToken
    
};

char workingCharacter;
token_t tokenCurrent;
partialToken_t tokenNextFragment;
partialToken_t tokenFragment;
token_t finalToken;
int stateIndex = 0; //start at index 0/state 1
int lineNumber = 1; //start at 1
bool isTokenComplete = false;

void checkCharacter(partialToken_t token){
    
    //check if we have a character still needing processed
    //copy data from tokenNextFragment to tokenFragment so rest of logic can execute the same
    if (tokenNextFragment.needToProcess == true){
        tokenFragment.characterToCheck = tokenNextFragment.characterToCheck;
        tokenFragment.lineNumberCharacterOn = tokenNextFragment.lineNumberCharacterOn;
        tokenFragment.charType = tokenNextFragment.charType;
        tokenNextFragment.needToProcess = false;
    }

    if (tokenFragment.charType == lower){
        if (stateTable[stateIndex][lower] != error){
            processCharacter(lower);
        } else {
            cout << "Error: " << tokenCurrent.tokenInstance + tokenFragment.characterToCheck << " is not valid." << endl;
            exit(EXIT_SUCCESS);
        }
        if (stateIndex >= 1000){
            processFinalTokenState();
            checkCharacter(tokenNextFragment);
        } else {
            stateIndex = stateIndex - 1;
        }
    }
    else if (tokenFragment.charType == upper){
        if (stateTable[stateIndex][upper] != error){
            processCharacter(upper);
        } else {
            cout << "Error: Invalid Character received on line: " << lineNumber << "while processing " << tokenFragment.characterToCheck << endl;
            exit(EXIT_SUCCESS);
        }
        if (stateIndex >= 1000){
            processFinalTokenState();
            checkCharacter(tokenNextFragment);
        } else {
            stateIndex = stateIndex - 1;
        }
    }
    else if (tokenFragment.charType == digit){
        if (stateTable[stateIndex][digit] != error){
            processCharacter(digit);
        } else {
            cout << "Error: Invalid Character received on line: " << lineNumber << "while processing " << tokenFragment.characterToCheck << endl;
            exit(EXIT_SUCCESS);
        }
        if (stateIndex >= 1000){
            processFinalTokenState();
            checkCharacter(tokenNextFragment);
        } else {
            stateIndex = stateIndex - 1;
        }
    }
    else if (tokenFragment.charType == delimiters) {
        if (stateTable[stateIndex][delimiters]  != error) {
            processCharacter(delimiters);
        } else {
            cout << "Error: Invalid Character received on line: " << lineNumber << "while processing " << tokenFragment.characterToCheck << endl;
            exit(EXIT_SUCCESS);
        }
        if (stateIndex  >= 1000) {
            processFinalTokenState();
            checkCharacter(tokenNextFragment);
        } else {
            stateIndex = stateIndex - 1;
        }
    }
    else if (tokenFragment.charType == operators) {
        if (stateTable[stateIndex][operators] != error) {
            processCharacter(operators);
        } else {
            cout << "Error: Invalid Character received on line: " << lineNumber << "while processing " << tokenFragment.characterToCheck << endl;
            exit(EXIT_SUCCESS);
        }
        if (stateIndex >= 1000) {
            processFinalTokenState();
            checkCharacter(tokenNextFragment);

        } else {
            stateIndex = stateIndex - 1;
        }
    }
    else if (tokenFragment.charType == whitespace){
        if (stateTable[stateIndex][5] != error){
            processCharacter(whitespace);
        } else {
            cout << "Error: Invalid Character received on line: " << lineNumber << "while processing " << tokenFragment.characterToCheck << endl;
            exit(EXIT_SUCCESS);
        }
        if (stateIndex >= 1000) {
            processFinalTokenState();
            checkCharacter(tokenNextFragment);
        } else {
            stateIndex = stateIndex - 1;
        }
    }
    else if (tokenFragment.characterToCheck == EOF){
        //special case if EOF
        tokenCurrent.tokenID = EOFToken;
        tokenCurrent.tokenInstance = "EOF ";
        tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
        processFinalTokenState();
    }
    else {
        cout << "Error: Invalid Character received on line: " << lineNumber << "while processing " << tokenFragment.characterToCheck << endl;
        exit(EXIT_SUCCESS);
    }
   
}

void processCharacter (int state){
    tokenCurrent.tokenInstance = tokenCurrent.tokenInstance + tokenFragment.characterToCheck;
    tokenCurrent.lineNumber = tokenFragment.lineNumberCharacterOn;
    stateIndex = (stateTable[stateIndex][state]);
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
    
    isTokenComplete = true;
    // prepare the token struct that will be sent to the calling function for the scanner
    finalToken.tokenID = tokenCurrent.tokenID;
    finalToken.tokenInstance = tokenCurrent.tokenInstance;
    finalToken.lineNumber = tokenCurrent.lineNumber;
    clearTokenCurrent();
}

token_t getFinalToken(){
    return finalToken;
}

void clearTokenCurrent(){
    tokenCurrent.tokenInstance = "";
}

void printToken(token_t token){
    cout << "[ " << tokenTypes[token.tokenID] << "|" << token.tokenInstance << "|" << token.lineNumber << " ]" << endl;
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

void filter1(char workingCharacter){
    isTokenComplete = false;
    

    // filter found a line starting with a comment
    if (workingCharacter == '&'){
        tokenFragment.isPartOfComment = true;
    }
    // keep receiving characters but do nothing with them if the comment hasn't been terminated yet
    if (tokenFragment.isPartOfComment == true){
        // do nothing, keep reading until \n is read
        if (workingCharacter == '\n'){
            //also increment line number since end of a comment means a new line after
            lineNumber++;   
            tokenFragment.isPartOfComment = false;
        }
        
    } else {
        tokenFragment.characterToCheck = workingCharacter;
        tokenFragment.lineNumberCharacterOn = lineNumber;

        // if character is \n, increment.  also EOF is on its line, so increment
        if (workingCharacter == '\n'){
            lineNumber++;
        }
        
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
            tokenFragment.charType = delimiters;
        }
        else if (isspace(workingCharacter)){
            tokenFragment.charType = whitespace;
        }
        else if (workingCharacter == EOF){
            tokenFragment.charType = eof;
        } else {
            cout << "Error: " << workingCharacter << " is not a valid character." << endl;
            exit(EXIT_SUCCESS);
        }
        
        scanner(tokenFragment);
        
    }
    
}

void executeScanner(char workingCharacter){
	filter1(workingCharacter);
    
}

void filter2(){
    //remove leading space that sometimes is included
    do {
        if (tokenCurrent.tokenInstance[0] == ' ' || tokenCurrent.tokenInstance[0] == '\n'){
            tokenCurrent.tokenInstance.replace(0,1,"");
        }
    } while (isspace(tokenCurrent.tokenInstance[0]));
    //remove trailing \n that sometimes is included
    if ((tokenCurrent.tokenInstance.length()) == '\n'){
        tokenCurrent.tokenInstance.pop_back();
    }
    if (tokenCurrent.tokenID == identifierToken){
	    for (int i = 0; i <= 10; i++){
            if (tokenCurrent.tokenInstance == reservedWords[i]) {
                tokenCurrent.tokenID = reservedWordToken;
            }
        }
    } else {
        //nothing
    }
    


}




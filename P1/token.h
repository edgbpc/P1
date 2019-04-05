//
//  token.h
//  P1
//
//  Created by Eric Goodwin on 3/28/19.
//  Copyright © 2019 umsl. All rights reserved.
//

#ifndef token_h
#define token_h

#include <map>
#include <string>

using namespace std;

enum tokenID_t{
    identifierToken, digitToken, delimiterToken, operatorToken, EOFToken, reservedWordToken
};
//         0        1      2    3       4       5       6
//          UC,   LC,    D,    opr,    del  ,      WS,

enum charTypes{
    upper, lower, digit, operators, delimiters, whitespace, eof
};

const string tokenTypes[] = { "identifierToken", "digitToken", "delimiterToken", "operatorToken", "EOFToken", "reservedWordToken" };



struct token_t {
    tokenID_t tokenID;
    string tokenInstance;
    int lineNumber;
};

struct partialToken_t{
    char characterToCheck;
    int lineNumberCharacterOn;
    bool isPartOfComment;
    bool fragmentCompleted;
    charTypes charType;
    bool needToProcess;
    
};




#endif /* token_h */


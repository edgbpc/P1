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

enum charTypes{
    lower, upper, digit, delimiter, operators, whitespace, eof
};



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


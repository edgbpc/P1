//
//  token.h
//  P1
//
//  Created by Eric Goodwin on 3/28/19.
//  Copyright © 2019 umsl. All rights reserved.
//

#ifndef token_h
#define token_h

#include <string>

using namespace std;

enum tokenID_t{
    identifierToken, digitToken, delimiterToken, operatorToken, EOFToken, reservedWordToken, comment
};

enum charTypes{
    lower, upper, digit, delimiter, operators, newLine, eof
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
    
};




#endif /* token_h */


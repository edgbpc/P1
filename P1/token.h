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
    identiferToken, digitToken, delimiterToken, operatorToken, EOFToken, reservedWordToken
};

struct token_t {
    tokenID_t tokenID;
    string tokenInstance;
    int lineNumber;
};



#endif /* token_h */


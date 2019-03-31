//
//  scanner.hpp
//  P1
//
//  Created by Eric Goodwin on 3/28/19.
//  Copyright © 2019 umsl. All rights reserved.
//

#ifndef scanner_h
#define scanner_h

#include <stdio.h>
#include "token.h"

void checkCharacter(partialToken_t tokenFragment);
void scanner(partialToken_t tokenFragment);
void filter1(char workingCharacter, int lineNumber);
bool filter2(token_t token);
void determineTokenType(int stateIndex);
void printToken();
void clearTokenCurrent();
void processFinalTokenState();






#endif /* scanner_h */

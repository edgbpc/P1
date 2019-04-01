//
//  testScanner.hpp
//  P1
//
//  Created by Eric Goodwin on 3/30/19.
//  Copyright © 2019 umsl. All rights reserved.
//

#ifndef testScanner_h
#define testScanner_h

#include <iostream>
#include <fstream>
#include <stdio.h>
#include "token.h"
#include "scanner.h"

class testScanner {
    public:
        testScanner();
        void runTestScanner(ifstream & fileToRead);
        void printToken();

    
    private:
    
    
    
};

#endif /* testScanner_h */

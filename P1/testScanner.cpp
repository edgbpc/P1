//
//  testScanner.cpp
//  P1
//
//  Created by Eric Goodwin on 3/30/19.
//  Copyright © 2019 umsl. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "scanner.h"
#include "testScanner.h"
#include "token.h"

using namespace std;



testScanner::testScanner(){
    
}

void testScanner::runTestScanner(ifstream  & fileToRead){
    int lineNumber = 1; //start on line 1
    char character;
 
    // if \n encountered, increment lineNumber
    while (fileToRead.get(character)){
        if (character == '\n') {
            lineNumber++;
        }
        
        filter1(character, lineNumber);
    }

    
}



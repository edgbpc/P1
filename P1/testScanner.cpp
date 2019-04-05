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

token_t receivedToken;

testScanner::testScanner(){
    
}

void testScanner::runTestScanner(ifstream  & fileToRead){
    
	if (fileToRead.eof()) {
        // end of file reach, testScanner is done
        return;
    }  else {
        char character = fileToRead.get();
	    executeScanner(character);
       
        if (isTokenComplete == true) {
            receivedToken = getFinalToken();
            ::printToken(receivedToken);
        }
    }
    runTestScanner(fileToRead);
}



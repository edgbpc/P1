//
//  main.cpp
//  P1
//
//  Created by Eric Goodwin on 3/28/19.
//  Copyright © 2019 umsl. All rights reserved.
//

#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "token.h"
#include "testScanner.h"

using namespace std;

int main(int argc, const char * argv[]) {
    ifstream fileToRead; //get the data to be processed
    ofstream fileToWrite;
    
    string fileName;
    string outFileNameBase;
    string outFileName;
    string data; //variable that will be used in the tree
    string word;
    
    testScanner myTestScanner;
  
    switch (argc)
    {
            //keyboard input or redirection input
        case 1:
            cout << "Keyboard or Redirection Input Mode" << endl;
            fileToWrite.open("temp.dat");
            //hitting enter will end input.  simulated end of file?
            while (getline(cin, data)){
                fileToWrite << data << endl;
            }
            outFileNameBase = "out";
            
            //will need to work on redirection on delmar
            fileToRead.open("temp.dat");
            
            break;
            //file input
        case 2:
            cout << "File Input Mode" << endl;
            fileName = argv[1];
            fileName.append(".sp19");
            cout << fileName << endl;
            cout << "after append" << endl;

	    fileToRead.open(fileName.c_str());
	    cout << "after fileToRead.open()" << endl; 
            outFileNameBase = argv[1];
            cout << "after outFileNameBase" << endl;
            break;
            
        default:
            cout << "Error:  Incorrect Parameters.  Exiting" << endl;
            return 0;
            break;
    }



  if (fileToRead.good()){
     myTestScanner.runTestScanner(fileToRead);
 }
        fileToRead.close();
    
    //clean up the temporary file created
    remove("temp.dat");

    return 0;
    
//} else {
 //   cout << "Error: Could not read file " << fileName <<  ". Exiting." << endl;
 //       return 0;
//    }
}

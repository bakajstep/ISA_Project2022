//
// Created by sbakaj on 10/11/22.
//

#include <cstdlib>
#include <iostream>
#include "error.hpp"

using namespace std;

void exit_with_error(int error, std::string message){
    cerr << message << endl;
    exit(error);
}
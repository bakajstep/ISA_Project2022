//
// Created by sbakaj on 10/11/22.
//


#include <iostream>
#include "parameters.h"

using namespace std;

int main(int argc, char* argv[]) {
    read_parameters(argc, argv);

    cout << get_file() << endl;
    cout << get_active_timer() << endl;
    cout << get_address() << endl;
    cout << get_count() << endl;
    cout << get_inactive_timer() << endl;
    return 0;
}

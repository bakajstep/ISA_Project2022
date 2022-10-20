//
// Created by sbakaj on 10/1/22.
//

#ifndef ISA_PROJECT2022_PARAMETERS_H
#define ISA_PROJECT2022_PARAMETERS_H

#include <string>

using namespace std;

void read_parameters(int argc, char* argv[]);

string get_file();

string get_address();

int get_port();

int get_active_timer();

int get_inactive_timer();

int get_count();

void free_params();

#endif //ISA_PROJECT2022_PARAMETERS_H

//
// Created by sbakaj on 10/1/22.
//

#ifndef ISA_PROJECT2022_PARAMETERS_H
#define ISA_PROJECT2022_PARAMETERS_H

#include <string>

using namespace std;

typedef struct Parameters {
    string file;
    string ip_address;
    int port;
    int active_timer;
    int inactive_timer;
    int count;
} *param_ptr;

void read_parameters(int argc, char* argv[]);

string get_file();

string get_address();

int get_port();

int get_active_timer();

int get_inactive_timer();

int get_count();

void free_params();

#endif //ISA_PROJECT2022_PARAMETERS_H

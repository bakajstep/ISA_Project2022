//
// Created by sbakaj on 10/1/22.
//

#ifndef ISA_PROJECT2022_PARAMETERS_H
#define ISA_PROJECT2022_PARAMETERS_H

#include <string>

typedef struct Parameters {
    std::string file;
    std::string ip_address;
    int port;
    int active_timer;
    int inactive_timer;
    int count;
} *param_ptr;

void read_parameters(int argc, char* argv[]);

std::string get_file();

std::string get_address();

int get_port();

int get_active_timer();

int get_inactive_timer();

int get_count();

void free_params();

#endif //ISA_PROJECT2022_PARAMETERS_H

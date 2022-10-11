//
// Created by sbakaj on 10/9/22.
//

#include <cstdlib>
#include <unistd.h>
#include "parameters.h"
#include "error.hpp"

using namespace std;


param_ptr params;

void init() {
    params = (param_ptr) malloc(sizeof(struct Parameters));
    params->file = "-";
    params->ip_address = "127.0.0.1";
    params->port = 2555;
    params->active_timer = 60;
    params->inactive_timer = 10;
    params->count = 10;
}


void read_parameters(int argc, char* argv[]) {
    init();
    int opt;
    string argument;

    while((opt = getopt(argc, argv, ":f:c:a:i:m:")) != -1)
    {
        switch(opt)
        {
            case 'f':
                params->file = optarg;
                break;
            case 'c':
                argument = optarg;
                if (argument.find(':') != string::npos){
                    
                } else {
                    params->ip_address = optarg;
                }
                break;
            case 'a':
                params->active_timer = atoi(optarg);
                break;
            case 'i':
                params->inactive_timer = atoi(optarg);
                break;
            case 'm':
                params->count = atoi(optarg);
                break;
            case ':':
                free_params();
                exit_with_error(Error::E_PARAMETER, "Parameter without value.");
                break;
            case '?':
                free_params();
                exit_with_error(Error::E_PARAMETER, "Unknown parameter.");
                break;
        }
    }
}

string get_file() {
    return params->file;
}

string get_address() {
    return params->ip_address;
}

int get_active_timer() {
    return params->active_timer;
}

int get_inactive_timer() {
    return params->inactive_timer;
}

int get_count() {
    return params->count;
}

void free_params() {
    free(params);
}



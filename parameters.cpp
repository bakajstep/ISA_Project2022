//
// Created by sbakaj on 10/9/22.
//

#include <cstdlib>
#include <unistd.h>
#include "parameters.h"
#include "error.hpp"

using namespace std;
string file;
string ip_address;
int port;
int active_timer;
int inactive_timer;
int count;

void init() {
    file = "-";
    ip_address = "127.0.0.1";
    port = 2555;
    active_timer = 60;
    inactive_timer = 120;
    count = 20;
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
                file = optarg;
                break;
            case 'c':
                argument = optarg;
                if (argument.find(':') != string::npos){
                    
                } else {
                    ip_address = optarg;
                }
                break;
            case 'a':
                active_timer = atoi(optarg);
                break;
            case 'i':
                inactive_timer = atoi(optarg);
                break;
            case 'm':
                count = atoi(optarg);
                break;
            case ':':
                exit_with_error(Error::E_PARAMETER, "Parameter without value.");
                break;
            case '?':
                exit_with_error(Error::E_PARAMETER, "Unknown parameter.");
                break;
        }
    }
}

string get_file() {
    return file;
}

string get_address() {
    return ip_address;
}

int get_active_timer() {
    return active_timer;
}

int get_inactive_timer() {
    return inactive_timer;
}

int get_count() {
    return count;
}

int get_port() {
    return port;
}



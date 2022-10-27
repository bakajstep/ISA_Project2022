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
int number_of_flows;

void init() {
    file = "-";
    ip_address = "127.0.0.1";
    port = 2055;
    active_timer = 60;
    inactive_timer = 10;
    number_of_flows = 1024;
}


void read_parameters(int argc, char *argv[]) {
    init();
    int opt;
    string argument;

    while ((opt = getopt(argc, argv, ":f:c:a:i:m:")) != -1) {
        switch (opt) {
            case 'f':
                file = optarg;
                break;
            case 'c':
                argument = optarg;
                if (argument.find(':') != string::npos) {
                    int pos = (int) argument.find(':');
                    ip_address = argument.substr(0, pos);
                    port = stoi(argument.substr(pos + 1));
                } else {
                    ip_address = optarg;
                }
                break;
            case 'a':
                active_timer = stoi(optarg);
                break;
            case 'i':
                inactive_timer = stoi(optarg);
                break;
            case 'm':
                number_of_flows = stoi(optarg);
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
    return number_of_flows;
}

int get_port() {
    return port;
}



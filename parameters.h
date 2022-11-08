//
// Created by sbakaj on 10/1/22.
//

#ifndef ISA_PROJECT2022_PARAMETERS_H
#define ISA_PROJECT2022_PARAMETERS_H

#include <string>

using namespace std;

/**
 * Parse arguments from from program.
 */
void read_parameters(int argc, char* argv[]);

/**
 * Get pcap file.
 */
string get_file();

/**
 * Get ip address where flows needs to be exported.
 */
string get_address();

/**
 * Get port.
 */
int get_port();

/**
 * Get active timer in seconds.
 */
int get_active_timer();

/**
 * Get inactive timer in seconds.
 */
int get_inactive_timer();

/**
 * Get size of flow cache.
 */
int get_count();

#endif //ISA_PROJECT2022_PARAMETERS_H

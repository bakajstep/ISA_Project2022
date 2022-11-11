//
// Created by sbakaj on 10/13/22.
//

#ifndef ISA_PROJECT2022_FLOWS_H
#define ISA_PROJECT2022_FLOWS_H

#include <tuple>
#include <string>
#include <map>
#define __FAVOR_BSD
#include <netinet/ip.h>


using namespace std;

typedef tuple<string, string, uint16_t, uint16_t, uint8_t, uint8_t> key_flow;

typedef struct Flow_Header {
    uint16_t version;
    uint16_t count;
    uint32_t sys_uptime;
    uint32_t unix_secs;
    uint32_t unix_nsecs;
    uint32_t flow_sequence;
    uint8_t engine_type;
    uint8_t engine_id;
    uint16_t sampling_interval;
} flow_header;

typedef struct Flow_Record {
    struct in_addr srcaddr;
    struct in_addr dstaddr;
    uint32_t nexthop;
    uint16_t input;
    uint16_t output;
    uint32_t dPkts;
    uint32_t dOctets;
    uint32_t first;
    uint32_t last;
    uint16_t srcport;
    uint16_t dstport;
    uint8_t pad1;
    uint8_t tcp_flags;
    uint8_t prot;
    uint8_t tos;
    uint16_t src_as;
    uint16_t dst_as;
    uint8_t src_mask;
    uint8_t dst_mask;
    uint16_t pad2;
} flow_record;

typedef struct Flow {
    flow_header header;
    flow_record record[30];
} flow;

/**
 * Create key to map of records.
 */
tuple<string, string, uint16_t, uint16_t, uint8_t, uint8_t> create_key(
        const string& ip_sc, const string& ip_dest, uint16_t p_sc, uint16_t p_dest, uint8_t protocol, uint8_t tos);

/**
 * Update record in map.
 */
void update_flow(const tuple<string, string, uint16_t, uint16_t, uint8_t, uint8_t> &key, time_t time, uint8_t tcp_flag,
                 uint32_t dOctets);

/**
 * Create record in map.
 */
void create_flow(const tuple<string, string, uint16_t, uint16_t, uint8_t, uint8_t> &key, in_addr ip_source, in_addr ip_desc,
                 uint16_t port_source, uint16_t port_desc, uint8_t protocol, uint8_t tos, uint32_t time, uint8_t tcp_flag,
                 uint32_t dOctets);

/**
 * Check if a given key is existing in map.
 */
bool is_exist_flow(const tuple<string, string, uint16_t, uint16_t, uint8_t, uint8_t>& key);

/**
 * Delete record with given key from map.
 */
void delete_flow(const tuple<string, string, uint16_t, uint16_t, uint8_t, uint8_t>& key);

/**
 * Export records from list.
 */
void export_flows(uint32_t time, uint32_t secs, uint32_t nsec);

/**
 * Export rest of the records after pcap file end.
 */
void export_rest(uint32_t time, uint32_t secs, uint32_t nsec);

/**
 * Check if some records needs to be exported due to inactive timer.
 */
void check_inactive_time(uint32_t time);

/**
 * Check if some records needs to be exported due to active timer.
 */
void check_active_time(uint32_t time);


/**
 * Add records to export list with given key.
 */
void add_flow_to_export(const tuple<string, string, uint16_t, uint16_t, uint8_t, uint8_t>& key);
#endif //ISA_PROJECT2022_FLOWS_H

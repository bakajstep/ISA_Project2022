//
// Created by sbakaj on 10/13/22.
//

#include <arpa/inet.h>
#include <cstdint>
#include <list>
#include <cstring>
#include <vector>
#include "flows.h"
#include "parameters.h"
#include "udp_client.h"


map<tuple<string, string, int, int, uint8_t, uint8_t>, flow_record> map_of_records; // map of flows
int current_number_of_flows = 0; // number of flows
list<flow_record> records_to_export;

tuple<string, string, int, int, uint8_t, uint8_t> get_key_flow(flow_record record) {
    return make_tuple(inet_ntoa(record.srcaddr), inet_ntoa(record.dstaddr),
                      record.srcport, record.dstport, record.prot, record.tos);
}


flow_record find_oldest_record() {
    flow_record oldest_record;
    uint32_t oldest = UINT32_MAX;
    for (const auto &pr: map_of_records) {
        if (oldest < ntohl(pr.second.first)) {
            oldest = pr.second.first;
            oldest_record = pr.second;
        }
    }
    return oldest_record;
}


tuple<string, string, int, int, uint8_t, uint8_t> create_key(
        const string &ip_sc, const string &ip_dest, int p_sc, int p_dest, uint8_t protocol, uint8_t tos) {
    return make_tuple(ip_sc, ip_dest, p_sc, p_dest, protocol, tos);
}

void create_flow(const tuple<string, string, int, int, uint8_t, uint8_t> &key, in_addr ip_source, in_addr ip_desc,
                 uint8_t port_source, uint8_t port_desc, uint8_t protocol, uint8_t tos, uint32_t time, uint8_t tcp_flag,
                 uint32_t dOctets) {
    flow_record record;
    if (++current_number_of_flows == get_count()) {
        record = find_oldest_record();
        key_flow tmp_key = get_key_flow(record);
        records_to_export.push_back(record);
        delete_flow(tmp_key);
    }

    record.srcaddr = ip_source;
    record.dstaddr = ip_desc;
    record.nexthop = 0;
    record.input = 0;
    record.output = 0;
    record.dPkts = htonl(1);
    record.dOctets = htonl(dOctets);
    record.first = htonl(time);
    record.last = htonl(time);
    record.srcport = port_source;
    record.dstport = port_desc;
    record.pad1 = 0;
    record.tcp_flags = 0 | tcp_flag;
    record.prot = protocol;
    record.tos = tos;
    record.src_as = 0;
    record.dst_as = 0;
    record.src_mask = 0;
    record.dst_mask = 0;
    record.pad2 = 0;
    map_of_records[key] = record;
}

void update_flow(const tuple<string, string, int, int, uint8_t, uint8_t> &key, time_t time, uint8_t tcp_flag,
                 uint32_t dOctets) {
    flow_record record = map_of_records[key];
    record.last = htonl(time);
    record.dPkts += htonl(1);
    record.dOctets += htonl(dOctets);
    record.tcp_flags = record.tcp_flags | tcp_flag;
    map_of_records[key] = record;
}

bool is_exist_flow(const tuple<string, string, int, int, uint8_t, uint8_t> &key) {
    if (map_of_records.find(key) != map_of_records.end()) {
        return true;
    }
    return false;
}

void delete_flow(const tuple<string, string, int, int, uint8_t, uint8_t> &key) {
    map_of_records.erase(key);
}

void check_inactive_time(uint32_t time) {
    vector<key_flow> to_delete;
    for (auto &map_of_record: map_of_records) {
        if ((time - ntohl(map_of_record.second.last)) > (get_inactive_timer() * (uint32_t) 1000)) {
            records_to_export.push_back(map_of_record.second);
            to_delete.push_back(map_of_record.first);
        }
    }
    for (auto &i: to_delete) {
        delete_flow(i);
    }

}

void check_active_time(uint32_t time) {
    vector<key_flow> to_delete;
    for (const auto &pr: map_of_records) {
        if ((time - ntohl(pr.second.first)) > (get_active_timer() * (uint32_t) 1000)) {
            records_to_export.push_back(pr.second);
            to_delete.push_back(pr.first);
        }
    }
    for (auto &i: to_delete) {
        delete_flow(i);
    }
}

flow_header create_flow_header(uint32_t time, uint32_t secs, uint32_t nsec) {
    flow_header header;
    header.version = htons(5);
    header.count = htons(1);
    header.sys_uptime = htonl(time);
    header.unix_secs = htonl(secs);
    header.unix_nsecs = htonl(nsec);
    header.engine_type = 0;
    header.engine_id = 0;
    header.sampling_interval = 0;
    return header;
}


void export_flows(uint32_t time, uint32_t secs, uint32_t nsec) {
    if (records_to_export.empty()) {
        return;
    }
    flow flow;
    flow_header header = create_flow_header(time, secs, nsec);
    int count = 0;
    int msg_size;
    list<flow_record>::iterator it;
    for (it = records_to_export.begin(); it != records_to_export.end(); ++it) {
        flow.record[count++] = *it;
        if (count == 30) {
            header.count = htons(count);
            flow.header = header;
            msg_size = int(sizeof(flow_header) + count * sizeof(flow_record));
            send_to_client(&flow, sizeof(flow));
            count = 0;
        }
    }

    if (count != 0) {
        header.count = htons(count);
        flow.header = header;
        msg_size = int(sizeof(flow_header) + count * sizeof(flow_record));
        send_to_client(&flow, msg_size);
    }
}

void export_rest(uint32_t time, uint32_t secs, uint32_t nsec) {
    for (const auto &pr: map_of_records) {
        records_to_export.push_back(pr.second);
    }
    export_flows(time, secs, nsec);
}
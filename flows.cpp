//
// Created by sbakaj on 10/13/22.
//

#include <arpa/inet.h>
#include <cstdint>
#include <list>
#include "flows.h"
#include "parameters.h"


map<tuple<string, string, int, int, uint8_t, uint8_t>, flow> map_of_flows; // map of flows
int current_number_of_flows = 0; // number of flows
list<flow> flows_to_export;

tuple<string, string, int, int, uint8_t, uint8_t> get_key_flow(flow flow) {
    return make_tuple(inet_ntoa(flow.record.srcaddr), inet_ntoa(flow.record.dstaddr),
                      flow.record.srcport, flow.record.dstport, flow.record.prot, flow.record.tos);
}


flow find_oldest_flow() {
    flow oldest_flow;
    uint32_t oldest = UINT32_MAX;
    for (const auto &pr: map_of_flows) {
        if (oldest < pr.second.record.first) {
            oldest = pr.second.record.first;
            oldest_flow = pr.second;
        }
    }
    return oldest_flow;
}


tuple<string, string, int, int, uint8_t, uint8_t> create_key(
        const string &ip_sc, const string &ip_dest, int p_sc, int p_dest, uint8_t protocol, uint8_t tos) {
    return make_tuple(ip_sc, ip_dest, p_sc, p_dest, protocol, tos);
}

void create_flow(const tuple<string, string, int, int, uint8_t, uint8_t> &key, in_addr ip_source, in_addr ip_desc,
                 uint8_t port_source, uint8_t port_desc, uint8_t protocol, uint8_t tos, uint32_t time, uint8_t tcp_flag,
                 uint32_t dOctets) {
    flow flow;
    if (++current_number_of_flows == get_count()) {
        flow = find_oldest_flow();
        key_flow tmp_key = get_key_flow(flow);
        //export_flow();
        delete_flow(tmp_key);
    }
    flow.header.version = 5;
    flow.header.count = 1;
    flow.header.sys_uptime = 0;     // TODO
    flow.header.unix_secs = 0;  // TODO
    flow.header.unix_nsecs = 0;     // TODO
    flow.header.sampling_interval = 0;      // TODO
    flow.header.engine_type = 0;
    flow.header.engine_id = 0;
    flow.header.sampling_interval = 0;

    flow.record.srcaddr = ip_source;
    flow.record.dstaddr = ip_desc;
    flow.record.nexthop = 0;
    flow.record.input = 0;
    flow.record.output = 0;
    flow.record.dPkts = 1;
    flow.record.dOctets = dOctets;
    flow.record.first = time * 1000;
    flow.record.last = time * 1000;
    flow.record.srcport = port_source;
    flow.record.dstport = port_desc;
    flow.record.pad1 = 0;
    flow.record.tcp_flags = 0 | tcp_flag;
    flow.record.prot = protocol;
    flow.record.tos = tos;
    flow.record.src_as = 0;
    flow.record.dst_as = 0;
    flow.record.src_mask = 0;
    flow.record.dst_mask = 0;
    flow.record.pad2 = 0;
    map_of_flows[key] = flow;
}

void update_flow(const tuple<string, string, int, int, uint8_t, uint8_t> &key, time_t time, uint8_t tcp_flag,
                 uint32_t dOctets) {
    flow flow = map_of_flows[key];
    flow.record.last = time;
    flow.record.dPkts += 1;
    flow.record.dOctets += dOctets;
    flow.record.tcp_flags = flow.record.tcp_flags | tcp_flag;
    map_of_flows[key] = flow;
}

bool is_exist_flow(const tuple<string, string, int, int, uint8_t, uint8_t> &key) {
    if (map_of_flows.find(key) != map_of_flows.end()) {
        return true;
    }
    return false;
}

void delete_flow(const tuple<string, string, int, int, uint8_t, uint8_t> &key) {
    map_of_flows.erase(key);
}

void check_inactive_time(uint32_t time) {
    for (const auto &pr: map_of_flows) {
        if ((time - pr.second.record.last) > (get_inactive_timer() * (uint32_t) 1000)) {
            flows_to_export.push_back(pr.second);
            delete_flow(pr.first);
        }
    }
}

void check_active_time(uint32_t time) {
    for (const auto &pr: map_of_flows) {
        if ((time - pr.second.record.first) > (get_active_timer() * (uint32_t) 1000)) {
            flows_to_export.push_back(pr.second);
            delete_flow(pr.first);
        }
    }
}

void export_flows() {

}
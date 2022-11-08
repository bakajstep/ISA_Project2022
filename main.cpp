//
// Created by sbakaj on 10/11/22.
//

#include "parameters.h"
#include "error.hpp"
#include "flows.h"
#include <pcap/pcap.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <sys/socket.h>
#include <netinet/ether.h>
#include <cstring>

#define PCAP_ERRBUF_SIZE 256
#define SIZE_ETHERNET 14
#define ICMP_DESTINATION_PORT(type, code) (type * 256 + code)
#define CALCULATE_TIME_TO_MS(time_s, time_ns) ((time_s * 1000) + ((time_ns + 500)/1000))

using namespace std;

int main(int argc, char *argv[]) {
    read_parameters(argc, argv);
    char errbuf[PCAP_ERRBUF_SIZE];  // constant defined in pcap.h
    const u_char *packet;
    struct ip *my_ip;
    const struct tcphdr *my_tcp;    // pointer to the beginning of TCP header
    const struct udphdr *my_udp;    // pointer to the beginning of UDP header
    const struct icmphdr *my_icmp;  // pointer to the beginning of ICMP header
    struct pcap_pkthdr header{};
    struct ether_header *eptr;
    pcap_t *handle;                 // file/device handler
    u_int size_ip;
    key_flow key;
    uint32_t first_time = 0;
    uint32_t current_packet_time;

    if ((handle = pcap_open_offline(get_file().c_str(), errbuf)) == nullptr)
        exit_with_error(Error::E_PCAP_FILE_READ, "Can't open file");
    // read packets from the file
    while ((packet = pcap_next(handle, &header)) != nullptr) {
        if (first_time == 0) {
            first_time = CALCULATE_TIME_TO_MS(header.ts.tv_sec, header.ts.tv_usec);
        }
        current_packet_time = CALCULATE_TIME_TO_MS(header.ts.tv_sec, header.ts.tv_usec) - first_time;
        check_inactive_time(current_packet_time);
        check_active_time(current_packet_time);

        // read the Ethernet header
        eptr = (struct ether_header *) packet;

        if (ntohs(eptr->ether_type) == ETHERTYPE_IP) {
            my_ip = (struct ip *) (packet + SIZE_ETHERNET);        // skip Ethernet header
            size_ip = my_ip->ip_hl * 4;                           // length of IP header
            switch (my_ip->ip_p) {
                case 1: // ICMP protocol
                    my_icmp = (struct icmphdr *) (packet + SIZE_ETHERNET + size_ip); // pointer to the ICMP header
                    key = create_key(inet_ntoa(my_ip->ip_src), inet_ntoa(my_ip->ip_dst), 0,
                                     ICMP_DESTINATION_PORT(my_icmp->type, my_icmp->code), my_ip->ip_p,
                                     my_ip->ip_tos);
                    if (is_exist_flow(key)) {
                        update_flow(key, current_packet_time, 0, my_ip->ip_len);
                    } else {
                        create_flow(key, my_ip->ip_src, my_ip->ip_dst, 0,
                                    ICMP_DESTINATION_PORT(my_icmp->type, my_icmp->code), my_ip->ip_p, my_ip->ip_tos,
                                    current_packet_time, 0, my_ip->ip_len);
                    }
                    export_flows(current_packet_time, header.ts.tv_sec, header.ts.tv_usec);
                    break;
                case 6: // TCP protocol
                    my_tcp = (struct tcphdr *) (packet + SIZE_ETHERNET + size_ip); // pointer to the TCP header
                    key = create_key(inet_ntoa(my_ip->ip_src), inet_ntoa(my_ip->ip_dst),
                                     ntohs(my_tcp->th_sport), ntohs(my_tcp->th_dport),
                                     my_ip->ip_p, my_ip->ip_tos);
                    if (is_exist_flow(key)) {
                        update_flow(key, current_packet_time, my_tcp->th_flags, my_ip->ip_len);
                    } else {
                        create_flow(key, my_ip->ip_src, my_ip->ip_dst, ntohs(my_tcp->th_sport),
                                    ntohs(my_tcp->th_dport), my_ip->ip_p, my_ip->ip_tos,
                                    current_packet_time, my_tcp->th_flags, my_ip->ip_len);
                    }
                    if (my_tcp->th_flags & TH_FIN || my_tcp->th_flags & TH_RST) {
                        add_flow_to_export(key);
                    }
                    export_flows(current_packet_time, header.ts.tv_sec, header.ts.tv_usec);
                    break;
                case 17: // UDP protocol
                    my_udp = (struct udphdr *) (packet + SIZE_ETHERNET + size_ip); // pointer to the UDP header
                    key = create_key(inet_ntoa(my_ip->ip_src), inet_ntoa(my_ip->ip_dst), ntohs(my_udp->uh_sport),
                                     ntohs(my_udp->uh_dport), my_ip->ip_p, my_ip->ip_tos);
                    if (is_exist_flow(key)) {
                        update_flow(key, current_packet_time, 0, my_ip->ip_len);
                    } else {
                        create_flow(key, my_ip->ip_src, my_ip->ip_dst, ntohs(my_udp->uh_sport),
                                    ntohs(my_udp->uh_dport), my_ip->ip_p, my_ip->ip_tos,
                                    current_packet_time, 0, my_ip->ip_len);
                    }
                    export_flows(current_packet_time, header.ts.tv_sec, header.ts.tv_usec);
                    break;
                default:
                    continue;
            }
        }
    }
    // close the capture device and deallocate resources
    pcap_close(handle);
    export_rest(current_packet_time, header.ts.tv_sec, header.ts.tv_usec);
    return Error::E_NO_ERR;
}

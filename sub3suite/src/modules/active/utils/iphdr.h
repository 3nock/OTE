#ifndef IPHDR_H
#define IPHDR_H

#if defined (Q_OS_WIN)

#include <WinSock2.h>
#include <in6addr.h>
#include <stdint.h>


// Align on a 1-byte boundary
#pragma pack(push, 1)

// IPv4 header
typedef struct ip_hdr {
    unsigned char  ip_verlen;        // 4-bit IPv4 version
                                     // 4-bit header length (in 32-bit words)
    unsigned char  ip_tos;           // IP type of service
    unsigned short ip_totallength;   // Total length
    unsigned short ip_id;            // Unique identifier 
    unsigned short ip_offset;        // Fragment offset field
    unsigned char  ip_ttl;           // Time to live
    unsigned char  ip_protocol;      // Protocol(TCP,UDP etc)
    unsigned short ip_checksum;      // IP checksum
    unsigned int   ip_srcaddr;       // Source address
    unsigned int   ip_destaddr;      // Source address
} IPV4_HDR, *PIPV4_HDR, *LPIPV4_HDR;

// IPv4 option header
typedef struct ipv4_option_hdr {
    unsigned char   opt_code;           // option type
    unsigned char   opt_len;            // length of the option header
    unsigned char   opt_ptr;            // offset into options
    unsigned long   opt_addr[9];        // list of IPv4 addresses
} IPV4_OPTION_HDR, *PIPV4_OPTION_HDR, *LPIPV4_OPTION_HDR;

// ICMP header
typedef struct icmp_hdr {
    unsigned char   icmp_type;
    unsigned char   icmp_code;
    unsigned short  icmp_checksum;
    unsigned short  icmp_id;
    unsigned short  icmp_sequence;
} ICMP_HDR, *PICMP_HDR, *LPICMP_HDR;

// IPv6 protocol header
typedef struct ipv6_hdr {
    unsigned long   ipv6_vertcflow;        // 4-bit IPv6 version
                                           // 8-bit traffic class
                                           // 20-bit flow label
    unsigned short  ipv6_payloadlen;       // payload length
    unsigned char   ipv6_nexthdr;          // next header protocol value
    unsigned char   ipv6_hoplimit;         // TTL 
    struct in6_addr ipv6_srcaddr;          // Source address
    struct in6_addr ipv6_destaddr;         // Destination address
} IPV6_HDR, *PIPV6_HDR, *LPIPV6_HDR;

// IPv6 fragment header
typedef struct ipv6_fragment_hdr {
    unsigned char   ipv6_frag_nexthdr;
    unsigned char   ipv6_frag_reserved;
    unsigned short  ipv6_frag_offset;
    unsigned long   ipv6_frag_id;
} IPV6_FRAGMENT_HDR, *PIPV6_FRAGMENT_HDR, *LPIPV6_FRAGMENT_HDR;

// ICMPv6 header
typedef struct icmpv6_hdr {
    unsigned char   icmp6_type;
    unsigned char   icmp6_code;
    unsigned short  icmp6_checksum;
} ICMPV6_HDR;

// ICMPv6 echo request body
typedef struct icmpv6_echo_request {
    unsigned short  icmp6_echo_id;
    unsigned short  icmp6_echo_sequence;
} ICMPV6_ECHO_REQUEST;

// Define the UDP header 
typedef struct udp_hdr {
    unsigned short src_portno;       // Source port no.
    unsigned short dst_portno;       // Dest. port no.
    unsigned short udp_length;       // Udp packet length
    unsigned short udp_checksum;     // Udp checksum (optional)
} UDP_HDR, *PUDP_HDR;

// IPv4 option for record route
#define IP_RECORD_ROUTE     0x7

// ICMP6 protocol value (used in the socket call and IPv6 header)
#define IPPROTO_ICMP6       58

// ICMP types and codes
#define ICMPV4_ECHO_REQUEST_TYPE   8
#define ICMPV4_ECHO_REQUEST_CODE   0
#define ICMPV4_ECHO_REPLY_TYPE     0
#define ICMPV4_ECHO_REPLY_CODE     0
#define ICMPV4_MINIMUM_HEADER      8

// ICPM6 types and codes
#define ICMPV6_ECHO_REQUEST_TYPE   128
#define ICMPV6_ECHO_REQUEST_CODE   0
#define ICMPV6_ECHO_REPLY_TYPE     129
#define ICMPV6_ECHO_REPLY_CODE     0

// Restore byte alignment back to default
#pragma pack(pop)


namespace s3s_headers {

struct ip {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    unsigned int ip_hl:4;               /* header length */
    unsigned int ip_v:4;                /* version */
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    unsigned int ip_v:4;                /* version */
    unsigned int ip_hl:4;               /* header length */
#endif
    uint8_t ip_tos;                       /* type of service */
    u_short ip_len;                       /* total length */
    u_short ip_id;                        /* identification */
    u_short ip_off;                       /* fragment offset field */
#define        IP_RF 0x8000                        /* reserved fragment flag */
#define        IP_DF 0x4000                        /* dont fragment flag */
#define        IP_MF 0x2000                        /* more fragments flag */
#define        IP_OFFMASK 0x1fff                   /* mask for fragmenting bits */
    uint8_t ip_ttl;                      /* time to live */
    uint8_t ip_p;                        /* protocol */
    u_short ip_sum;                      /* checksum */
    struct in_addr ip_src, ip_dst;       /* source and dest address */
};

struct tcphdr {
    uint16_t source;
    uint16_t dest;
    uint32_t seq;
    uint32_t ack_seq;
#  if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    uint16_t res1:4;
    uint16_t doff:4;
    uint16_t fin:1;
    uint16_t syn:1;
    uint16_t rst:1;
    uint16_t psh:1;
    uint16_t ack:1;
    uint16_t urg:1;
    uint16_t res2:2;
#  elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    uint16_t doff:4;
    uint16_t res1:4;
    uint16_t res2:2;
    uint16_t urg:1;
    uint16_t ack:1;
    uint16_t psh:1;
    uint16_t rst:1;
    uint16_t syn:1;
    uint16_t fin:1;
#  else
#   error "Adjust your <bits/endian.h> defines"
#  endif
    uint16_t window;
    uint16_t check;
    uint16_t urg_ptr;
};

struct iphdr {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    unsigned int ihl:4;
    unsigned int version:4;
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    unsigned int version:4;
    unsigned int ihl:4;
#else
# error        "Please fix <bits/endian.h>"
#endif
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
    /*The options start here. */
};

struct pseudo_header    //needed for checksum calculation
{
    unsigned int source_address;
    unsigned int dest_address;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short tcp_length;

    struct tcphdr tcp;
};
}

#endif // WINDOWS
#endif // IPHDR_H

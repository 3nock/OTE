#ifndef Scanner_H
#define Scanner_H

#include "AbstractScanner.h"
#include "src/items/HostItem.h"

#include <QMutex>
#include <QQueue>
#include <QTcpSocket>
#include <QWaitCondition>

#if defined(Q_OS_WIN)
#include <WinSock2.h>
#endif

#if defined(SYN_SCAN)

#if defined(Q_OS_WIN)
#include <WinSock2.h>
#include <Windows.h>
#include <pcap.h>
#include <ws2def.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>
#include <tchar.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")

#include "utils/iphdr.h"

union time_union {
    FILETIME fileTime;
    ULARGE_INTEGER ul;
};

// Constants for Ethernet section of packet
#define ETHERNET_IPV4          0x0800

// Constants for IP section of packet
#define IP_VIHL                0x45
#define IP_DSCP_ECN            0x00
#define IP_LENGTH              0x002C
#define IP_ID                  0x568E // Check this, can it not be zero?
#define IP_FLAGS_FOFFSET       0x0000
#define IP_TIME_TO_LIVE        0x32
#define IP_TCP                 0x06
#define IP_INITIAL_CHECKSUM    0x0000

// Constants for TCP section of packets
#define TCP_SEQ_NO             0x00000000
#define TCP_ACK_NO             0x00000000
#define TCP_OFFSET             0x60
#define TCP_FLAGS_SYN          0x02  // Syn only
#define TCP_FLAGS_RST          0x04  // Reset only
#define TCP_FLAGS_ACK          0x10  // Ack only
#define TCP_WINDOW_SIZE        0x0400
#define TCP_URGENT_PTR         0x0000
#define TCP_MAX_SEG_SIZE       0x02
#define TCP_LENGTH             0x04
#define TCP_MSS_VALUE          0x05B4
#define TCP_INITIAL_CHECKSUM   0x0000

// Constants for internal use
#define DEVICE_STRING_SIZE       100
#define MAX_PORTS                65536
#define PACKET_SIZE              58
#define MAC_ADAPTER_LENGTH       6
#define IP_ADDRESS_LENGTH        4
#define IP_ADDRESS_STRING_LENGTH 16
#define PSEUDO_HEADER_SIZE       12
#define SOURCE_PORT              54321

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#endif // WINDOWS

#if defined(Q_OS_UNIX)

#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>

struct pseudo_header { //Needed for checksum calculation
    unsigned int source_address;
    unsigned int dest_address;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short tcp_length;

    struct tcphdr tcp;
};

#endif // UNIX

#endif // SYN SCAN

namespace port {

enum ScanType{
    CONNECTION,
    SYN
};

struct ScanArgs {
    QMutex mutex;
    QSet<u_short> target_ports;
    QQueue<QString> target_ips;
    ScanType scan_type;

    int timeout = 3000;
    int progress = 0;
    bool is_host = false;
    bool is_ip = false;
};

class Scanner: public AbstractScanner {
    Q_OBJECT

    public:
        explicit Scanner(port::ScanArgs *args);
        ~Scanner() override;

    signals:
        void scanResult_ip(QString ip, u_short port);
        void scanResult_host(QString hostname, QString ip, u_short port);

    public slots:
        virtual void onStopScan() override {
            stop = true;
        }

        void onPauseScan() override {
            m_mutex.lock();
            pause = true;
            m_mutex.unlock();
        }

        void onResumeScan() override {
            m_mutex.lock();
            pause = false;
            m_mutex.unlock();
            m_wait.wakeAll();
        }

    private slots:
        void lookup() override;

    private:
        port::ScanArgs *m_args;
        QString m_target;
        bool stop = false;
        bool pause = false;
        QWaitCondition m_wait;
        QMutex m_mutex;

        void scanner_connection();
        void scanner_syn();

        /* for syn scanner */
#if defined(SYN_SCAN)

#if defined(Q_OS_WIN_)
        pcap_t* fp = nullptr;
        char device_name[200];
        char *source_ip_address = nullptr;
        char *dest_ip_address = nullptr;
        char *default_gateway = nullptr;
        u_char src_ipv4[4];
        u_char dest_ipv4[4];
        u_char dest_mac_address[MAC_ADAPTER_LENGTH];
        u_char source_mac_address[MAC_ADAPTER_LENGTH];
        u_long src_ip = 0, dest_ip = 0, gate_ip = 0;
        s3s_headers::iphdr *m_iph = nullptr;
        u_short packet_id = 1234;
        int active_packets = 0;
        u_short source_port = 54321;

        int start_syn_scan(QList<u_short> ports);
        int init_syn_scan();
        bool load_npcap_dlls();
        bool get_source_adaptor_details(char device[]);
        bool get_source_adaptor_full_name(char device[]);
        void copy_u_short_to_array(u_char* array, int index, u_short val);
        void copy_u_long_to_array(u_char* array, int index, u_long val);
        u_short calculate_ip_checksum(u_char packet[PACKET_SIZE]);
        u_short calculate_tcp_checksum(u_char packet[PACKET_SIZE]);
        void set_ethernet_fields(u_char packet[PACKET_SIZE]);
        void set_internet_protocol_fields(u_char packet[PACKET_SIZE]);
        void set_tcp_fields(u_char packet[PACKET_SIZE], u_short dest_port);
        bool get_destination_adaptor_details();
        u_int64 calc_time_difference(SYSTEMTIME st1, SYSTEMTIME st2);
        bool get_address(SOCKADDR *sa, size_t salen, char* addr);
#endif  // WINDOWS

#if defined (Q_OS_UNIX)
        int start_syn_scan(char *target, QList<u_short> ports);
        unsigned short check_sum(unsigned short*, int);
        char* hostname_to_ip(char*);
        static void ip_to_host(const char*, char*);
        static int start_sniffer();
        static void* receive_ack(void*);
        static void process_packet(unsigned char*, int, char*);
        void str_to_int(int*, char*, int);
        int get_local_ip(char*);
        void err_exit(char*, ...);
        void prepare_datagram(char*, const char*, struct iphdr*, struct tcphdr*);
        void parse_target(char*, struct in_addr*, int64_t*);

        static struct in_addr dest_ip;
#endif  // UNIX

#endif // SYN SCAN
};

QString getTarget(port::ScanArgs *args);

}

#endif // Scanner_H

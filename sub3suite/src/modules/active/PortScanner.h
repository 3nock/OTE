#ifndef Scanner_H
#define Scanner_H

#include "AbstractScanner.h"
#include "src/items/HostItem.h"

#include <QMutex>
#include <QQueue>
#include <tchar.h>
#include <QTcpSocket>

#if defined(Q_OS_WIN)
#include <WinSock2.h>
#include <Windows.h>
#include <pcap.h>
#include <ws2def.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")

#endif //defined(Q_OS_WIN)

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

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


namespace port {

enum ScanType{
    CONNECTION,
    STEALTH,
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

class Scanner: public AbstractScanner{
    Q_OBJECT

    public:
        explicit Scanner(port::ScanArgs *args);
        ~Scanner() override;

    signals:
        void scanResult_ip(QString ip, QList<u_short> ports);
        void scanResult_host(QString hostname, QString ip, QList<u_short> ports);

    private slots:
        void lookup() override;

    private:
        port::ScanArgs *m_args;
        QString m_target;

        void scanner_connection();
        void scanner_stealth();
        void scanner_syn();

        /* for syn scanner */
        static s3s_struct::HOST host;

        static pcap_t* fp;
        static int total_ports;
        static u_char source_ip_address[IP_ADDRESS_LENGTH];
        static u_char dest_ip_address[IP_ADDRESS_LENGTH];
        static SYSTEMTIME start_time;
        static BOOL packets_received[MAX_PORTS];
        u_short packet_id = 1234;

        void start_syn_scan(char *device, char *target_ip, QList<u_short> ports);
        bool load_npcap_dlls();
        bool get_source_adaptor_details(char device[], u_char mac_address[], u_char ip_address[], u_char default_gateway[]);
        bool get_source_adaptor_full_name(char device[], char device_full_name[]);
        void copy_u_short_to_array(u_char* array, int index, u_short val);
        void copy_u_long_to_array(u_char* array, int index, u_long val);
        u_short calculate_ip_checksum(u_char packet[PACKET_SIZE]);
        u_short calculate_tcp_checksum(u_char packet[PACKET_SIZE]);
        void set_ethernet_fields(u_char packet[PACKET_SIZE], u_char source_mac_address[MAC_ADAPTER_LENGTH], u_char dest_mac_address[MAC_ADAPTER_LENGTH]);
        void set_internet_protocol_fields(u_char packet[PACKET_SIZE]);
        void set_tcp_fields(u_char packet[PACKET_SIZE], u_short dest_port);
        static void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
        bool get_destination_adaptor_details(char* srcIP, char* destIP, u_char dest_mac_address[MAC_ADAPTER_LENGTH]);
        static u_int64 calc_time_difference(SYSTEMTIME st1, SYSTEMTIME st2);
};

QString getTarget(port::ScanArgs *args);

}

#endif // Scanner_H

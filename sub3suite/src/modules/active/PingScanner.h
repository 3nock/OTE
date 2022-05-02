#ifndef PINGSCANNER_H
#define PINGSCANNER_H

#include "AbstractScanner.h"

#include <QMutex>
#include <QQueue>
#include <QWaitCondition>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(Q_OS_WIN)

#include <WinSock2.h>
#include <Windows.h>
#include <ws2ipdef.h>
#include <WS2tcpip.h>

#include "utils/iphdr.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")

#define MAX_RECV_BUF_LEN       0xFFFF   // Max incoming packet size.

#endif // WINDOWS

#if defined(Q_OS_UNIX)
#ifndef _GNU_SOURCE
    #define _GNU_SOURCE /* for additional type definitions */
#endif

#ifdef __APPLE__
    #define __APPLE_USE_RFC_3542 /* for IPv6 definitions on Apple platforms */
#endif

#include <errno.h>
#include <fcntl.h>            /* fcntl() */
#include <netdb.h>            /* getaddrinfo() */
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>        /* inet_XtoY() */
#include <netinet/in.h>       /* IPPROTO_ICMP */
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>  /* struct icmp */
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

typedef int socket_t;
typedef struct msghdr msghdr_t;
typedef struct cmsghdr cmsghdr_t;

#define IP_VERSION_ANY 0
#define IP_V4 4
#define IP_V6 6

#define ICMP_HEADER_LENGTH 8
#define MESSAGE_BUFFER_SIZE 1024

#ifndef ICMP_ECHO
    #define ICMP_ECHO 8
#endif
#ifndef ICMP_ECHO6
    #define ICMP6_ECHO 128
#endif
#ifndef ICMP_ECHO_REPLY
    #define ICMP_ECHO_REPLY 0
#endif
#ifndef ICMP_ECHO_REPLY6
    #define ICMP6_ECHO_REPLY 129
#endif

#define REQUEST_TIMEOUT 1000000
#define REQUEST_INTERVAL 1000000

#define close_socket close

#pragma pack(push, 1)

struct icmp {
    uint8_t icmp_type;
    uint8_t icmp_code;
    uint16_t icmp_cksum;
    uint16_t icmp_id;
    uint16_t icmp_seq;
};

struct ip6_pseudo_hdr {
    struct in6_addr src;
    struct in6_addr dst;
    uint8_t unused1[2];
    uint16_t plen;
    uint8_t unused2[3];
    uint8_t nxt;
};

struct icmp6_packet {
    struct ip6_pseudo_hdr ip6_hdr;
    struct icmp icmp;
};

#pragma pack(pop)

#endif // UNIX

namespace ping {

struct ScanArgs {
    QMutex mutex;
    QQueue<QString> targets;
    int progress = 0;
    int send_count = 1;
    int timeout = 3000;
    int data_size = 32;
    int ttl = 128;
    bool is_host = false;
    bool is_ip = false;
};

class Scanner: public AbstractScanner {
    Q_OBJECT

    public:
        explicit Scanner(ping::ScanArgs *args);
        ~Scanner() override;

    signals:
        void scanResult_ip(QString ip, unsigned long time);
        void scanResult_host(QString host, QString ip, unsigned long time);

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
        ping::ScanArgs *m_args;
        QString m_target;
        bool stop = false;
        bool pause = false;
        QWaitCondition m_wait;
        QMutex m_mutex;

#if defined(Q_OS_WIN)
        int gAddressFamily=AF_UNSPEC;         // Address family to use
        char *gDestination=nullptr;           // Destination
        int recvbuflen = MAX_RECV_BUF_LEN;    // Length of received packets.
        char recvbuf[MAX_RECV_BUF_LEN];       // For received packets

        int ping_win();
        int SetTtl(SOCKET s, int ttl);
        void SetIcmpSequence(char *buf);
        USHORT checksum(USHORT *buffer, int size);
        void InitIcmpHeader(char *buf, int datasize);
        int InitIcmp6Header(char *buf, int datasize);
        QString print_address(SOCKADDR *sa, size_t salen);
        USHORT ComputeIcmp6PseudoHeaderChecksum(SOCKET s, char *icmppacket, int icmplen, struct addrinfo *dest);
        void ComputeIcmpChecksum(SOCKET s, char *buf, int packetlen, struct addrinfo *dest);
        int PostRecvfrom(SOCKET s, char *buf, int buflen, SOCKADDR *from, int *fromlen, WSAOVERLAPPED *ol);
#endif // WINDOWS

#if defined(Q_OS_UNIX)
        void ping_unix();
        uint64_t utime(void);
        uint16_t compute_checksum(const char *buf, size_t size);
#endif // UNIX
};

QString getTarget(ping::ScanArgs *args);

}

#endif // PINGSCANNER_H

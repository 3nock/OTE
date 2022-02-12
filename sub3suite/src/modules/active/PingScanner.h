#ifndef PINGSCANNER_H
#define PINGSCANNER_H

#include "AbstractScanner.h"
#include "src/models/HostModel.h"

#include <QMutex>
#include <QQueue>

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE /* for additional type definitions */
#endif

#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0601 /* for inet_XtoY functions on MinGW */
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

#include <process.h>  /* _getpid() */
#include <WinSock2.h>
#include <WS2tcpip.h> /* getaddrinfo() */
#include <MSWSock.h>  /* WSARecvMsg() */

#undef CMSG_SPACE
#define CMSG_SPACE WSA_CMSG_SPACE
#undef CMSG_FIRSTHDR
#define CMSG_FIRSTHDR WSA_CMSG_FIRSTHDR
#undef CMSG_NXTHDR
#define CMSG_NXTHDR WSA_CMSG_NXTHDR
#undef CMSG_DATA
#define CMSG_DATA WSA_CMSG_DATA

typedef SOCKET socket_t;
typedef WSAMSG msghdr_t;
typedef WSACMSGHDR cmsghdr_t;

/*
 * Pointer to the WSARecvMsg() function. It must be obtained at runtime...
 */
static LPFN_WSARECVMSG WSARecvMsg;

#else /* _WIN32 */

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

#endif /* !_WIN32 */

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

#ifdef _WIN32
    #define socket(af, type, protocol) \
        WSASocketW(af, type, protocol, NULL, 0, 0)
    #define close_socket closesocket
    #define getpid _getpid
    #define usleep(usec) Sleep((DWORD)((usec) / 1000))
#else
    #define close_socket close
#endif

#pragma pack(push, 1)

#if defined _WIN32 || defined __CYGWIN__

#if defined _MSC_VER || defined __MINGW32__
    typedef unsigned __int8 uint8_t;
    typedef unsigned __int16 uint16_t;
    typedef unsigned __int32 uint32_t;
    typedef unsigned __int64 uint64_t;
#endif

struct icmp {
    uint8_t icmp_type;
    uint8_t icmp_code;
    uint16_t icmp_cksum;
    uint16_t icmp_id;
    uint16_t icmp_seq;
};

#endif /* _WIN32 || __CYGWIN__ */

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

/**
 * Returns a timestamp with microsecond resolution.
 */
static uint64_t utime(void)
{
#ifdef _WIN32
    LARGE_INTEGER count;
    LARGE_INTEGER frequency;
    if (QueryPerformanceCounter(&count) == 0
        || QueryPerformanceFrequency(&frequency) == 0) {
        return 0;
    }
    return count.QuadPart * 1000000 / frequency.QuadPart;
#else
    struct timeval now;
    return gettimeofday(&now, NULL) != 0
        ? 0
        : now.tv_sec * 1000000 + now.tv_usec;
#endif
}

#ifdef _WIN32

static void init_winsock_lib(void)
{
    int error;
    WSADATA wsa_data;

    error = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (error != 0) {
        fprintf(stderr, "Failed to initialize WinSock: %d\n", error);
        exit(EXIT_FAILURE);
    }
}

static void init_winsock_extensions(socket_t sockfd)
{
    int error;
    GUID recvmsg_id = WSAID_WSARECVMSG;
    DWORD size;

    /*
     * Obtain a pointer to the WSARecvMsg (recvmsg) function.
     */
    error = WSAIoctl(sockfd,
                     SIO_GET_EXTENSION_FUNCTION_POINTER,
                     &recvmsg_id,
                     sizeof(recvmsg_id),
                     &WSARecvMsg,
                     sizeof(WSARecvMsg),
                     &size,
                     nullptr,
                     nullptr);
    if (error == SOCKET_ERROR)
        exit(EXIT_FAILURE);
}

#endif /* _WIN32 */

static uint16_t compute_checksum(const char *buf, size_t size)
{
    /* RFC 1071 - http://tools.ietf.org/html/rfc1071 */

    size_t i;
    uint64_t sum = 0;

    for (i = 0; i < size; i += 2) {
        sum += *(uint16_t *)buf;
        buf += 2;
    }
    if (size - i > 0)
        sum += *(uint8_t *)buf;

    while ((sum >> 16) != 0)
        sum = (sum & 0xffff) + (sum >> 16);

    return (uint16_t)~sum;
}

enum PING_TYPE {
    IPV4,
    IPV6,
    ANY
};

namespace ping {

struct ScanArgs {
    QQueue<QString> targets;
    PING_TYPE ping_type;
    QMutex mutex;
};

class Scanner: public AbstractScanner {
        Q_OBJECT

    public:
        Scanner(ping::ScanArgs *args);
        ~Scanner() override;

    private slots:
        void lookup() override;

    signals:
        void scanResult(s3s_struct::HOST host);

    private:
        ping::ScanArgs *m_args;
        QString m_target;
        scan::Log m_log;
};

QString getTarget(ping::ScanArgs *args);

}

#endif // PINGSCANNER_H

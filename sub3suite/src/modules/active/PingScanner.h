#ifndef PINGSCANNER_H
#define PINGSCANNER_H

#include "AbstractScanner.h"

#include <QMutex>
#include <QQueue>

#include <WinSock2.h>
#include <Windows.h>
#include <ws2ipdef.h>
#include <WS2tcpip.h>

#include "utils/iphdr.h"
#include "utils/resolve.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")

#define DEFAULT_DATA_SIZE      32       // default data size
#define DEFAULT_SEND_COUNT     1        // number of ICMP requests to send
#define DEFAULT_RECV_TIMEOUT   6000     // six second
#define DEFAULT_TTL            128
#define MAX_RECV_BUF_LEN       0xFFFF   // Max incoming packet size.

namespace ping {

struct ScanArgs {
    QMutex mutex;
    QQueue<QString> targets;
    int progress = 0;
    int send_count = 1;
    int timeout = 3000;
    int data_size = 32;
    int ttl = 128;
    int max_recv_buf_length = 0xffff;
    bool is_host = false;
    bool is_ip = false;
};

class Scanner: public AbstractScanner {
    Q_OBJECT

    public:
        explicit Scanner(ping::ScanArgs *args);
        ~Scanner() override;

    signals:
        void scanResult_host(QString host, QString ip, int bytes, int time, int ttl);
        void scanResult_ip(QString ip, int bytes, int time, int ttl);

    private slots:
        void lookup() override;

    private:
        ping::ScanArgs *m_args;
        QString m_target;

        int gAddressFamily=AF_UNSPEC;         // Address family to use
        int gProtocol=IPPROTO_ICMP;           // Protocol value
        int gTtl=DEFAULT_TTL;                 // Default TTL value
        int gDataSize=DEFAULT_DATA_SIZE;      // Amount of data to send
        BOOL bRecordRoute=FALSE;              // Use IPv4 record route?
        char *gDestination=nullptr;           // Destination
        int recvbuflen = MAX_RECV_BUF_LEN;    // Length of received packets.
        char recvbuf[MAX_RECV_BUF_LEN];       // For received packets

        USHORT ip_checksum(USHORT* buffer, int size);
        void InitIcmpHeader(char *buf, int datasize);
        int InitIcmp6Header(char *buf, int datasize);
        USHORT checksum(USHORT *buffer, int size);
        void SetIcmpSequence(char *buf);
        USHORT ComputeIcmp6PseudoHeaderChecksum(SOCKET s, char *icmppacket, int icmplen, struct addrinfo *dest);
        void ComputeIcmpChecksum(SOCKET s, char *buf, int packetlen, struct addrinfo *dest);
        int PostRecvfrom(SOCKET s, char *buf, int buflen, SOCKADDR *from, int *fromlen, WSAOVERLAPPED *ol);
        int SetTtl(SOCKET s, int ttl);
        int start_scan();
};

QString getTarget(ping::ScanArgs *args);

}

#endif // PINGSCANNER_H

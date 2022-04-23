#include "PingScanner.h"


QString ping::getTarget(ping::ScanArgs *args){
    QMutexLocker(&args->mutex);

    if(!args->targets.isEmpty())
        return args->targets.dequeue();
    else
        return nullptr;
}

ping::Scanner::Scanner(ping::ScanArgs *args): AbstractScanner (),
    m_args(args)
{
}
ping::Scanner::~Scanner(){
}

void ping::Scanner::lookup(){
    m_target = ping::getTarget(m_args);

    /*
    set address family:
        gAddressFamily = AF_INET;
        gAddressFamily = AF_INET6;

    set ttl value:
        gTtl = atoi(128);

    set buffer size:
        gDataSize = atoi(58);

    set record_route option:
        bRecordRoute = TRUE;
    */

    while(m_target != nullptr)
    {
        /* set current target */
        QByteArray target = m_target.toLocal8Bit();
        gDestination = target.data();

        /* start scan */
        start_scan();

        /* scan progress */
        m_args->progress++;
        emit scanProgress(m_args->progress);

        /* get next target */
        m_target = getTarget(m_args);
    }

    emit quitThread();
}

USHORT ping::Scanner::ip_checksum(USHORT *buffer, int size){
    unsigned long cksum = 0;

    // Sum all the words together, adding the final byte if size is odd
    while(size > 1) {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if(size)
        cksum += *(UCHAR*)buffer;

    // Do a little shuffling
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);

    // Return the bitwise complement of the resulting mishmash
    return static_cast<USHORT>(~cksum);
}


///
/// @Brief:
///   Helper function to fill in various stuff in our ICMP request.
///
void ping::Scanner::InitIcmpHeader(char *buf, int datasize)
{
    ICMP_HDR   *icmp_hdr=nullptr;
    char       *datapart=nullptr;

    icmp_hdr = reinterpret_cast<ICMP_HDR*>(buf);
    icmp_hdr->icmp_type     = ICMPV4_ECHO_REQUEST_TYPE;        // request an ICMP echo
    icmp_hdr->icmp_code     = ICMPV4_ECHO_REQUEST_CODE;
    icmp_hdr->icmp_id       = static_cast<USHORT>(GetCurrentProcessId());
    icmp_hdr->icmp_checksum = 0;
    icmp_hdr->icmp_sequence = 0;

    datapart = buf + sizeof(ICMP_HDR);

    // Place some data in the buffer.
    memset(datapart, 'E', datasize);
}

///
///  @Brief:
///    Initialize the ICMP6 header as well as the echo request header.
///
int ping::Scanner::InitIcmp6Header(char *buf, int datasize) {
    ICMPV6_HDR          *icmp6_hdr=nullptr;
    ICMPV6_ECHO_REQUEST *icmp6_req=nullptr;
    char                *datapart=nullptr;

    // Initialize the ICMP6 headerf ields
    icmp6_hdr = reinterpret_cast<ICMPV6_HDR*>(buf);
    icmp6_hdr->icmp6_type     = ICMPV6_ECHO_REQUEST_TYPE;
    icmp6_hdr->icmp6_code     = ICMPV6_ECHO_REQUEST_CODE;
    icmp6_hdr->icmp6_checksum = 0;

    // Initialize the echo request fields
    icmp6_req = (ICMPV6_ECHO_REQUEST *)(buf + sizeof(ICMPV6_HDR));
    icmp6_req->icmp6_echo_id       = static_cast<USHORT>(GetCurrentProcessId());
    icmp6_req->icmp6_echo_sequence = 0;

    datapart = static_cast<char*>(buf) + sizeof(ICMPV6_HDR) + sizeof(ICMPV6_ECHO_REQUEST);

    memset(datapart, '#', datasize);

    return (sizeof(ICMPV6_HDR) + sizeof(ICMPV6_ECHO_REQUEST));
}

///
///  @Brief:
///    This function calculates the 16-bit one's complement sum
///    of the supplied buffer (ICMP) header.
///
USHORT ping::Scanner::checksum(USHORT *buffer, int size) {
    unsigned long cksum=0;

    while (size > 1) {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size)
        cksum += *(UCHAR*)buffer;

    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    return static_cast<USHORT>(~cksum);
}

///
/// @Brief:
///    This routine sets the sequence number of the ICMP request packet.
///
void ping::Scanner::SetIcmpSequence(char *buf) {
    ULONG    sequence=0;

    sequence = GetTickCount();
    if (gAddressFamily == AF_INET) {
        ICMP_HDR *icmpv4=nullptr;

        icmpv4 = reinterpret_cast<ICMP_HDR*>(buf);
        icmpv4->icmp_sequence = static_cast<USHORT>(sequence);
    }
    else if (gAddressFamily == AF_INET6) {
        ICMPV6_HDR *icmpv6=nullptr;
        ICMPV6_ECHO_REQUEST *req6=nullptr;

        icmpv6 = reinterpret_cast<ICMPV6_HDR*>(buf);
        req6   = (ICMPV6_ECHO_REQUEST *)(buf + sizeof(ICMPV6_HDR));

        req6->icmp6_echo_sequence = static_cast<USHORT>(sequence);
    }
}

///
/// @Brief:
///    This routine computes the ICMP6 checksum which includes the pseudo
///    header of the IPv6 header (see RFC2460 and RFC2463). The one difficulty
///    here is we have to know the source and destination IPv6 addresses which
///    will be contained in the IPv6 header in order to compute the checksum.
///    To do this we call the SIO_ROUTING_INTERFACE_QUERY ioctl to find which
///    local interface for the outgoing packet.
///
USHORT ping::Scanner::ComputeIcmp6PseudoHeaderChecksum(SOCKET s, char *icmppacket, int icmplen, struct addrinfo *dest)
{
    SOCKADDR_STORAGE localif;
    DWORD            bytes;
    char             tmp[MAX_RECV_BUF_LEN] = {'\0'}, *ptr=nullptr, proto=0;
    int              rc, total, length, i;

    // Find out which local interface for the destination
    rc = WSAIoctl(s,SIO_ROUTING_INTERFACE_QUERY,dest->ai_addr,(DWORD)dest->ai_addrlen,(SOCKADDR *)&localif,(DWORD)sizeof(localif),&bytes,nullptr,nullptr);
    if (rc == SOCKET_ERROR){
        qWarning() << "WSAIoctl failed: " << WSAGetLastError();
        return 0xFFFF;
    }

    // We use a temporary buffer to calculate the pseudo header.
    ptr = tmp;
    total = 0;

    // Copy source address
    memcpy(ptr, &((SOCKADDR_IN6 *)&localif)->sin6_addr, sizeof(struct in6_addr));
    ptr   += sizeof(struct in6_addr);
    total += sizeof(struct in6_addr);

    /*
    printf("%x%x%x%x\n",
            ((SOCKADDR_IN6 *) &localif)->sin6_addr.u.Byte[0],
            ((SOCKADDR_IN6 *) &localif)->sin6_addr.u.Byte[1],
            ((SOCKADDR_IN6 *) &localif)->sin6_addr.u.Byte[2],
            ((SOCKADDR_IN6 *) &localif)->sin6_addr.u.Byte[3]
            );
    */

    // Copy destination address
    memcpy(ptr, &((SOCKADDR_IN6 *)dest->ai_addr)->sin6_addr, sizeof(struct in6_addr));
    ptr   += sizeof(struct in6_addr);
    total += sizeof(struct in6_addr);

    // Copy ICMP packet length
    length = static_cast<int>(htonl(static_cast<u_long>(icmplen)));

    memcpy(ptr, &length, sizeof(length));
    ptr   += sizeof(length);
    total += sizeof(length);

    /*
    printf("%x%x%x%x\n",
            (char ) *(ptr - 4),
            (char ) *(ptr - 3),
            (char ) *(ptr - 2),
            (char ) *(ptr - 1)
            );
    */

    // Zero the 3 bytes
    memset(ptr, 0, 3);
    ptr   += 3;
    total += 3;

    // Copy next hop header
    proto = IPPROTO_ICMP6;

    memcpy(ptr, &proto, sizeof(proto));
    ptr   += sizeof(proto);
    total += sizeof(proto);

    // Copy the ICMP header and payload
    memcpy(ptr, icmppacket, static_cast<size_t>(icmplen));
    ptr   += icmplen;
    total += icmplen;

    for(i=0; i < icmplen%2 ;i++){
        *ptr = 0;
        ptr++;
        total++;
    }

    return checksum(reinterpret_cast<USHORT*>(tmp), total);
}

///
/// @Brief:
///    This routine computes the checksum for the ICMP request. For IPv4 its
///    easy, just compute the checksum for the ICMP packet and data. For IPv6,
///    its more complicated. The pseudo checksum has to be computed for IPv6
///    which includes the ICMP6 packet and data plus portions of the IPv6
///    header which is difficult since we aren't building our own IPv6
///    header.
///
void ping::Scanner::ComputeIcmpChecksum(SOCKET s, char *buf, int packetlen, struct addrinfo *dest){
    if (gAddressFamily == AF_INET){
        ICMP_HDR    *icmpv4=nullptr;

        icmpv4 = reinterpret_cast<ICMP_HDR*>(buf);
        icmpv4->icmp_checksum = 0;
        icmpv4->icmp_checksum = checksum(reinterpret_cast<USHORT*>(buf), packetlen);
    }
    else if (gAddressFamily == AF_INET6){
        ICMPV6_HDR  *icmpv6=nullptr;

        icmpv6 = reinterpret_cast<ICMPV6_HDR*>(buf);
        icmpv6->icmp6_checksum = 0;
        icmpv6->icmp6_checksum = ComputeIcmp6PseudoHeaderChecksum(s,buf,packetlen,dest);
    }
}

///
///  @Brief:
///    This routine posts an overlapped WSARecvFrom on the raw socket.
///
int ping::Scanner::PostRecvfrom(SOCKET s, char *buf, int buflen, SOCKADDR *from, int *fromlen, WSAOVERLAPPED *ol){
    WSABUF  wbuf;
    DWORD   flags, bytes;
    int     rc;

    wbuf.buf = buf;
    wbuf.len = static_cast<ULONG>(buflen);

    flags = 0;

    rc = WSARecvFrom(s,&wbuf,1,&bytes,&flags,from,fromlen,ol,nullptr);
    if (rc == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING){
            qWarning() << "WSARecvFrom failed: " << WSAGetLastError();
            return SOCKET_ERROR;
        }
    }
    return NO_ERROR;
}

///
///  @Brief:
///    Sets the TTL on the socket.
///
int ping::Scanner::SetTtl(SOCKET s, int ttl) {
    int     optlevel = 0, option = 0, rc;

    rc = NO_ERROR;
    if(gAddressFamily == AF_INET){
        optlevel = IPPROTO_IP;
        option   = IP_TTL;
    }
    else if(gAddressFamily == AF_INET6){
        optlevel = IPPROTO_IPV6;
        option   = IPV6_UNICAST_HOPS;
    }
    else
        rc = SOCKET_ERROR;

    if(rc == NO_ERROR)
        rc = setsockopt(s,optlevel,option,reinterpret_cast<char*>(&ttl),sizeof(ttl));

    if (rc == SOCKET_ERROR)
        qWarning() << "SetTtl: setsockopt failed: " << WSAGetLastError();

    return rc;
}

///
/// @Brief:
///    Setup the ICMP raw socket and create the ICMP header. Add
///    the appropriate IP option header and start sending ICMP
///    echo requests to the endpoint. For each send and receive we
///    set a timeout value so that we don't wait forever for a
///    response in case the endpoint is not responding. When we
///    receive a packet decode it.
///
int ping::Scanner::start_scan() {
    WSADATA            wsd;
    WSAOVERLAPPED      recvol;
    SOCKET             s=INVALID_SOCKET;
    char              *icmpbuf=nullptr;
    struct addrinfo   *dest=nullptr, *local=nullptr;
    IPV4_OPTION_HDR    ipopt;
    SOCKADDR_STORAGE   from;
    DWORD              bytes, flags;
    int                packetlen=0, fromlen, time=0, rc, i, status = 0;

    recvol.hEvent = WSA_INVALID_EVENT;

    char zero[] = "0";
    char *bind_addr = nullptr;

    // Load Winsock
    if ((rc = WSAStartup(MAKEWORD(2,2), &wsd)) != 0){
        qWarning() << "WSAStartup() failed: " << rc;
        status = -1;
        goto EXIT;
    }

    // Resolve the destination address
    dest = ResolveAddress(gDestination,zero,gAddressFamily,0,0);
    if (dest == nullptr){
        qWarning() << "bad name " << gDestination;
        status = -1;
        goto CLEANUP;
    }
    gAddressFamily = dest->ai_family;

    if (gAddressFamily == AF_INET)
        gProtocol = IPPROTO_ICMP;
    else if (gAddressFamily == AF_INET6)
        gProtocol = IPPROTO_ICMP6;

    // Get the bind address
    local = ResolveAddress(bind_addr,zero,gAddressFamily,0,0);

    if (local == nullptr){
        qWarning() << "Unable to obtain the bind address!";
        status = -1;
        goto CLEANUP;
    }

    // Create the raw socket
    s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    if (s == INVALID_SOCKET){
        qWarning() << "socket failed: " << WSAGetLastError();
        status = -1;
        goto CLEANUP;
    }

    SetTtl(s, gTtl);

    // Figure out the size of the ICMP header and payload
    if (gAddressFamily == AF_INET)
        packetlen += sizeof(ICMP_HDR);
    else if (gAddressFamily == AF_INET6)
        packetlen += sizeof(ICMPV6_HDR) + sizeof(ICMPV6_ECHO_REQUEST);

    // Add in the data size
    packetlen += gDataSize;

    // Allocate the buffer that will contain the ICMP request
    icmpbuf = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, packetlen);
    if (icmpbuf == nullptr){
        qWarning() << "HeapAlloc failed: " << GetLastError();
        status = -1;
        goto CLEANUP;
    }

    // Initialize the ICMP headers
    if (gAddressFamily == AF_INET)
    {
        if (bRecordRoute)
        {
            // Setup the IP option header to go out on every ICMP packet
            ZeroMemory(&ipopt, sizeof(ipopt));
            ipopt.opt_code = IP_RECORD_ROUTE; // record route option
            ipopt.opt_ptr  = 4;               // point to the first addr offset
            ipopt.opt_len  = 39;              // length of option header

            rc = setsockopt(s, IPPROTO_IP, IP_OPTIONS, (char *)&ipopt, sizeof(ipopt));
            if (rc == SOCKET_ERROR){
                qWarning() << "setsockopt(IP_OPTIONS) failed: " << WSAGetLastError();
                status = -1;
                goto CLEANUP;
            }
        }

        InitIcmpHeader(icmpbuf, gDataSize);
    }
    else if (gAddressFamily == AF_INET6)
        InitIcmp6Header(icmpbuf, gDataSize);

    // Bind the socket -- need to do this since we post a receive first
    rc = bind(s, local->ai_addr, (int)local->ai_addrlen);
    if (rc == SOCKET_ERROR) {
        qWarning() << "bind failed: " << WSAGetLastError();
        status = -1;
        goto CLEANUP;
    }

    // Setup the receive operation
    memset(&recvol, 0, sizeof(recvol));
    recvol.hEvent = WSACreateEvent();
    if (recvol.hEvent == WSA_INVALID_EVENT){
        qWarning() << "WSACreateEvent failed: " << WSAGetLastError();
        status = -1;
        goto CLEANUP;
    }

    // Post the first overlapped receive
    fromlen = sizeof(from);
    PostRecvfrom(s, recvbuf, recvbuflen, reinterpret_cast<SOCKADDR*>(&from), &fromlen, &recvol);

    qDebug() << "Pinging " << PrintAddress(dest->ai_addr, static_cast<int>(dest->ai_addrlen)) << " with " << gDataSize <<"bytes of data";

    // Start sending the ICMP requests
    for(i=0; i < DEFAULT_SEND_COUNT ;i++)
    {
        // Set the sequence number and compute the checksum
        SetIcmpSequence(icmpbuf);
        ComputeIcmpChecksum(s, icmpbuf, packetlen, dest);

        time = static_cast<int>(GetTickCount());
        rc = sendto(s,icmpbuf,packetlen,0,dest->ai_addr,static_cast<int>(dest->ai_addrlen));
        if (rc == SOCKET_ERROR){
            qWarning() << "sendto failed: " << WSAGetLastError();
            status = -1;
            goto CLEANUP;
        }

        // Waite for a response
        rc = WaitForSingleObject((HANDLE)recvol.hEvent, m_args->timeout);
        if (static_cast<DWORD>(rc) == WAIT_FAILED) {
            qWarning() << "WaitForSingleObject failed: " << GetLastError();
            status = -1;
            goto CLEANUP;
        }
        else if (rc == WAIT_TIMEOUT)
            qDebug() << "Request timed out.";

        else
        {
            rc = WSAGetOverlappedResult(s,&recvol,&bytes,FALSE,&flags);
            if (rc == FALSE)
                qWarning() << "WSAGetOverlappedResult failed: " << WSAGetLastError();

            time = static_cast<int>(GetTickCount()) - time;

            WSAResetEvent(recvol.hEvent);

            if (time == 0){
                if(m_args->is_host)
                    emit scanResult_host(m_target, PrintAddress(reinterpret_cast<SOCKADDR*>(&from), fromlen), gDataSize, 0, gTtl);
                if(m_args->is_ip)
                    emit scanResult_ip(PrintAddress(reinterpret_cast<SOCKADDR*>(&from), fromlen), gDataSize, 0, gTtl);
            }
            else{
                if(m_args->is_host)
                    emit scanResult_host(m_target, PrintAddress(reinterpret_cast<SOCKADDR*>(&from), fromlen), gDataSize, time, gTtl);
                if(m_args->is_host)
                    emit scanResult_ip(PrintAddress(reinterpret_cast<SOCKADDR*>(&from), fromlen), gDataSize, time, gTtl);
            }

            if (i < DEFAULT_SEND_COUNT - 1){
                fromlen = sizeof(from);
                PostRecvfrom(s, recvbuf, recvbuflen, (SOCKADDR *)&from, &fromlen, &recvol);
            }
        }
    }

CLEANUP:
    if (dest)
         freeaddrinfo(dest);
    if (local)
         freeaddrinfo(local);
    if (s != INVALID_SOCKET)
        closesocket(s);
    if (recvol.hEvent != WSA_INVALID_EVENT)
        WSACloseEvent(recvol.hEvent);
    if (icmpbuf)
        HeapFree(GetProcessHeap(), 0, icmpbuf);

    WSACleanup();

EXIT:
    return status;
}

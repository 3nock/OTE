#include "PingScanner.h"


#if defined(Q_OS_UNIX)

int ping::Scanner::ping() {
    int error;
    socket_t sockfd = -1;
    struct addrinfo *addrinfo_list = nullptr;
    struct addrinfo *addrinfo;
    char addr_str[INET6_ADDRSTRLEN] = "<unknown>";
    struct sockaddr_storage addr;
    socklen_t dst_addr_len;
    uint16_t id = (uint16_t)getpid();
    uint16_t seq = random();
    int optlevel = 0, option = 0;

    /* get ping type */
    int ip_version = IP_V4;
    if (ip_version == IP_V4 || ip_version == IP_VERSION_ANY) {
        struct addrinfo hints;
        hints.ai_flags = 0;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_RAW;
        hints.ai_protocol = IPPROTO_ICMP;
        error = getaddrinfo(m_target.toUtf8(),nullptr,&hints,&addrinfo_list);
    }
    if (ip_version == IP_V6
        || (ip_version == IP_VERSION_ANY && error != 0)) {
        struct addrinfo hints;
        hints.ai_flags = 0;
        hints.ai_family = AF_INET6;
        hints.ai_socktype = SOCK_RAW;
        hints.ai_protocol = IPPROTO_ICMPV6;
        error = getaddrinfo(m_target.toUtf8(),nullptr,&hints,&addrinfo_list);
    }
    if (error != 0){
        log.message = "getaddrinfo() failed!";
        goto exit_error;
    }

    for (addrinfo = addrinfo_list;
        addrinfo != nullptr;
        addrinfo = addrinfo->ai_next) {
        sockfd = socket(addrinfo->ai_family,addrinfo->ai_socktype,addrinfo->ai_protocol);
        if (sockfd >= 0)
            break;
    }
    if ((int)sockfd < 0){
        log.message = "socket() failed!";
        goto exit_error;
    }

    memcpy(&addr, addrinfo->ai_addr, addrinfo->ai_addrlen);
    dst_addr_len = (socklen_t)addrinfo->ai_addrlen;

    freeaddrinfo(addrinfo_list);
    addrinfo = NULL;
    addrinfo_list = NULL;

    /*
     * Switch the socket to non-blocking I/O mode. This allows us to implement
     * the timeout feature.
     */
    if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1){
        log.message = "fcntl() failed!";
        goto exit_error;
    }

    if (addr.ss_family == AF_INET6) {
        /*
         * This allows us to receive IPv6 packet headers in incoming messages.
         */
        int opt_value = 1;
        error = setsockopt(sockfd,IPPROTO_IPV6,IPV6_RECVPKTINFO,(char *)&opt_value,sizeof(opt_value));
        if (error != 0){
            log.message = "setsockopt() failed!";
            goto exit_error;
        }
    }

    /*
     * Convert the destination IP-address to a string.
     */
    inet_ntop(addr.ss_family,
              addr.ss_family == AF_INET6
                  ? (void *)&((struct sockaddr_in6 *)&addr)->sin6_addr
                  : (void *)&((struct sockaddr_in *)&addr)->sin_addr,
              addr_str,
              sizeof(addr_str));

    struct icmp request;
    request.icmp_type = addr.ss_family == AF_INET6 ? ICMP6_ECHO : ICMP_ECHO;
    request.icmp_code = 0;
    request.icmp_cksum = 0;
    request.icmp_id = htons(id);
    request.icmp_seq = htons(seq);

    if (addr.ss_family == AF_INET6) {
        /*
         * Checksum is calculated from the ICMPv6 packet prepended
         * with an IPv6 "pseudo-header".
         *
         * https://tools.ietf.org/html/rfc2463#section-2.3
         * https://tools.ietf.org/html/rfc2460#section-8.1
         */
        struct icmp6_packet request_packet = {0};

        request_packet.ip6_hdr.src = in6addr_loopback;
        request_packet.ip6_hdr.dst = ((struct sockaddr_in6 *)&addr)->sin6_addr;
        request_packet.ip6_hdr.plen = htons((uint16_t)ICMP_HEADER_LENGTH);
        request_packet.ip6_hdr.nxt = IPPROTO_ICMPV6;
        request_packet.icmp = request;

        request.icmp_cksum = compute_checksum((char *)&request_packet,
                                              sizeof(request_packet));
    } else {
        request.icmp_cksum = compute_checksum((char *)&request,
                                              sizeof(request));
    }

    error = (int)sendto(sockfd,
                        (char *)&request,
                        sizeof(request),
                        0,
                        (struct sockaddr *)&addr,
                        (int)dst_addr_len);
    if (error < 0){
        log.message = "sendto() failed!";
        goto exit_error;
    }

    struct timespec start_ts;
    clock_gettime(CLOCK_REALTIME, &start_ts);

    for (;;) {
        char msg_buf[MESSAGE_BUFFER_SIZE];
        char packet_info_buf[MESSAGE_BUFFER_SIZE];
        struct in6_addr msg_addr = {0};

        struct iovec msg_buf_struct = {
            msg_buf,
            sizeof(msg_buf)
        };

        struct msghdr msg = {
            NULL,
            0,
            &msg_buf_struct,
            1,
            packet_info_buf,
            sizeof(packet_info_buf),
            0
        };

        size_t msg_len;
        cmsghdr_t *cmsg;
        size_t ip_hdr_len;
        struct icmp *reply;
        int reply_id;
        int reply_seq;
        uint16_t reply_checksum;
        uint16_t checksum;

        error = (int)recvmsg(sockfd, &msg, 0);

        /* get delay time */
        struct timespec recv_ts;
        clock_gettime(CLOCK_REALTIME, &recv_ts);
        int delay = (recv_ts.tv_nsec - start_ts.tv_nsec)/1000000;

        if (error < 0) {
            if (errno == EAGAIN) {
                if (delay > m_args->timeout){
                    log.message = "Request timed out";
                    goto exit_error;
                }
                else // No data available yet, try to receive again.
                    continue;
            }
            else{
                log.message = "recvmsg() failed";
                goto exit_error;
            }
        }
        msg_len = error;

        if (addr.ss_family == AF_INET6) {
            /*
             * The IP header is not included in the message, msg_buf points
             * directly to the ICMP data.
             */
            ip_hdr_len = 0;

            /*
             * Extract the destination address from IPv6 packet info. This
             * will be used to compute the checksum later.
             */
            for (
                cmsg = CMSG_FIRSTHDR(&msg);
                cmsg != NULL;
                cmsg = CMSG_NXTHDR(&msg, cmsg))
            {
                if (cmsg->cmsg_level == IPPROTO_IPV6
                    && cmsg->cmsg_type == IPV6_PKTINFO) {
                    struct in6_pktinfo *pktinfo = (in6_pktinfo *)CMSG_DATA(cmsg);
                    memcpy(&msg_addr,
                           &pktinfo->ipi6_addr,
                           sizeof(struct in6_addr));
                }
            }
        } else {
            /*
             * For IPv4, we must take the length of the IP header into
             * account.
             *
             * Header length is stored in the lower 4 bits of the VHL field
             * (VHL = Version + Header Length).
             */
            ip_hdr_len = ((*(uint8_t *)msg_buf) & 0x0F) * 4;
        }

        reply = (struct icmp *)(msg_buf + ip_hdr_len);
        reply_id = ntohs(reply->icmp_id);
        reply_seq = ntohs(reply->icmp_seq);

        /*
         * Verify that this is indeed an echo reply packet.
         */
        if (!(addr.ss_family == AF_INET
              && reply->icmp_type == ICMP_ECHO_REPLY)
            && !(addr.ss_family == AF_INET6
                 && reply->icmp_type == ICMP6_ECHO_REPLY)) {
            continue;
        }

        /*
         * Verify the ID and sequence number to make sure that the reply
         * is associated with the current request.
         */
        if (reply_id != id || reply_seq != seq) {
            continue;
        }

        reply_checksum = reply->icmp_cksum;
        reply->icmp_cksum = 0;

        /*
         * Verify the checksum.
         */
        if (addr.ss_family == AF_INET6) {
            size_t size = sizeof(struct ip6_pseudo_hdr) + msg_len;
            struct icmp6_packet *reply_packet = (icmp6_packet*)calloc(1, size);

            if (reply_packet == NULL){
                log.message = "reply_packet == NULL";
                goto exit_error;
            }

            memcpy(&reply_packet->ip6_hdr.src,
                   &((struct sockaddr_in6 *)&addr)->sin6_addr,
                   sizeof(struct in6_addr));
            reply_packet->ip6_hdr.dst = msg_addr;
            reply_packet->ip6_hdr.plen = htons((uint16_t)msg_len);
            reply_packet->ip6_hdr.nxt = IPPROTO_ICMPV6;
            memcpy(&reply_packet->icmp,
                   msg_buf + ip_hdr_len,
                   msg_len - ip_hdr_len);

            checksum = compute_checksum((char *)reply_packet, size);
        } else {
            checksum = compute_checksum(msg_buf + ip_hdr_len,
                                        msg_len - ip_hdr_len);
        }

        if(m_args->is_host)
            emit scanResult_host(m_target, addr_str, delay);
        if(m_args->is_ip)
            emit scanResult_ip(addr_str, delay);
        break;
    }

    if (addrinfo_list != nullptr)
        freeaddrinfo(addrinfo_list);

    if(sockfd != -1)
        close_socket(sockfd);

    return 0;

exit_error:
    if (addrinfo_list != nullptr)
        freeaddrinfo(addrinfo_list);

    if(sockfd != -1)
        close_socket(sockfd);

    return -1;
}

uint16_t ping::Scanner::compute_checksum(const char *buf, size_t size) {
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

#endif // UNIX

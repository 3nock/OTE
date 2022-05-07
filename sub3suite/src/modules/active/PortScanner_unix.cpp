#include "PortScanner.h"

#if defined(SYN_SCAN)

#if defined(Q_OS_UNIX)

struct in_addr port::Scanner::dest_ip;

int port::Scanner::start_syn_scan(char *target, QList<u_short> ports) {
    // if target is hostname. Resolve to IP
    if(m_args->is_host){
        struct addrinfo *dest = nullptr;
        struct addrinfo hints;
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_RAW;
        hints.ai_protocol = IPPROTO_TCP;
        if(getaddrinfo(target,nullptr,&hints,&dest) != 0){
            log.message = "getaddrinfo() failed!";
            return -1;
        }

        inet_ntop(AF_INET, &(((sockaddr_in*)dest->ai_addr)->sin_addr), target, dest->ai_addrlen);
    }

    //Get machine's local IP for IP header information in datagram packet
    char source_ip[INET6_ADDRSTRLEN];
    if(get_local_ip(source_ip) == -1)
        return -1;

    //This is the main socket to send the SYN packet
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0){
        log.message = "Error creating socket. Error number: "+QString::number(errno)+" Error message: "+strerror(errno);
        return -1;
    }

    //Set IP_HDRINCL socket option to tell the kernel that headers are included in the packet
    int oneVal = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &oneVal, sizeof(oneVal)) < 0){
        log.message = "Error setting IP_HDRINCL. Error number: "+QString::number(errno)+" Error message: "+strerror(errno);
        close(sockfd);
        return -1;
    }

    //Current iteration's target host address
    dest_ip.s_addr = inet_addr(target);
    if (dest_ip.s_addr == -1){
        log.message = "Invalid address";
        close(sockfd);
        return -1;
    }

    char datagram[4096];
    struct iphdr* iph = (struct iphdr*)datagram; //IP header
    struct tcphdr* tcph = (struct tcphdr*)(datagram + sizeof(struct ip)); //TCP header

    prepare_datagram(datagram, source_ip, iph, tcph);

    //Thread to listen for just one SYN-ACK packet from any of the selected ports
    pthread_t sniffer_thread;
    if (pthread_create(&sniffer_thread, NULL, receive_ack, NULL) < 0){
        log.message = "Could not create sniffer thread. Error number: "+QString::number(errno)+" Error message: "+strerror(errno);
        close(sockfd);
        return -1;
    }

    //Iterate all selected ports and send SYN packet all at once
    for(int i = 0; i < ports.length(); i++) {
        struct sockaddr_in dest;
        struct pseudo_header psh;

        dest.sin_family = AF_INET;
        dest.sin_addr.s_addr = dest_ip.s_addr;

        tcph->dest = htons(ports.at(i));
        tcph->check = 0;

        psh.source_address = inet_addr(source_ip);
        psh.dest_address = dest.sin_addr.s_addr;
        psh.placeholder = 0;
        psh.protocol = IPPROTO_TCP;
        psh.tcp_length = htons(sizeof(struct tcphdr));

        memcpy(&psh.tcp, tcph, sizeof(struct tcphdr));

        tcph->check = check_sum((unsigned short*)&psh, sizeof(struct pseudo_header));

        if(sendto(sockfd, datagram, sizeof(struct iphdr) + sizeof(struct tcphdr), 0, (struct sockaddr*)&dest, sizeof(dest)) < 0){
            log.message = "Error sending syn packet. Error number: "+QString::number(errno)+" Error message: "+strerror(errno);
            close(sockfd);
            return -1;
        }
    }

    //Will wait for the sniffer to receive a reply, host is considered closed if there aren't any
    pthread_join(sniffer_thread, NULL);
    close(sockfd);

    return 0;
}

/**
  Initialize the datagram packet
 */
void port::Scanner::prepare_datagram(char* datagram, const char* source_ip, struct iphdr* iph, struct tcphdr* tcph) {
    memset(datagram, 0, 4096);

    //Fill in the IP Header
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct ip) + sizeof(struct tcphdr);
    iph->id = htons(46156); //Id of this packet
    iph->frag_off = htons(16384);
    iph->ttl = 64;
    iph->protocol = IPPROTO_TCP;
    iph->check = 0; //Set to 0 before calculating checksum
    iph->saddr = inet_addr(source_ip); //Spoof the source ip address
    iph->daddr = dest_ip.s_addr;
    iph->check = check_sum((unsigned short*)datagram, iph->tot_len >> 1);

    //TCP Header
    tcph->source = htons(46156); //Source Port
    tcph->dest = htons(80);
    tcph->seq = htonl(1105024978);
    tcph->ack_seq = 0;
    tcph->doff = sizeof(struct tcphdr) / 4; //Size of tcp header
    tcph->fin = 0;
    tcph->syn = 1;
    tcph->rst = 0;
    tcph->psh = 0;
    tcph->ack = 0;
    tcph->urg = 0;
    tcph->window = htons(14600); //Maximum allowed window size
    tcph->check = 0; //If you set a checksum to zero, your kernel's IP stack should fill in the correct checksum during transmission
    tcph->urg_ptr = 0;
}

/**
  Method to sniff incoming packets and look for Ack replies
*/
int port::Scanner::start_sniffer() {
    int sock_raw;

    socklen_t saddr_size, data_size;
    struct sockaddr_in saddr;

    unsigned char* buffer = (unsigned char*)malloc(65536);

    //Create a raw socket that shall sniff
    sock_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock_raw < 0) {
        free(buffer);
        qWarning() << "Socket Error";
        return 1;
    }

    saddr_size = sizeof(saddr);

    //Receive a packet
    qDebug() << "receiving a packet";
    data_size = recvfrom(sock_raw, buffer, 65536, 0, (struct sockaddr*)&saddr, &saddr_size);

    if (data_size < 0) {
        qWarning() << "Recvfrom error, failed to get packets";
        return 1;
    }

    qDebug() << "processing a packet";
    process_packet(buffer, data_size, inet_ntoa(saddr.sin_addr));
    close(sock_raw);

    return 0;
}

/**
  Method to sniff incoming packets and look for Ack replies
*/
void* port::Scanner::receive_ack(void* ptr){
    start_sniffer();
    return nullptr;
}

/**
  Method to process incoming packets and look for Ack replies
*/
void port::Scanner::process_packet(unsigned char* buffer, int size, char* source_ip){
    struct iphdr* iph = (struct iphdr*)buffer; //IP Header part of this packet
    struct sockaddr_in source, dest;
    unsigned short iphdrlen;

    qDebug() << "Received!";

    if (iph->protocol == 6) {
        struct iphdr* iph = (struct iphdr*)buffer;
        iphdrlen = iph->ihl * 4;

        struct tcphdr* tcph = (struct tcphdr*)(buffer + iphdrlen);

        memset(&source, 0, sizeof(source));
        source.sin_addr.s_addr = iph->saddr;

        memset(&dest, 0, sizeof(dest));
        dest.sin_addr.s_addr = iph->daddr;

        if (tcph->syn == 1 && tcph->ack == 1 && source.sin_addr.s_addr == dest_ip.s_addr) {
            char source_host[NI_MAXHOST];
            ip_to_host(source_ip, source_host);

            qDebug() << "Port " << ntohs(tcph->source) << "open";
        }
    }
}

/**
 Checksums - IP and TCP
 */
unsigned short port::Scanner::check_sum(unsigned short* ptr, int nbytes) {
    register long sum;
    register short answer;
    unsigned short oddbyte;

    sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }

    if (nbytes == 1) {
        oddbyte = 0;
        *((u_char*)&oddbyte) = *(u_char*)ptr;
        sum += oddbyte;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum = sum + (sum >> 16);
    answer = (short)~sum;

    return answer;
}

/**
  Get ip from domain name
 */
char* port::Scanner::hostname_to_ip(char* hostname){
    struct hostent* he;
    struct in_addr** addr_list;

    if ((he = gethostbyname(hostname)) == NULL){
        log.message = "gethostbyname";
        return nullptr;
    }

    addr_list = (struct in_addr**)he->h_addr_list;

    int a;
    for (a = 0; addr_list[a] != NULL; a++)
        return inet_ntoa(*addr_list[a]); //Return the first one;

    return nullptr;
}

/**
 Get source IP of the system running this program
 */
int port::Scanner::get_local_ip(char* buffer) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    const char* kGoogleDnsIp = "8.8.8.8";
    int dns_port = 53;

    struct sockaddr_in serv;

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);
    serv.sin_port = htons(dns_port);

    if (::connect(sock, (const struct sockaddr*)&serv, sizeof(serv)) != 0){
        log.message = "Failed to get local IP\n";
        return -1;
    }

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);

    if (getsockname(sock, (struct sockaddr*)&name, &namelen) != 0){
        log.message = "Failed to get local IP";
        return -1;
    }

    inet_ntop(AF_INET, &name.sin_addr, buffer, INET6_ADDRSTRLEN);

    close(sock);
    return 0;
}

/**
 Get hostname of an IP address
 */
void port::Scanner::ip_to_host(const char* ip, char* buffer) {
    struct sockaddr_in dest;

    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(ip);
    dest.sin_port = 0;

    if (getnameinfo((struct sockaddr*)&dest, sizeof(dest), buffer, NI_MAXHOST, NULL, 0, NI_NAMEREQD) != 0)
        qWarning() << "Hostname can't be determined";
}

#endif // UNIX

#endif // SYN SCAN

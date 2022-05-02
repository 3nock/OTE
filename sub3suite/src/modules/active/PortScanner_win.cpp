#include "PortScanner.h"

#if defined(Q_OS_WIN)

int port::Scanner::start_syn_scan(QList<u_short> target_ports) {
    active_packets = 0;
    packet_id = 1234;
    source_port = 54321;
    QSet<u_short> open_ports;

    struct sockaddr_in sa;
    if(inet_pton(AF_INET, dest_ip_address, &(sa.sin_addr)) != 1){
        log.message = "inet_pton: dest_ip failed";
        return -1;
    }
    dest_ip = sa.sin_addr.S_un.S_addr;
    dest_ipv4[0] = sa.sin_addr.S_un.S_un_b.s_b1;
    dest_ipv4[1] = sa.sin_addr.S_un.S_un_b.s_b2;
    dest_ipv4[2] = sa.sin_addr.S_un.S_un_b.s_b3;
    dest_ipv4[3] = sa.sin_addr.S_un.S_un_b.s_b4;

    // Get the destination MAC address
    if (!get_destination_adaptor_details()){
        log.message = "get_destination_adaptor_details() failed";
        return -1;
    }

    // Build the packets and send
    u_char packet[PACKET_SIZE];
    set_ethernet_fields(packet);
    set_internet_protocol_fields(packet);
    for (int j = 0; j < m_args->target_ports.size(); j++) {
        set_tcp_fields(packet, target_ports.at(j));
        if (pcap_sendpacket(fp, packet, PACKET_SIZE) != 0) {
            log.message = "Error sending the packet: " + QString::fromLocal8Bit(pcap_geterr(fp));
            return -1;
        }
        active_packets++;
    }

    SYSTEMTIME start_time;
    GetSystemTime(&start_time);

    /* Retrieve the packets */
    struct pcap_pkthdr *header;
    const u_char *pkt_data;
    int res;
    while((res = pcap_next_ex( fp, &header, &pkt_data)) >= 0){
        if(res == 0)
            continue;

        if (header->len > 50) {
            if (
                (pkt_data[12] == 0x08) && (pkt_data[13] == 0x00) && //  12-13 0x0800 (IP)
                (pkt_data[14] == 0x45) &&                           //  14    0x45 (IPv4)
                (pkt_data[23] == 0x06))                             //  23    0x06 (TCP)
            {
                if((pkt_data[26] == dest_ipv4[0]) &&
                   (pkt_data[27] == dest_ipv4[1]) &&
                   (pkt_data[28] == dest_ipv4[2]) &&
                   (pkt_data[29] == dest_ipv4[3]) &&
                   (pkt_data[30] == src_ipv4[0]) &&
                   (pkt_data[31] == src_ipv4[1]) &&
                   (pkt_data[32] == src_ipv4[2]) &&
                   (pkt_data[33] == src_ipv4[3]))
                {
                    // get port
                    u_short port = ((u_short)pkt_data[34] << 8) + pkt_data[35];
                    if(open_ports.contains(port))
                        continue;
                    open_ports.insert(port);

                    // get flags
                    u_short flags = ((u_short)pkt_data[46] << 8) + pkt_data[47];

                    // check flags
                    if (((flags & TCP_FLAGS_ACK) == TCP_FLAGS_ACK) && ((flags & TCP_FLAGS_SYN) == TCP_FLAGS_SYN)){
                        if(m_args->is_host)
                            emit scanResult_host(m_target, dest_ip_address, port);
                        if(m_args->is_ip)
                            emit scanResult_ip(m_target, port);
                    }
                    active_packets--;
                    if(active_packets == 0)
                        break;
                }
            }
        }

        // timeout elapsed
        SYSTEMTIME current_time;
        GetSystemTime(&current_time);
        u_int64 time_difference = calc_time_difference(start_time, current_time);
        if ((time_difference / 10000) > m_args->timeout)
            break;
    }
    return 0;
}

int port::Scanner::init_syn_scan(){
    DWORD size = 0;
    PIP_ADAPTER_ADDRESSES adapter_addresses, adapter_address;
    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS, nullptr, nullptr, &size) != ERROR_BUFFER_OVERFLOW) {
        log.message = "Unable to get network adaptors(GetAdaptersAddresses())";
        return -1;
    }
    adapter_addresses = static_cast<PIP_ADAPTER_ADDRESSES>(malloc(size));

    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS, nullptr, adapter_addresses, &size) != ERROR_SUCCESS) {
        log.message = "Unable to get network adaptors(GetAdaptersAddresses())";
        free(adapter_addresses);
        return -1;
    }

    char *device = nullptr;
    for (adapter_address = adapter_addresses; adapter_address != nullptr; adapter_address = adapter_address->Next) {
        if(adapter_address->FirstGatewayAddress){
            device = adapter_address->AdapterName;
            break;
        }
    }

    if(device == nullptr){
        log.message = "No active adapter found!";
        return -1;
    }

    if (!load_npcap_dlls()) {
        log.message = "Couldn't load Npcap";
        return -1;
    }

    // initializing data;
    source_ip_address = new char[16];
    default_gateway = new char[16];

    // Get the source IP address & MAC address & default Gateway
    if (!get_source_adaptor_details(device)){
        log.message = "get_source_adaptor_details failed";
        return -1;
    }

    // Get the networking device full-name
    if (!get_source_adaptor_full_name(device)) {
        log.message = "get_source_adaptor_full_name failed";
        return -1;
    }

    // Open the network adapter
    char errorBuffer[PCAP_ERRBUF_SIZE];
    if ((fp = pcap_open_live(device_name, 65536, 1, 1000, errorBuffer)) == nullptr) {
        log.message = "Unable to open the adapter " + QString::fromLocal8Bit(device);
        return -1;
    }

    if(inet_pton(AF_INET, source_ip_address, &src_ip) != 1){
        log.message = "inet_pton: src_ip failed";
        return -1;
    }
    if(inet_pton(AF_INET, default_gateway, &gate_ip) != 1){
        log.message = "inet_pton: gate_ip failed";
        return -1;
    }
    return 0;
}

bool port::Scanner::load_npcap_dlls() {
    _TCHAR npcap_dir[512];
    UINT len;
    len = GetSystemDirectory(npcap_dir, 480);
    if (!len) {
        log.message = "Error in GetSystemDirectory: " + QString::number(GetLastError());
        return false;
    }
    _tcscat_s(npcap_dir, 512, _T("\\Npcap"));
    if (SetDllDirectory(npcap_dir) == 0) {
        log.message = "Error in SetDllDirectory: " + QString::number(GetLastError());
        return false;
    }
    return true;
}

bool port::Scanner::get_source_adaptor_details(char device[]) {
    bool found_adaptor = false;

    DWORD size = 0;
    PIP_ADAPTER_ADDRESSES adapter_addresses, adapter_address;
    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS, nullptr, nullptr, &size) != ERROR_BUFFER_OVERFLOW) {
        log.message = "Unable to get network adaptors(GetAdaptersAddresses())";
        return false;
    }
    adapter_addresses = static_cast<PIP_ADAPTER_ADDRESSES>(malloc(size));

    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS, nullptr, adapter_addresses, &size) != ERROR_SUCCESS) {
        log.message = "Unable to get network adaptors(GetAdaptersAddresses())";
        free(adapter_addresses);
        return false;
    }

    for (adapter_address = adapter_addresses; adapter_address != nullptr; adapter_address = adapter_address->Next) {
        if (!strcmp(adapter_address->AdapterName, device))
        {
            found_adaptor = true;
            memcpy(source_mac_address, adapter_address->PhysicalAddress, adapter_address->PhysicalAddressLength);

            PIP_ADAPTER_UNICAST_ADDRESS unicast_address = adapter_address->FirstUnicastAddress;
            while(unicast_address){
                if (unicast_address->Address.lpSockaddr->sa_family == AF_INET){
                    SOCKADDR_IN* unicast_address_sockaddr = reinterpret_cast<SOCKADDR_IN*>(unicast_address->Address.lpSockaddr);
                    inet_ntop(AF_INET, &(unicast_address_sockaddr->sin_addr), source_ip_address, 16);
                    src_ipv4[0] = unicast_address_sockaddr->sin_addr.S_un.S_un_b.s_b1;
                    src_ipv4[1] = unicast_address_sockaddr->sin_addr.S_un.S_un_b.s_b2;
                    src_ipv4[2] = unicast_address_sockaddr->sin_addr.S_un.S_un_b.s_b3;
                    src_ipv4[3] = unicast_address_sockaddr->sin_addr.S_un.S_un_b.s_b4;
                    break;
                }
                unicast_address = unicast_address->Next;
            }

            PIP_ADAPTER_GATEWAY_ADDRESS_LH gateway_address = adapter_address->FirstGatewayAddress;
            while(gateway_address){
                if(gateway_address->Address.lpSockaddr->sa_family == AF_INET){
                    SOCKADDR_IN* gateway_address_sockaddr = reinterpret_cast<SOCKADDR_IN*>(gateway_address->Address.lpSockaddr);
                    inet_ntop(AF_INET, &(gateway_address_sockaddr->sin_addr), default_gateway, 16);
                    break;
                }
                gateway_address = gateway_address->Next;
            }
        }
    }

    if (!found_adaptor) {
        log.message = "Unable to find adaptor";
        return false;
    }
    return true;
}

bool port::Scanner::get_source_adaptor_full_name(char device[]) {
    pcap_if_t* all_devices;
    pcap_if_t* device_found;
    char error_buffer[PCAP_ERRBUF_SIZE];

    /* Retrieve the device list */
    if (pcap_findalldevs(&all_devices, error_buffer) == -1) {
        log.message = "Error in pcap_findalldevs: " + QString::fromLocal8Bit(error_buffer);
        return false;
    }

    /* Print the list */
    for (device_found = all_devices; device_found; device_found = device_found->next)
    {
        if (strstr(device_found->name, device) != nullptr) {
            strcpy_s(device_name, 200, device_found->name);
            pcap_freealldevs(all_devices);
            return true;
        }
    }

    pcap_freealldevs(all_devices);
    return false;
}

bool port::Scanner::get_destination_adaptor_details() {
    ULONG physical_address_length = MAC_ADAPTER_LENGTH;
    u_char temp_dest_mac_address[MAC_ADAPTER_LENGTH];

    if (SendARP(dest_ip, src_ip, &temp_dest_mac_address, &physical_address_length)) {
        log.message = "Unable to get destination ethernet address";
        return false;
    }

    memcpy(dest_mac_address, temp_dest_mac_address, MAC_ADAPTER_LENGTH);
    return true;
}

void port::Scanner::copy_u_short_to_array(u_char* array, int index, u_short val) {
    u_short val2 = htons(val);
    memcpy((array + index), (unsigned char*)&val2, sizeof(u_short));
}

void port::Scanner::copy_u_long_to_array(u_char* array, int index, u_long val) {
    u_long val2 = htonl(val);
    memcpy((array + index), (unsigned char*)&val2, sizeof(u_long));
}

u_short port::Scanner::calculate_ip_checksum(u_char packet[PACKET_SIZE]) {
    u_short checksum = 0;
    u_long total = 0x00000000;

    // For the 10 words, starting at offset 14, sum the words
    for (int i = 0; i < 10; i++) {
        int index = 14 + (i * 2);
        u_short word;
        memcpy(&word, packet + index, 2);
        total += ntohs(word);
    }

    // Whilst the sum is too large to fit in a word, grab the carries
    // and add to the total
    while (total > 0xFFFF) {
        u_short carry = total >> 16;
        total = total & 0xFFFF;
        total += carry;
    }

    checksum = static_cast<u_short>(total);
    // Finally, calculate onescomplement (not) and return in correct endian-ness
    return (~checksum);
}

u_short port::Scanner::calculate_tcp_checksum(u_char packet[PACKET_SIZE]) {
    u_char pseudo_header[PSEUDO_HEADER_SIZE];
    memcpy(pseudo_header + 0, packet + 26, 4);
    memcpy(pseudo_header + 4, packet + 30, 4);
    pseudo_header[8] = 0x00;
    pseudo_header[9] = packet[23]; //( IP_TCP )
    pseudo_header[10] = 0x00;
    pseudo_header[11] = 0x18;

    u_short checksum = 0;
    u_long total = 0x00000000;

    for (int i = 0; i < 6; i++) {
        int index = i * 2;
        u_short word;
        memcpy(&word, pseudo_header + index, 2);
        total += ntohs(word);
    }

    // For the 12 words, starting at offset 34, sum the words
    for (int i = 0; i < 12; i++) {
        int index = 34 + (i * 2);
        u_short word;
        memcpy(&word, packet + index, 2);
        total += ntohs(word);
    }

    // Whilst the sum is too large to fit in a word, grab the carries
    // and add to the total
    while (total > 0xFFFF) {
        u_short carry = total >> 16;
        total = total & 0xFFFF;
        total += carry;
    }

    checksum = static_cast<u_short>(total);
    // Finally, calculate onescomplement (not) and return in correct endian-ness
    return (~checksum);
}

void port::Scanner::set_ethernet_fields(u_char packet[PACKET_SIZE]) {
    memcpy(packet + 0, dest_mac_address, MAC_ADAPTER_LENGTH); // Copy destination MAC address
    memcpy(packet + 6, source_mac_address, MAC_ADAPTER_LENGTH); // Copy source MAC address
    copy_u_short_to_array(packet, 12, static_cast<u_short>(ETHERNET_IPV4)); // EtherType 0x0800 for IPv4 https://en.wikipedia.org/wiki/EtherType#Examples
}

void port::Scanner::set_internet_protocol_fields(u_char packet[PACKET_SIZE]) {
    packet[14] = IP_VIHL; // Version and IHL
    packet[15] = IP_DSCP_ECN; // DSCP and ECN
    copy_u_short_to_array(packet, 16, IP_LENGTH); // Total Length
    copy_u_short_to_array(packet, 18, packet_id); // ID
    packet_id++;
    copy_u_short_to_array(packet, 20, IP_FLAGS_FOFFSET); // Flags and Fragment Offset
    packet[22] = IP_TIME_TO_LIVE; // Time to Live
    packet[23] = IP_TCP; // Protocol
    copy_u_short_to_array(packet, 24, IP_INITIAL_CHECKSUM); // Header Checksum. Initially set to zero, we will calculate it properly later.
    memcpy(packet+26, src_ipv4, 4); // Source IP
    memcpy(packet+30, dest_ipv4, 4); // Destination IP
    u_short ip_checksum = calculate_ip_checksum(packet); // Header Checksum
    copy_u_short_to_array(packet, 24, ip_checksum);
}

void port::Scanner::set_tcp_fields(u_char packet[PACKET_SIZE], u_short dest_port) {
    copy_u_short_to_array(packet, 34, source_port); // SrcPort
    source_port++;
    copy_u_short_to_array(packet, 36, dest_port); //packet[36] = destPort;
    copy_u_long_to_array(packet, 38, htonl(5555l)); // Sequence No.
    copy_u_long_to_array(packet, 42, TCP_ACK_NO); // Ack No.
    packet[46] = TCP_OFFSET; // Offset
    packet[47] = TCP_FLAGS_SYN; // Flags
    copy_u_short_to_array(packet, 48, TCP_WINDOW_SIZE); // Window Size
    copy_u_short_to_array(packet, 50, TCP_INITIAL_CHECKSUM); // Set the checksum to zero for now. We calculate the actual value later.
    copy_u_short_to_array(packet, 52, TCP_URGENT_PTR); // Urgent Pointer
    packet[54] = TCP_MAX_SEG_SIZE; // Max Segment Size
    packet[55] = TCP_LENGTH; // Length
    copy_u_short_to_array(packet, 56, TCP_MSS_VALUE); // MSS Value
    u_short tcp_checksum = calculate_tcp_checksum(packet); // Calculate the actual checksum
    copy_u_short_to_array(packet, 50, tcp_checksum);
}

u_int64 port::Scanner::calc_time_difference(SYSTEMTIME st1, SYSTEMTIME st2) {
    union time_union ft1;
    union time_union ft2;

    SystemTimeToFileTime(&st1, &ft1.fileTime);
    SystemTimeToFileTime(&st2, &ft2.fileTime);

    return ft2.ul.QuadPart - ft1.ul.QuadPart;
}


bool port::Scanner::get_address(SOCKADDR *sa, size_t salen, char* addr) {
    switch(sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                    addr, salen);
            return true;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                    addr, salen);
            return true;

        default:
            log.message = "unknown sa_family type";
            return false;
    }
}
#endif // WINDOWS

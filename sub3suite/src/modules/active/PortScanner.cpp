#include "PortScanner.h"


pcap_t* port::Scanner::fp = nullptr;
int port::Scanner::total_ports = NULL;
u_char port::Scanner::source_ip_address[IP_ADDRESS_LENGTH];
u_char port::Scanner::dest_ip_address[IP_ADDRESS_LENGTH];
SYSTEMTIME port::Scanner::start_time;
BOOL port::Scanner::packets_received[MAX_PORTS];
s3s_struct::HOST port::Scanner::host;


QString port::getTarget(port::ScanArgs *args){
    QMutexLocker(&args->mutex);

    if(!args->target_ips.isEmpty())
        return args->target_ips.dequeue();
    else
        return nullptr;
}


port::Scanner::Scanner(port::ScanArgs *args): AbstractScanner(nullptr),
    m_args(args)
{
}
port::Scanner::~Scanner(){
}

void port::Scanner::lookup(){
    switch (m_args->scan_type)
    {
    case port::ScanType::SYN:
        this->scanner_syn();
        break;

    case port::ScanType::STEALTH:
        this->scanner_stealth();
        break;

    case port::ScanType::CONNECTION:
        this->scanner_connection();
        break;
    }

    emit quitThread();
}

void port::Scanner::scanner_connection(){
    QTcpSocket *socket = new QTcpSocket;
    m_target = port::getTarget(m_args);

    while(m_target != nullptr)
    {
        QList<u_short> ports;

        foreach(const quint16 &port, m_args->target_ports){
            socket->connectToHost(m_target, port);
            if(socket->waitForConnected(m_args->timeout)){
                socket->close();
                ports.append(port);
            }
        }

        m_args->progress++;
        emit scanProgress(m_args->progress);

        if(m_args->is_host)
            emit scanResult_host(m_target, QString(), ports);
        if(m_args->is_ip)
            emit scanResult_ip(m_target, ports);

        m_target = getTarget(m_args);
    }

    socket->deleteLater();
}

void port::Scanner::scanner_stealth(){

}

void port::Scanner::scanner_syn(){
    m_target = getTarget(m_args);

    DWORD size = 0;
    PIP_ADAPTER_ADDRESSES adapter_addresses, adapter_address;
    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS, nullptr, nullptr, &size) != ERROR_BUFFER_OVERFLOW) {
        qWarning() << "Unable to get network adaptors(GetAdaptersAddresses())";
        return;
    }
    adapter_addresses = static_cast<PIP_ADAPTER_ADDRESSES>(malloc(size));

    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS, nullptr, adapter_addresses, &size) != ERROR_SUCCESS) {
        qWarning() << "Unable to get network adaptors(GetAdaptersAddresses())";
        free(adapter_addresses);
        return;
    }

    char *device = nullptr;
    for (adapter_address = adapter_addresses; adapter_address != nullptr; adapter_address = adapter_address->Next) {
        if(adapter_address->FirstGatewayAddress){
            device = adapter_address->AdapterName;
            qWarning() << "adapter name: " << adapter_address->AdapterName << " description: " << *adapter_address->Description;
        }
    }

    if(device == nullptr){
        qWarning() << "No active adapter found!";
        return;
    }

    if (!load_npcap_dlls()) {
        qWarning() << "Couldn't load Npcap";
        return;
    }

    while(m_target != nullptr) {
        host.host = m_target;

        QByteArray ba = m_target.toLocal8Bit();
        char *target = ba.data();

        start_syn_scan(device, target, m_args->target_ports.values());

        if(m_args->is_host)
            emit scanResult_host(QString(), QString(), QList<u_short>());
        if(m_args->is_ip)
            emit scanResult_ip(QString(), QList<u_short>());

        /* next target */
        m_target = getTarget(m_args);
    }
}

void port::Scanner::start_syn_scan(char *device, char *target_ip, QList<u_short> target_ports) {
    char errorBuffer[PCAP_ERRBUF_SIZE];
    u_char packet[PACKET_SIZE];

    total_ports = target_ports.length();
    for (int i = 0; i < MAX_PORTS; i++)
        packets_received[i] = TRUE;
    for (int i = 0; i < total_ports; i++)
        packets_received[target_ports[i]] = FALSE;

    // Get the destination IP address in bytes (u_chars)
    struct sockaddr_in sa;
    inet_pton(AF_INET, target_ip, &(sa.sin_addr));
    dest_ip_address[0] = static_cast<u_char>(sa.sin_addr.S_un.S_un_b.s_b1);
    dest_ip_address[1] = static_cast<u_char>(sa.sin_addr.S_un.S_un_b.s_b2);
    dest_ip_address[2] = static_cast<u_char>(sa.sin_addr.S_un.S_un_b.s_b3);
    dest_ip_address[3] = static_cast<u_char>(sa.sin_addr.S_un.S_un_b.s_b4);

    u_char default_gateway[IP_ADDRESS_LENGTH];
    // Get the source MAC address
    u_char source_mac_address[MAC_ADAPTER_LENGTH];
    if (!get_source_adaptor_details(device, source_mac_address, source_ip_address, default_gateway))
        return;

    // Save the source IP address
    char source_ip_address_string[IP_ADDRESS_STRING_LENGTH];
    sprintf_s(source_ip_address_string, IP_ADDRESS_STRING_LENGTH, "%d.%d.%d.%d", source_ip_address[0], source_ip_address[1], source_ip_address[2], source_ip_address[3]);
    char default_gateway_ip_address_string[IP_ADDRESS_STRING_LENGTH];
    sprintf_s(default_gateway_ip_address_string, IP_ADDRESS_STRING_LENGTH, "%d.%d.%d.%d", default_gateway[0], default_gateway[1], default_gateway[2], default_gateway[3]);

    // Get the destination MAC address
    u_char dest_mac_address[MAC_ADAPTER_LENGTH];
    if (!get_destination_adaptor_details(source_ip_address_string, default_gateway_ip_address_string, dest_mac_address))
        return;

    // Get the networking device full-name
    char device_name[200];
    if (!get_source_adaptor_full_name(device, device_name)) {
        return;
    }

    // Open the network adapter
    if ((fp = pcap_open_live(device_name, 65536, 1, 1000, errorBuffer)) == nullptr) {
        qWarning() << "Unable to open the adapter " << device;
        return;
    }

    // Build the packets and send
    for (int j = 0; j < m_args->target_ports.size(); j++) {
        set_ethernet_fields(packet, source_mac_address, dest_mac_address);
        set_internet_protocol_fields(packet);
        set_tcp_fields(packet, target_ports.at(j));

        if (pcap_sendpacket(fp, packet, PACKET_SIZE) != 0) {
            qWarning() << "Error sending the packet: " << pcap_geterr(fp);
            return;
        }
    }

    GetSystemTime(&start_time);

    // Start listening for packets
    pcap_loop(fp, 0, packet_handler, nullptr);

    // Close
    pcap_close(fp);

    qDebug() << "Scan Complete For Target: " << host.host;
    return;
}

bool port::Scanner::load_npcap_dlls() {
    _TCHAR npcap_dir[512];
    UINT len;
    len = GetSystemDirectory(npcap_dir, 480);
    if (!len) {
        qWarning() << "Error in GetSystemDirectory: " << GetLastError();
        return false;
    }
    _tcscat_s(npcap_dir, 512, _T("\\Npcap"));
    if (SetDllDirectory(npcap_dir) == 0) {
        qWarning() << "Error in SetDllDirectory: " << GetLastError();
        return false;
    }
    return true;
}

bool port::Scanner::get_source_adaptor_details(char device[], u_char mac_address[], u_char ip_address[], u_char default_gateway[]) {
    bool found_adaptor = false;

    DWORD size = 0;
    PIP_ADAPTER_ADDRESSES adapter_addresses, adapter_address;
    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS, nullptr, nullptr, &size) != ERROR_BUFFER_OVERFLOW) {
        qWarning() << "Unable to get network adaptors(GetAdaptersAddresses())";
        return false;
    }
    adapter_addresses = static_cast<PIP_ADAPTER_ADDRESSES>(malloc(size));

    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS, nullptr, adapter_addresses, &size) != ERROR_SUCCESS) {
        qWarning() << "Unable to get network adaptors(GetAdaptersAddresses())";
        free(adapter_addresses);
        return false;
    }

    for (adapter_address = adapter_addresses; adapter_address != nullptr; adapter_address = adapter_address->Next) {
        if (!strcmp(adapter_address->AdapterName, device))
        {
            found_adaptor = true;
            memcpy(mac_address, adapter_address->PhysicalAddress, adapter_address->PhysicalAddressLength);

            PIP_ADAPTER_UNICAST_ADDRESS unicast_address = adapter_address->FirstUnicastAddress;
            while(unicast_address){
                if (unicast_address->Address.lpSockaddr->sa_family == AF_INET){
                    SOCKADDR_IN* unicast_address_sockaddr = reinterpret_cast<SOCKADDR_IN*>(unicast_address->Address.lpSockaddr);
                    ip_address[0] = unicast_address_sockaddr->sin_addr.S_un.S_un_b.s_b1;
                    ip_address[1] = unicast_address_sockaddr->sin_addr.S_un.S_un_b.s_b2;
                    ip_address[2] = unicast_address_sockaddr->sin_addr.S_un.S_un_b.s_b3;
                    ip_address[3] = unicast_address_sockaddr->sin_addr.S_un.S_un_b.s_b4;
                    break;
                }
                unicast_address = unicast_address->Next;
            }

            PIP_ADAPTER_GATEWAY_ADDRESS_LH gateway_address = adapter_address->FirstGatewayAddress;
            while(gateway_address){
                if(gateway_address->Address.lpSockaddr->sa_family == AF_INET){
                    SOCKADDR_IN* gateway_address_sockaddr = reinterpret_cast<SOCKADDR_IN*>(gateway_address->Address.lpSockaddr);
                    default_gateway[0] = gateway_address_sockaddr->sin_addr.S_un.S_un_b.s_b1;
                    default_gateway[1] = gateway_address_sockaddr->sin_addr.S_un.S_un_b.s_b2;
                    default_gateway[2] = gateway_address_sockaddr->sin_addr.S_un.S_un_b.s_b3;
                    default_gateway[3] = gateway_address_sockaddr->sin_addr.S_un.S_un_b.s_b4;
                    break;
                }
                gateway_address = gateway_address->Next;
            }
        }
    }

    if (!found_adaptor) {
        qWarning() << "Unable to find adaptor";
        return false;
    }
    return true;
}

bool port::Scanner::get_source_adaptor_full_name(char device[], char device_full_name[]) {
    pcap_if_t* all_devices;
    pcap_if_t* device_found;
    char error_buffer[PCAP_ERRBUF_SIZE];

    /* Retrieve the device list */
    if (pcap_findalldevs(&all_devices, error_buffer) == -1) {
        qWarning() << "Error in pcap_findalldevs: " << error_buffer;
        return false;
    }

    /* Print the list */
    for (device_found = all_devices; device_found; device_found = device_found->next)
    {
        if (strstr(device_found->name, device) != nullptr) {
            strcpy_s(device_full_name, 200, device_found->name);
            pcap_freealldevs(all_devices);
            return true;
        }
    }

    pcap_freealldevs(all_devices);
    return false;
}

bool port::Scanner::get_destination_adaptor_details(char* source_ip_string, char* default_gateway_ip_string, u_char dest_mac_address[MAC_ADAPTER_LENGTH]) {
    ULONG physical_address_length = MAC_ADAPTER_LENGTH;
    u_char temp_dest_mac_address[MAC_ADAPTER_LENGTH];

    IPAddr source_ip = 0;
    IPAddr dest_ip = 0;

    if (inet_pton(AF_INET, default_gateway_ip_string, &dest_ip) != 1)
        return false;
    if (inet_pton(AF_INET, source_ip_string, &source_ip) != 1)
        return false;

    if (SendARP((IPAddr)dest_ip, (IPAddr)source_ip, &temp_dest_mac_address, &physical_address_length)) {
        qWarning() << "Unable to get destination ethernet address";
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

void port::Scanner::set_ethernet_fields(u_char packet[PACKET_SIZE], u_char source_mac_address[MAC_ADAPTER_LENGTH], u_char dest_mac_address[MAC_ADAPTER_LENGTH]) {
    // Copy destination MAC address
    memcpy(packet + 0, dest_mac_address, MAC_ADAPTER_LENGTH);

    // Copy source MAC address
    memcpy(packet + 6, source_mac_address, MAC_ADAPTER_LENGTH);

    // EtherType 0x0800 for IPv4 https://en.wikipedia.org/wiki/EtherType#Examples
    copy_u_short_to_array(packet, 12, static_cast<u_short>(ETHERNET_IPV4));
}

void port::Scanner::set_internet_protocol_fields(u_char packet[PACKET_SIZE]) {
    // Version and IHL
    packet[14] = IP_VIHL;

    // DSCP and ECN
    packet[15] = IP_DSCP_ECN;

    // Total Length
    copy_u_short_to_array(packet, 16, IP_LENGTH);

    // ID
    copy_u_short_to_array(packet, 18, packet_id);
    packet_id++;

    // Flags and Fragment Offset
    copy_u_short_to_array(packet, 20, IP_FLAGS_FOFFSET);

    // Time to Live
    packet[22] = IP_TIME_TO_LIVE;

    // Protocol
    packet[23] = IP_TCP;

    // Header Checksum. Initially set to zero, we will calculate it properly later.
    copy_u_short_to_array(packet, 24, IP_INITIAL_CHECKSUM);

    // Source IP
    packet[26] = source_ip_address[0];
    packet[27] = source_ip_address[1];
    packet[28] = source_ip_address[2];
    packet[29] = source_ip_address[3];

    // Destination IP
    packet[30] = dest_ip_address[0];
    packet[31] = dest_ip_address[1];
    packet[32] = dest_ip_address[2];
    packet[33] = dest_ip_address[3];

    // Header Checksum
    u_short ip_checksum = calculate_ip_checksum(packet);
    copy_u_short_to_array(packet, 24, ip_checksum);
}

void port::Scanner::set_tcp_fields(u_char packet[PACKET_SIZE], u_short dest_port) {
    //packet[34] = SrcPort; 0x1394 ->tcp, 0x1393 ->80 /*0xD55E*/
    copy_u_short_to_array(packet, 34, 0x1393);

    //packet[36] = destPort;
    copy_u_short_to_array(packet, 36, dest_port);

    // Sequence No.
    copy_u_long_to_array(packet, 38, TCP_SEQ_NO);

    // Ack No.
    copy_u_long_to_array(packet, 42, TCP_ACK_NO);

    // Offset
    packet[46] = TCP_OFFSET;

    // Flags
    packet[47] = TCP_FLAGS_SYN;

    // Window Size
    copy_u_short_to_array(packet, 48, TCP_WINDOW_SIZE);

    // Set the checksum to zero for now. We calculate the actual value later.
    copy_u_short_to_array(packet, 50, TCP_INITIAL_CHECKSUM);

    // Urgent Pointer
    copy_u_short_to_array(packet, 52, TCP_URGENT_PTR);

    // Max Segment Size
    packet[54] = TCP_MAX_SEG_SIZE;

    // Length
    packet[55] = TCP_LENGTH;

    // MSS Value
    copy_u_short_to_array(packet, 56, TCP_MSS_VALUE);

    // Calculate the actual checksum
    u_short tcp_checksum = calculate_tcp_checksum(packet);
    copy_u_short_to_array(packet, 50, tcp_checksum);
}

void port::Scanner::packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data) {
    if (header->len > 50) {
        if (
            (pkt_data[12] == 0x08) && (pkt_data[13] == 0x00) && //  12-13 0x0800 (IP)
            (pkt_data[14] == 0x45) &&                           //  14    0x45 (IPv4)
            (pkt_data[23] == 0x06))                             //  23    0x06 (TCP)
        {
            if (
                (pkt_data[26] == dest_ip_address[0]) &&
                (pkt_data[27] == dest_ip_address[1]) &&
                (pkt_data[28] == dest_ip_address[2]) &&
                (pkt_data[29] == dest_ip_address[3]) &&
                (pkt_data[30] == source_ip_address[0]) &&
                (pkt_data[31] == source_ip_address[1]) &&
                (pkt_data[32] == source_ip_address[2]) &&
                (pkt_data[33] == source_ip_address[3]))
            {
                u_short port = ((u_short)pkt_data[34] << 8) + pkt_data[35];
                u_short flags = ((u_short)pkt_data[46] << 8) + pkt_data[47];

                if (!packets_received[port])
                {
                    if (((flags & TCP_FLAGS_ACK) == TCP_FLAGS_ACK) && ((flags & TCP_FLAGS_SYN) == TCP_FLAGS_SYN)){
                        host.ports.append(port);
                        qDebug() << "Target: " << host.host << " Port: " << port;
                    }
                    else if (((flags & TCP_FLAGS_ACK) == TCP_FLAGS_ACK) && ((flags & TCP_FLAGS_RST) == TCP_FLAGS_RST))
                        qDebug() << "Closed: " << port;
                    else
                        qDebug() << "Unknown: " << port;

                    packets_received[port] = TRUE;
                }

                total_ports--;
                if (total_ports == 0) {
                    pcap_breakloop(fp);
                }
            }
        }
    }

    SYSTEMTIME current_time;
    GetSystemTime(&current_time);

    u_int64 time_difference = calc_time_difference(start_time, current_time);

    if ((time_difference / 10000000) > 30)
        pcap_breakloop(fp);
}

u_int64 port::Scanner::calc_time_difference(SYSTEMTIME st1, SYSTEMTIME st2) {
    union time_union ft1;
    union time_union ft2;

    SystemTimeToFileTime(&st1, &ft1.fileTime);
    SystemTimeToFileTime(&st2, &ft2.fileTime);

    return ft2.ul.QuadPart - ft1.ul.QuadPart;
}

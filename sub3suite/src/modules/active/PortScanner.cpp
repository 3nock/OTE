#include "PortScanner.h"


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
        foreach(const quint16 &port, m_args->target_ports){
            socket->connectToHost(m_target, port);
            if(socket->waitForConnected(m_args->timeout)){
                if(m_args->is_host)
                    emit scanResult_host(m_target, socket->peerAddress().toString(), port);
                if(m_args->is_ip)
                    emit scanResult_ip(m_target, port);
                socket->close();
            }
        }

        m_args->progress++;
        emit scanProgress(m_args->progress);

        m_target = getTarget(m_args);
    }

    socket->deleteLater();
}

void port::Scanner::scanner_syn(){
#if defined(Q_OS_WIN)
    if(init_syn_scan() == -1)
        return;

    m_target = port::getTarget(m_args);
    while(m_target != nullptr) {
        m_mutex.lock();

        /* if received pause signal lock the thread, dont unlock until resume signal*/
        if(pause)
            m_wait.wait(&m_mutex);

        /* check if received stop signal */
        if(stop)
            break;

        m_mutex.unlock();

        log.target = m_target;
        QByteArray ba = m_target.toLocal8Bit();
        dest_ip_address = ba.data();

        // Resolve the destination address
        if(m_args->is_host)
        {
            // get destination addrinfo
            struct addrinfo *dest = nullptr;
            struct addrinfo hints;
            memset(&hints, 0, sizeof(hints));
            hints.ai_flags = 0;
            hints.ai_family = AF_INET;
            hints.ai_socktype = 0;
            hints.ai_protocol = 0;
            if(getaddrinfo(dest_ip_address, nullptr, &hints, &dest) != 0) {
                log.message = "Invalid address: " + QString::fromLocal8Bit(dest_ip_address) + "getaddrinfo() failed";
                emit scanLog(log);
                continue;
            }

            // get destination char*
            if(!get_address(dest->ai_addr, dest->ai_addrlen, dest_ip_address)){
                emit scanLog(log);
                continue;
            }
        }

        // start scan
        if(start_syn_scan(m_args->target_ports.values()) == -1)
            emit scanLog(log);

        // next target
        m_target = getTarget(m_args);
    }

    if(fp)
        pcap_close(fp);
    if(source_ip_address)
        delete source_ip_address;
    if(default_gateway)
        delete default_gateway;

#endif // WINDOWS

#if defined(Q_OS_UNIX)
    m_target = port::getTarget(m_args);
    while(m_target != nullptr) {
        QByteArray ba = m_target.toLocal8Bit();
        char *target = ba.data();

        // Resolve the destination address
        if(m_args->is_host)
            target = hostname_to_ip(target);

        // start scan
        start_syn_unix(target, m_args->target_ports.values());

        // next target
        m_target = getTarget(m_args);
    }
#endif // UNIX
}

QString port::getTarget(port::ScanArgs *args){
    QMutexLocker(&args->mutex);

    if(!args->target_ips.isEmpty())
        return args->target_ips.dequeue();
    else
        return nullptr;
}

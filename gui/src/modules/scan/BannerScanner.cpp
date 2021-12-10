#include "BannerScanner.h"


banner::Scanner::Scanner(ScanConfig *scanConfig, banner::ScanArguments *scanArguments)
    : AbstractScanner (nullptr),
      m_config(scanConfig),
      m_args(scanArguments),
      m_socket(new QTcpSocket(this))
{
}
banner::Scanner::~Scanner(){
    delete m_socket;
}

void banner::Scanner::lookup(){
    ///
    /// getting service type...
    ///
    quint16 service = NULL;
    if(m_args->http)
        service = 80;
    if(m_args->https)
        service = 443;
    if(m_args->ftp)
        service = 21;
    if(m_args->ssh)
        service = 22;
    if(m_args->smtp)
        service = 25;

    ///
    /// getting target type and establishing connection...
    ///
    if(m_args->hostname)
        m_socket->connectToHost(m_args->target, service);
    if(m_args->ipaddress)
        m_socket->connectToHost(QHostAddress(m_args->target), service);

    ///
    /// reading response...
    ///
    if(m_socket->waitForConnected(m_config->timeout))
    {
        m_socket->write("HEAD / HTTP/1.1");
        if(m_socket->waitForReadyRead(m_config->timeout)){
            emit scanResultBanner(m_socket->readAll());
        }
        m_socket->close();
    }
    else{
        emit errorLog("Connection Timeout...");
    }

    emit quitThread();
}

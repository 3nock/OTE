#include "ActiveScanner.h"


active::Scanner::Scanner(ScanConfig *scanConfig, active::ScanArguments *scanArguments)
    : AbstractScanner (nullptr),
      m_scanConfig(scanConfig),
      m_scanArguments(scanArguments),
      //...
      m_dns(new QDnsLookup(this)),
      m_socket(new QTcpSocket(this))
{
    m_dns->setType(scanConfig->dnsRecordType);
    m_dns->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));

    connect(m_dns, SIGNAL(finished()), this, SLOT(lookupFinished()));
    connect(this, SIGNAL(anotherLookup()), this, SLOT(lookup()));
}
active::Scanner::~Scanner(){
    delete m_dns;
    delete m_socket;
}

void active::Scanner::lookupFinished(){
    ///
    /// check the results of the lookup if no error occurred emit the results
    /// if error occurred emit appropriate response...
    ///
    switch(m_dns->error()){
        case QDnsLookup::NotFoundError:
            break;
        //...
        case QDnsLookup::NoError:
            ///
            /// If user choosed to check if certain service in that host is available,
            /// connect to that service via specific port n see if connection is
            /// Established else just emit the result...
            ///
            if(m_scanArguments->checkActiveService)
            {
                m_socket->connectToHost(m_dns->name(), m_scanArguments->service);
                if(m_socket->waitForConnected(m_scanConfig->timeout))
                {
                    m_socket->close();
                    emit scanResult(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString());
                }
            }
            else
            {
                emit scanResult(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString());
            }
            break;
        //...
        case QDnsLookup::InvalidReplyError:
            emit errorLog("[ERROR] InvalidReplyError! SUBDOMAIN: "+m_dns->name()+"  NAMESERVER: "+m_dns->nameserver().toString());
            break;
        //...
        case QDnsLookup::InvalidRequestError:
            emit errorLog("[ERROR] InvalidRequestError! SUBDOMAIN: "+m_dns->name()+"  NAMESERVER: "+m_dns->nameserver().toString());
            break;
        //...
        case QDnsLookup::ResolverError:
            emit errorLog("[ERROR] ResolverError! SUBDOMAIN: "+m_dns->name()+"  NAMESERVER: "+m_dns->nameserver().toString());
            break;
        //...
        default:
            break;
    }
    ///
    /// scan progress...
    ///
    m_scanArguments->progress++;
    emit scanProgress(m_scanArguments->progress);
    //...
    emit anotherLookup();
}

void active::Scanner::lookup(){
    m_currentTargetToEnumerate = m_scanArguments->currentTargetToEnumerate;
    m_scanArguments->currentTargetToEnumerate++;
    if(m_currentTargetToEnumerate < m_scanArguments->targetList.count())
    {
        m_dns->setName(m_scanArguments->targetList.at(m_currentTargetToEnumerate));
        m_dns->lookup();
    }
    else
    {
        ///
        /// at the end of the targetList, signal the thread to Quit...
        ///
        emit quitThread();
    }
}

#include "WildcardCheck.h"

WildcardCheck::WildcardCheck(ScanConfig *scanConfig, brute::ScanArguments *scanArguments)
    : m_scanArguments(scanArguments),
      m_scanConfig(scanConfig),
      m_dns(new QDnsLookup(this))
{
    m_dns->setType(m_scanConfig->dnsRecordType);
    m_dns->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
}
WildcardCheck::~WildcardCheck(){
    delete m_dns;
}

void WildcardCheck::enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void WildcardCheck::onLookupFinished(){
    switch(m_dns->error()){
    //...
    case QDnsLookup::NoError:
        m_scanConfig->hasWildcard = true;
        m_scanConfig->wildcardIp = m_dns->hostAddressRecords()[0].value().toString();
        break;
    //...
    case QDnsLookup::NotFoundError:
        m_scanConfig->hasWildcard = false;
        break;
    //....
    default:
        break;
    }
    emit quitThread();
}

void WildcardCheck::lookup(){
    ///
    /// check for random non-existent subdomains name
    /// TODO: make it more advanced...
    ///
    m_dns->setName("3nocknicolas."+m_scanArguments->targetList[m_scanArguments->currentTargetToEnumerate]);
    m_dns->lookup();
}

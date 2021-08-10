#include "src/engines/CommonScanners.h"

/***************************************************************************************
                                  Is Target Active
****************************************************************************************/
TargetCheck::TargetCheck(QObject *parent): QObject (parent)
{
    connect(m_dns, SIGNAL(finished()), this, SLOT(lookupFinished()));
}

void TargetCheck::lookupFinished(){
    if(m_dns->error() == QDnsLookup::NoError)
    {
        emit isActive();
        return;
    }
    else
    {
        lookup();
    }
}

void TargetCheck::lookup(){
    if(m_check == 0){
        m_dns->setType(QDnsLookup::A);
        m_dns->setNameserver(QHostAddress("8.8.8.8"));
    }
    if(m_check == 1){
        m_dns->setType(QDnsLookup::AAAA);
        m_dns->setNameserver(QHostAddress("8.8.8.8"));
    }
    if(m_check == 2){
        emit notActive();
        return;
    }
    m_dns->lookup();
}

void TargetCheck::isTargetActive(QString target){
    m_dns->setName(target);
    //...
    lookup();
}

/***************************************************************************************
                                      GET NameServers
****************************************************************************************/

/***************************************************************************************
                                      CHECK WILDCARDS
****************************************************************************************/
BruteEnumerator_Wildcards::BruteEnumerator_Wildcards(ScanConfig *scanConfig, brute::ScanArguments *scanArguments)
    : m_scanArguments(scanArguments),
      m_scanConfig(scanConfig),
      m_dns(new QDnsLookup(this))
{
    m_dns->setType(m_scanConfig->dnsRecordType);
    m_dns->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
}
BruteEnumerator_Wildcards::~BruteEnumerator_Wildcards(){
    delete m_dns;
}

void BruteEnumerator_Wildcards::enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void BruteEnumerator_Wildcards::onLookupFinished(){
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

void BruteEnumerator_Wildcards::lookup(){
    ///
    /// check for random non-existent subdomains name
    /// TODO: make it more advanced...
    ///
    m_dns->setName("3nocknicolas."+m_scanArguments->targetList[m_scanArguments->currentTargetToEnumerate]);
    m_dns->lookup();
}

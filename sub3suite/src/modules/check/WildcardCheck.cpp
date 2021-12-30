#include "WildcardCheck.h"

WildcardCheck::WildcardCheck(brute::ScanArgs *args)
    : m_args(args),
      m_dns(new QDnsLookup(this))
{
    m_dns->setType(m_args->config->recordType);
    m_dns->setNameserver(QHostAddress(m_args->config->nameservers.at(0)));

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

    case QDnsLookup::NoError:
        //m_args->config->hasWildcard = true;
        //m_args->config->wildcardIp = m_dns->hostAddressRecords()[0].value().toString();
        break;

    case QDnsLookup::NotFoundError:
        //m_args->config->hasWildcard = false;
        break;

    default:
        break;
    }

    emit quitThread();
}

void WildcardCheck::lookup(){
    /*
     check for random non-existent subdomains name
     TODO: make it more advanced...
    */

    m_dns->lookup();
}

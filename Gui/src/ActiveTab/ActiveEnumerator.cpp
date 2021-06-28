#include "ActiveEnumerator.h"


/******************************************************************************************************
 *                                      ACTIVE_SUBDOMAINS ENUMERATOR
 ******************************************************************************************************/
ActiveEnumerator::ActiveEnumerator(ScanArguments_Brute *scanArguments)
    : m_scanArguments(scanArguments), m_dns(new QDnsLookup(this))
{
    m_dns->setType(scanArguments->dnsRecordType);
    m_dns->setNameserver(RandomNameserver(m_scanArguments->useCustomNameServers));
    //...
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
    connect(this, SIGNAL(performAnotherLookup()), this, SLOT(lookup()));
}
ActiveEnumerator::~ActiveEnumerator(){
    delete m_dns;
}

void ActiveEnumerator::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void ActiveEnumerator::onLookupFinished(){
    ///
    /// check the results of the lookup if no error occurred emit the results
    /// if error occurred emit appropriate response...
    ///
    switch(m_dns->error()){
        case QDnsLookup::NotFoundError:
            break;
        //...
        case QDnsLookup::NoError:
            emit resolvedSubdomain(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString());
            break;
        //...
        case QDnsLookup::InvalidReplyError:
            emit scanLog("[ERROR] InvalidReplyError! SUBDOMAIN: "+m_dns->name()+"  NAMESERVER: "+m_dns->nameserver().toString());
            break;
        //...
        case QDnsLookup::InvalidRequestError:
            emit scanLog("[ERROR] InvalidRequestError! SUBDOMAIN: "+m_dns->name()+"  NAMESERVER: "+m_dns->nameserver().toString());
            break;
        //...
        case QDnsLookup::ResolverError:
            emit scanLog("[ERROR] ResolverError! SUBDOMAIN: "+m_dns->name()+"  NAMESERVER: "+m_dns->nameserver().toString());
            break;
        //...
        default:
            break;
    }
    //...
    emit performAnotherLookup();
}

void ActiveEnumerator::lookup(){
    m_currentItemToEnumerate = m_scanArguments->currentWordlistToEnumerate;
    m_scanArguments->currentWordlistToEnumerate++;
    if(m_currentItemToEnumerate < m_scanArguments->wordlist->count()){
        //...
        progressBarValue(m_currentItemToEnumerate);
        m_dns->setName(m_scanArguments->wordlist->item(m_currentItemToEnumerate)->text());
        m_dns->lookup();
        //...
    }
    else{
        ///
        /// at the end of the wordlist, signal the thread to Quit...
        ///
        emit quitThread();
    }
}

void ActiveEnumerator::onStop(){
    emit quitThread();
}

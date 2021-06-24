#include "BruteEnumerator.h"

/********************************************************************************************
 *                              BRUTE-ENUMERATOR
 ********************************************************************************************/

BruteEnumerator_subBrute::BruteEnumerator_subBrute(ScanArguments_Brute *scanArguments)
    : m_scanArguments(scanArguments), m_dns(new QDnsLookup(this))
{
    m_dns->setNameserver(RandomNameserver(m_scanArguments->useCustomNameServers));
    m_dns->setType(m_scanArguments->dnsRecordType);
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
    connect(this, SIGNAL(performAnotherLookup()), this, SLOT(lookup()));
}
BruteEnumerator_subBrute::~BruteEnumerator_subBrute(){
    delete m_dns;
}

void BruteEnumerator_subBrute::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void BruteEnumerator_subBrute::onLookupFinished(){
    ///
    /// check the results of the lookup if no error occurred emit the results
    /// if error occurred emit appropriate response...
    ///
    switch(m_dns->error()){
        case QDnsLookup::NotFoundError:
            break;
        //...
        case QDnsLookup::NoError:
            if(m_scanArguments->checkWildcardSubdomains){
                if(m_scanArguments->usesWildcards){
                    ///
                    /// check if the Ip adress of the subdomain is similar to the wildcard Ip found
                    /// if not similar we emit the results if similar discard the results...
                    ///
                    if(!(m_dns->hostAddressRecords()[0].value().toString() == m_scanArguments->foundWildcardIp)){
                        emit resolvedSubdomain(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString());
                    }
                }
                else{
                    emit resolvedSubdomain(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString());
                }
            }
            else{
                emit resolvedSubdomain(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString());
            }
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
    ///
    /// call the lookup() method to continue another enumuerations...
    ///
    emit performAnotherLookup();
}

void BruteEnumerator_subBrute::lookup(){
    //...
    m_currentItemToEnumerate = m_scanArguments->currentItemToEnumerate;
    m_scanArguments->currentItemToEnumerate++;
    //...
    if(m_currentItemToEnumerate < m_scanArguments->wordlist->count()){
        //...
        m_dns->setName(nameProcessor_subBrute(m_scanArguments->wordlist->item(m_currentItemToEnumerate)->text(), m_scanArguments->targetDomain));
        progressBarValue(m_currentItemToEnumerate);
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

void BruteEnumerator_subBrute::onStop(){
    ///
    /// quiting all running threads upon receiving stop signal...
    ///
    emit quitThread();
}

/******************************************************************************************************
 *                                      TLD-BRUTE ENUMERATOR
 ******************************************************************************************************/
BruteEnumerator_tldBrute::BruteEnumerator_tldBrute(ScanArguments_Brute *scanArguments)
    : m_scanArguments(scanArguments), m_dns(new QDnsLookup(this))
{
    m_dns->setType(scanArguments->dnsRecordType);
    m_dns->setNameserver(RandomNameserver(m_scanArguments->useCustomNameServers));
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
    connect(this, SIGNAL(performAnotherLookup()), this, SLOT(lookup()));
}
BruteEnumerator_tldBrute::~BruteEnumerator_tldBrute(){
    delete m_dns;
}

void BruteEnumerator_tldBrute::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void BruteEnumerator_tldBrute::onLookupFinished(){
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

void BruteEnumerator_tldBrute::lookup(){
    m_currentItemToEnumerate = m_scanArguments->currentItemToEnumerate;
    m_scanArguments->currentItemToEnumerate++;
    if(m_currentItemToEnumerate < m_scanArguments->wordlist->count()){
        progressBarValue(m_currentItemToEnumerate);
        m_dns->setName(nameProcessor_tldBrute(m_scanArguments->wordlist->item(m_currentItemToEnumerate)->text(), m_scanArguments->targetDomain));
        m_dns->lookup();
    }
    else{
        ///
        /// at the end of the wordlist, signal the thread to Quit...
        ///
        emit quitThread();
    }
}

void BruteEnumerator_tldBrute::onStop(){
    emit quitThread();
}



/******************************************************************************************************
 *                                      CHECK WILDCARDS
 ******************************************************************************************************/

BruteEnumerator_Wildcards::BruteEnumerator_Wildcards(ScanArguments_Brute *scanArguments)
    : m_scanArguments(scanArguments), m_dns(new QDnsLookup(this))
{
    m_dns->setType(m_scanArguments->dnsRecordType);
    m_dns->setNameserver(RandomNameserver(m_scanArguments->useCustomNameServers));
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
}
BruteEnumerator_Wildcards::~BruteEnumerator_Wildcards(){
    delete m_dns;
}

void BruteEnumerator_Wildcards::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void BruteEnumerator_Wildcards::onLookupFinished(){
    switch(m_dns->error()){
    //...
    case QDnsLookup::NoError:
        m_scanArguments->usesWildcards = true;
        m_scanArguments->foundWildcardIp = m_dns->hostAddressRecords()[0].value().toString();
        break;
    //...
    case QDnsLookup::NotFoundError:
        m_scanArguments->usesWildcards = false;
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
    m_dns->setName("3nocknicolas."+m_scanArguments->targetDomain);
    m_dns->lookup();
}

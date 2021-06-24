#include "BruteEnumerator.h"

/********************************************************************************************
 *                              BRUTE-ENUMERATOR
 ********************************************************************************************/

Enumerator_subBrute::Enumerator_subBrute(ScanArguments_Brute *scanArguments)
    : m_scanArguments(scanArguments), m_dns(new QDnsLookup(this))
{
    m_dns->setNameserver(RandomNameserver(m_scanArguments->useCustomNameServers));
    m_dns->setType(m_scanArguments->dnsRecordType);
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
    connect(this, SIGNAL(performAnotherLookup()), this, SLOT(lookup()));
}
Enumerator_subBrute::~Enumerator_subBrute(){
    delete m_dns;
}

void Enumerator_subBrute::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void Enumerator_subBrute::onLookupFinished(){
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

void Enumerator_subBrute::lookup(){
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

void Enumerator_subBrute::onStop(){
    ///
    /// quiting all running threads upon receiving stop signal...
    ///
    emit quitThread();
}

/******************************************************************************************************
 *                                      TLD-BRUTE ENUMERATOR
 ******************************************************************************************************/
Enumerator_tldBrute::Enumerator_tldBrute(ScanArguments_Brute *scanArguments)
    : m_scanArguments(scanArguments), m_dns(new QDnsLookup(this))
{
    m_dns->setType(scanArguments->dnsRecordType);
    m_dns->setNameserver(RandomNameserver(m_scanArguments->useCustomNameServers));
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
    connect(this, SIGNAL(performAnotherLookup()), this, SLOT(lookup()));
}
Enumerator_tldBrute::~Enumerator_tldBrute(){
    delete m_dns;
}

void Enumerator_tldBrute::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void Enumerator_tldBrute::onLookupFinished(){
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

void Enumerator_tldBrute::lookup(){
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

void Enumerator_tldBrute::onStop(){
    emit quitThread();
}



/******************************************************************************************************
 *                                      CHECK WILDCARDS
 ******************************************************************************************************/

Enumerator_Wildcards::Enumerator_Wildcards(ScanArguments_Brute *scanArguments)
    : m_scanArguments(scanArguments), m_dns(new QDnsLookup(this))
{
    m_dns->setType(m_scanArguments->dnsRecordType);
    m_dns->setNameserver(RandomNameserver(m_scanArguments->useCustomNameServers));
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
}
Enumerator_Wildcards::~Enumerator_Wildcards(){
    delete m_dns;
}

void Enumerator_Wildcards::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void Enumerator_Wildcards::onLookupFinished(){
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

void Enumerator_Wildcards::lookup(){
    ///
    /// check for random non-existent subdomains name
    /// TODO: make it more advanced...
    ///
    m_dns->setName("3nocknicolas."+m_scanArguments->targetDomain);
    m_dns->lookup();
}

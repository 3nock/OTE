#include "LevelEnumerator.h"

LevelEnumerator::LevelEnumerator(ScanArguments_level *scanArguments)
    : m_scanArguments(scanArguments), m_dns(new QDnsLookup)
{
    m_dns->setNameserver(QHostAddress("8.8.8.8"));
    m_dns->setType(QDnsLookup::A);
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
    connect(this, SIGNAL(performAnotherLookup()), this, SLOT(lookup()));
}
LevelEnumerator::~LevelEnumerator(){
    delete m_dns;
}

void LevelEnumerator::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void LevelEnumerator::lookup(){
    m_currentItemToEnumerate = m_scanArguments->currentWordlistToEnumerate;
    m_scanArguments->currentWordlistToEnumerate++;
    if(m_currentItemToEnumerate < m_scanArguments->wordlist->count())
    {
        //...
        //m_dns->setName(m_scanArguments->wordlist->item(m_currentItemToEnumerate)->text(), m_scanArguments->model_subdomains->item(m_scanArguments->currentSubdomainToEnumerate, 0)->text()));
        //...
        m_dns->lookup();
    }
    else
    {
        if(m_scanArguments->currentSubdomainToEnumerate < m_scanArguments->model_subdomains->rowCount()-1)
        {
            m_scanArguments->currentSubdomainToEnumerate++;
            m_scanArguments->currentWordlistToEnumerate = 0;
            emit performAnotherLookup();
            return;
        }
        ///
        /// Reached end of the subdomains and wordlists to enumerate...
        ///
        emit quitThread();
    }
}

void LevelEnumerator::onLookupFinished(){
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

void LevelEnumerator::onStop(){
    quitThread();
}

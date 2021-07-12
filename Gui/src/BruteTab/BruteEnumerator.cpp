#include "BruteEnumerator.h"

/********************************************************************************************
 *                              BRUTE-ENUMERATOR
 ********************************************************************************************/

BruteEnumerator::BruteEnumerator(ScanConfig* scanConfig, brute::ScanArguments *scanArguments)
    : m_scanArguments(scanArguments),
      m_scanConfig(scanConfig),
      m_dns(new QDnsLookup)
{
    m_dns->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));
    m_dns->setType(m_scanConfig->dnsRecordType);
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(lookupFinished()));
    connect(this, SIGNAL(performAnotherLookup()), this, SLOT(lookup()));
}
BruteEnumerator::~BruteEnumerator(){
    delete m_dns;
}

void BruteEnumerator::enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void BruteEnumerator::lookupFinished(){
    ///
    /// check the results of the lookup if no error occurred emit the results
    /// if error occurred emit appropriate response...
    ///
    switch(m_dns->error())
    {
        case QDnsLookup::NotFoundError:
            break;
        //...
        case QDnsLookup::NoError:
            if(m_scanArguments->subBrute && m_scanConfig->checkWildcard && m_scanConfig->hasWildcard){
                ///
                /// check if the Ip adress of the subdomain is similar to the wildcard Ip found
                /// if not similar we emit the results if similar discard the results...
                ///
                if(!(m_dns->hostAddressRecords()[0].value().toString() == m_scanConfig->wildcardIp)){
                    emit scanResult(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString());
                }
            }
            else{
                emit scanResult(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString());
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
    /// scan progress...
    ///
    m_scanArguments->progress++;
    emit progress(m_scanArguments->progress);
    //...
    emit performAnotherLookup();
}

void BruteEnumerator::lookup(){
    //...
    m_currentWordlistToEnumerate = m_scanArguments->currentWordlistToEnumerate;
    m_currentTargetToEnumerate = m_scanArguments->currentTargetToEnumerate;
    m_scanArguments->currentWordlistToEnumerate++;
    //...
    if(m_currentWordlistToEnumerate < m_scanArguments->wordlist->count())
    {
        if(m_scanArguments->subBrute)
        {
            m_dns->setName(m_scanArguments->wordlist->item(m_currentWordlistToEnumerate)->text()+"."+m_scanArguments->targetList[m_currentTargetToEnumerate]);
        }
        if(m_scanArguments->tldBrute)
        {
            m_dns->setName(m_scanArguments->targetList[m_currentTargetToEnumerate]+"."+m_scanArguments->wordlist->item(m_currentWordlistToEnumerate)->text());
        }
        m_dns->lookup();
    }
    else
    {
        ///
        /// Reached end of the wordlist...
        /// if there are multiple targets, choose another target and start afresh...
        ///
        if(m_scanArguments->currentTargetToEnumerate < m_scanArguments->targetList.count()-1)
        {
            m_scanArguments->currentTargetToEnumerate++;
            m_scanArguments->currentWordlistToEnumerate = 0;
            emit performAnotherLookup();
        }
        else
        {
            emit quitThread();
            return;
        }
    }
}

void BruteEnumerator::onStop(){
    ///
    /// quiting all running threads upon receiving stop signal...
    ///
    emit quitThread();
}


/******************************************************************************************************
 *                                      CHECK WILDCARDS
 ******************************************************************************************************/

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

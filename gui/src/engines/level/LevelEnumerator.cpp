#include "LevelEnumerator.h"

LevelEnumerator::LevelEnumerator(ScanConfig *scanConfig, level::ScanArguments *scanArguments)
    : m_scanConfig(scanConfig),
      m_scanArguments(scanArguments),
      m_dns(new QDnsLookup)
{
    m_dns->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));
    m_dns->setType(m_scanConfig->dnsRecordType);
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(lookupFinished()));
    connect(this, SIGNAL(performAnotherLookup()), this, SLOT(lookup()));
}
LevelEnumerator::~LevelEnumerator(){
    delete m_dns;
}

void LevelEnumerator::enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void LevelEnumerator::lookupFinished(){
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
            if(m_scanConfig->checkWildcard && m_scanConfig->hasWildcard){
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

void LevelEnumerator::lookup(){
    //...
    m_currentWordlistToEnumerate = m_scanArguments->currentWordlistToEnumerate;
    m_scanArguments->currentWordlistToEnumerate++;
    m_currentTargetToEnumerate = m_scanArguments->currentTargetToEnumerate;
    //...
    if(m_currentWordlistToEnumerate < m_scanArguments->wordlist->count())
    {
        m_dns->setName(m_scanArguments->wordlist->item(m_currentWordlistToEnumerate)->text()+"."+m_scanArguments->targetList->item(m_currentTargetToEnumerate)->text());
        m_dns->lookup();
    }
    ///
    /// reached end of the wordlist...
    ///
    else
    {
        ///
        /// choose another target and start again...
        ///
        if(m_scanArguments->currentTargetToEnumerate < m_scanArguments->targetList->count()-1)
        {
            m_scanArguments->currentTargetToEnumerate++;
            m_scanArguments->currentWordlistToEnumerate = 0;
            emit performAnotherLookup();
        }
        else
        {
            emit quitThread();
        }
    }
}

void LevelEnumerator::onStop(){
    ///
    /// quiting all running threads upon receiving stop signal...
    ///
    emit quitThread();
}

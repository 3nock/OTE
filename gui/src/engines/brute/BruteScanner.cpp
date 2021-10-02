#include "BruteScanner.h"


brute::Scanner::Scanner(ScanConfig* scanConfig, brute::ScanArguments *scanArguments)
    : m_scanArguments(scanArguments),
      m_scanConfig(scanConfig),
      m_dns(new QDnsLookup)
{
    m_dns->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));
    m_dns->setType(m_scanConfig->dnsRecordType);
    //...
    connect(m_dns, SIGNAL(finished()), this, SLOT(lookupFinished()));
    connect(this, SIGNAL(anotherLookup()), this, SLOT(lookup()));
}
brute::Scanner::~Scanner(){
    delete m_dns;
}

void brute::Scanner::lookupFinished(){
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
                    emit scanResult(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString(), m_scanArguments->targetList[m_currentTargetToEnumerate]);
                }
            }
            else{
                emit scanResult(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString(), m_scanArguments->targetList[m_currentTargetToEnumerate]);
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

void brute::Scanner::lookup(){
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
            emit anotherLookup();
        }
        else
        {
            emit quitThread();
            return;
        }
    }
}

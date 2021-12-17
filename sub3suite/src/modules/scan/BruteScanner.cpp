#include "BruteScanner.h"


brute::Scanner::Scanner(brute::ScanArgs *args)
    : m_args(args),
      m_dns(new QDnsLookup)
{
    m_dns->setNameserver(RandomNameserver(m_args->config->useCustomNameServers));
    m_dns->setType(m_args->config->dnsRecordType);

    connect(m_dns, SIGNAL(finished()), this, SLOT(lookupFinished()));
    connect(this, SIGNAL(anotherLookup()), this, SLOT(lookup()));
}
brute::Scanner::~Scanner(){
    delete m_dns;
}

void brute::Scanner::lookupFinished(){
    /*
     check the results of the lookup if no error occurred emit the results
     if error occurred emit appropriate response...
    */
    switch(m_dns->error())
    {
        case QDnsLookup::NotFoundError:
            break;

        case QDnsLookup::NoError:
            if(m_args->subBrute && m_args->config->checkWildcard && m_args->config->hasWildcard)
            {
                /*
                 check if the Ip adress of the subdomain is similar to the wildcard Ip found
                 if not similar we emit the results if similar discard the results...
                */
                if(!(m_dns->hostAddressRecords()[0].value().toString() == m_args->config->wildcardIp))
                    emit scanResult(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString(), m_args->targetList[m_currentTargetToEnumerate]);
            }
            else
                emit scanResult(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString(), m_args->targetList[m_currentTargetToEnumerate]);
            break;

        case QDnsLookup::InvalidReplyError:
            emit errorLog("[ERROR] InvalidReplyError! SUBDOMAIN: "+m_dns->name()+"  NAMESERVER: "+m_dns->nameserver().toString());
            break;

        case QDnsLookup::InvalidRequestError:
            emit errorLog("[ERROR] InvalidRequestError! SUBDOMAIN: "+m_dns->name()+"  NAMESERVER: "+m_dns->nameserver().toString());
            break;

        case QDnsLookup::ResolverError:
            emit errorLog("[ERROR] ResolverError! SUBDOMAIN: "+m_dns->name()+"  NAMESERVER: "+m_dns->nameserver().toString());
            break;

        default:
            break;
    }

    /* scan progress */
    m_args->progress++;

    /* send results and continue scan */
    emit scanProgress(m_args->progress);
    emit anotherLookup();
}

void brute::Scanner::lookup(){
    m_currentWordlistToEnumerate = m_args->currentWordlistToEnumerate;
    m_currentTargetToEnumerate = m_args->currentTargetToEnumerate;
    m_args->currentWordlistToEnumerate++;

    if(m_currentWordlistToEnumerate < m_args->wordlist.count())
    {
        if(m_args->subBrute)
            m_dns->setName(m_args->wordlist.at(m_currentWordlistToEnumerate)+"."+m_args->targetList.at(m_currentTargetToEnumerate));

        if(m_args->tldBrute)
            m_dns->setName(m_args->targetList.at(m_currentTargetToEnumerate)+"."+m_args->wordlist.at(m_currentWordlistToEnumerate));

        m_dns->lookup();
    }
    else
    {
        /*
         Reached end of the wordlist.
         If there are multiple targets, choose another target and start afresh
        */
        if(m_args->currentTargetToEnumerate < m_args->targetList.count()-1)
        {
            m_args->currentTargetToEnumerate++;
            m_args->currentWordlistToEnumerate = 0;
            emit anotherLookup();
        }
        else{
            emit quitThread();
            return;
        }
    }
}

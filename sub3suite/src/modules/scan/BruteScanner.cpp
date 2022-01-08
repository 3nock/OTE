#include "BruteScanner.h"


brute::Scanner::Scanner(brute::ScanArgs *args): AbstractScanner(nullptr),
    m_args(args),
    m_dns(new QDnsLookup(this))
{
    m_dns->setNameserver(QHostAddress(m_args->config->nameservers.at(0)));
    m_dns->setType(m_args->config->recordType);

    connect(m_dns, &QDnsLookup::finished, this, &brute::Scanner::lookupFinished);
    connect(this, &brute::Scanner::next, this, &brute::Scanner::lookup);
}
brute::Scanner::~Scanner(){
    delete m_dns;
}

void brute::Scanner::lookup(){
    if(m_args->subdomain){
        RetVal retVal = brute::lookupSubdomain(m_dns, m_args);

        if(retVal.lookup)
            m_dns->lookup();
        if(retVal.next)
            emit next();
        if(retVal.quit)
            emit quitThread();
    }

    if(m_args->tld){
        RetVal retVal = brute::lookupTLD(m_dns, m_args);

        if(retVal.lookup)
            m_dns->lookup();
        if(retVal.next)
            emit next();
        if(retVal.quit)
            emit quitThread();
    }
}

void brute::Scanner::lookupFinished(){
    switch(m_dns->error()){
    case QDnsLookup::NotFoundError:
        break;

    case QDnsLookup::NoError:
        if(m_dns->hostAddressRecords().isEmpty())
            break;
        emit scanResult(m_dns->name(), m_dns->hostAddressRecords().at(0).value().toString());
        break;

    default:
        brute::ScanLog log;
        log.message = m_dns->errorString();
        log.target = m_dns->name();
        log.nameserver = m_dns->nameserver().toString();
        emit scanLog(log);
        break;
    }

    /* scan progress */
    m_args->progress++;

    /* send results and continue scan */
    emit scanProgress(m_args->progress);
    emit next();
}

RetVal brute::lookupSubdomain(QDnsLookup *dns, brute::ScanArgs *args){
    RetVal retVal;

    /* lock */
    QMutex mutex;
    mutex.lock();

    if(args->currentWordlist < args->wordlist.length())
    {
        /* append to target then set the name */
        dns->setName(args->wordlist.at(args->currentWordlist)+"."+args->currentTarget);

        /* lookup */
        retVal.lookup = true;

        /* next wordlist */
        args->currentWordlist++;
    }
    /* Reached end of the wordlist */
    else
    {
         /* next target */
        if(args->targets.isEmpty()){
            retVal.quit = true;
        }
        else{
            args->currentWordlist = 0;
            args->currentTarget = args->targets.dequeue();
            retVal.next = true;
        }
    }

    /* unlock */
    mutex.unlock();
    return retVal;
}

RetVal brute::lookupTLD(QDnsLookup *dns, brute::ScanArgs *args){
    RetVal retVal;

    /* lock */
    QMutex mutex;
    mutex.lock();

    if(args->currentWordlist < args->wordlist.length())
    {
        /* append to target then set the name */
        dns->setName(args->currentTarget+"."+args->wordlist.at(args->currentWordlist));

        /* lookup */
        retVal.lookup = true;

        /* next wordlist */
        args->currentWordlist++;
    }
    /* Reached end of the wordlist */
    else
    {
         /* next target */
        if(args->targets.isEmpty())
            retVal.quit = true;
        else{
            args->currentWordlist = 0;
            args->currentTarget = args->targets.dequeue();
            retVal.next = true;
        }
    }

    /* unlock */
    mutex.unlock();
    return retVal;
}

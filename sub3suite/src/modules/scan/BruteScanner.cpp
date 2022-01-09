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
    switch(m_args->output)
    {
    case OUTPUT::SUBDOMAIN:
        switch(brute::lookupSubdomain(m_dns, m_args)){
        case RETVAL::LOOKUP:
            m_dns->lookup();
            break;
        case RETVAL::NEXT:
            emit next();
            break;
        case RETVAL::QUIT:
            emit quitThread();
            break;
        }
        break;

    case OUTPUT::TLD:
        switch(brute::lookupTLD(m_dns, m_args)){
        case RETVAL::LOOKUP:
            m_dns->lookup();
            break;
        case RETVAL::NEXT:
            emit next();
            break;
        case RETVAL::QUIT:
            emit quitThread();
            break;
        }
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
        scan::Log log;
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

RETVAL brute::lookupSubdomain(QDnsLookup *dns, brute::ScanArgs *args){
    /* lock */
    QMutexLocker(&args->mutex);

    /* check if Reached end of the wordlist */
    if(args->currentWordlist < args->wordlist.length())
    {
        /* append to target then set the name */
        dns->setName(args->wordlist.at(args->currentWordlist)+"."+args->currentTarget);

        /* next wordlist */
        args->currentWordlist++;

        return RETVAL::LOOKUP;
    }
    else{
        if(args->targets.isEmpty())
            return RETVAL::QUIT;
        else{
            /* next target */
            args->currentWordlist = 0;
            args->currentTarget = args->targets.dequeue();
            return RETVAL::NEXT;
        }
    }
}

RETVAL brute::lookupTLD(QDnsLookup *dns, brute::ScanArgs *args){
    /* lock */
    QMutexLocker(&args->mutex);

    /* check if Reached end of the wordlist */
    if(args->currentWordlist < args->wordlist.length())
    {
        /* append to target then set the name */
        dns->setName(args->currentTarget+"."+args->wordlist.at(args->currentWordlist));

        /* next wordlist */
        args->currentWordlist++;

        return RETVAL::LOOKUP;
    }
    else{
        if(args->targets.isEmpty())
            return RETVAL::QUIT;
        else{
            /* next target */
            args->currentWordlist = 0;
            args->currentTarget = args->targets.dequeue();
            return RETVAL::NEXT;
        }
    }
}

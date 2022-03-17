/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "BruteScanner.h"
#include "src/utils/s3s.h"


brute::Scanner::Scanner(brute::ScanArgs *args): AbstractScanner(nullptr),
    m_args(args),
    m_dns(new QDnsLookup(this)),
    m_dns_wildcard(new QDnsLookup(this))
{
    m_dns->setType(m_args->config->recordType);

    /* setting nameserver */
    QString nameserver = m_args->config->nameservers.dequeue();
    m_dns->setNameserver(QHostAddress(nameserver));
    m_args->config->nameservers.enqueue(nameserver);

    connect(m_dns_wildcard, &QDnsLookup::finished, this, &brute::Scanner::lookupFinished_wildcard);
    connect(m_dns, &QDnsLookup::finished, this, &brute::Scanner::lookupFinished);
    connect(this, &brute::Scanner::next, this, &brute::Scanner::lookup);
}
brute::Scanner::~Scanner(){
    delete m_dns;
}

void brute::Scanner::lookupFinished(){
    switch(m_dns->error()){
    case QDnsLookup::NotFoundError:
        break;

    case QDnsLookup::OperationCancelledError:
        log.message = "Operation Cancelled due to Timeout";
        log.target = m_dns->name();
        log.nameserver = m_dns->nameserver().toString();
        emit scanLog(log);
        break;

    case QDnsLookup::NoError:
        if(m_dns->hostAddressRecords().isEmpty())
            break;

        switch (m_args->config->recordType) {
        case QDnsLookup::A:
        {
            QString address = m_dns->hostAddressRecords().at(0).value().toString();
            if(m_args->config->checkWildcard && has_wildcards){
                if(address == wildcard_ip)
                    break;
            }
            s3s_struct::HOST host;
            host.host = m_dns->name();
            host.ipv4 = address;
            emit scanResult(host);
        }
            break;
        case QDnsLookup::AAAA:
        {
            QString address = m_dns->hostAddressRecords().at(0).value().toString();
            if(m_args->config->checkWildcard && has_wildcards){
                if(address == wildcard_ip)
                    break;
            }
            s3s_struct::HOST host;
            host.host = m_dns->name();
            host.ipv6 = address;
            emit scanResult(host);
        }
            break;
        case QDnsLookup::ANY:
        {
            s3s_struct::HOST host;
            host.host = m_dns->name();
            foreach(const QDnsHostAddressRecord &addr, m_dns->hostAddressRecords()){
                if(addr.value().protocol() == QAbstractSocket::IPv4Protocol)
                    host.ipv4 = addr.value().toString();
                if(addr.value().protocol() == QAbstractSocket::IPv6Protocol)
                    host.ipv6 = addr.value().toString();
            }
            emit scanResult(host);
        }
            break;
        default:
            break;
        }
        break;

    default:
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

void brute::Scanner::lookup(){
    if(m_args->reScan){
        switch(brute::getTarget_reScan(m_dns, m_args)){
        case RETVAL::LOOKUP:
            m_dns->lookup();
            s3s_LookupTimeout::set(m_dns, m_args->config->timeout);
            break;
        case RETVAL::QUIT:
            emit quitThread();
            break;
        default:
            break;
        }
        return;
    }

    switch(m_args->output)
    {
    case OUTPUT::SUBDOMAIN:
        switch(brute::getTarget_subdomain(this, m_dns, m_args)){
        case RETVAL::LOOKUP:
            m_dns->lookup();
            s3s_LookupTimeout::set(m_dns, m_args->config->timeout);
            break;
        case RETVAL::NEXT_LEVEL:
            emit nextLevel();
            emit newProgress(m_args->targets.size() * m_args->wordlist.size());
            emit next();
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
        switch(brute::getTarget_tld(m_dns, m_args)){
        case RETVAL::LOOKUP:
            m_dns->lookup();
            s3s_LookupTimeout::set(m_dns, m_args->config->timeout);
            break;
        case RETVAL::NEXT_LEVEL:
            emit nextLevel();
            emit newProgress(m_args->targets.size() * m_args->wordlist.size());
            emit next();
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

///
/// wildcard scan...
///
void brute::Scanner::lookup_wildcard(){
    m_dns_wildcard->setType(m_dns->type());
    m_dns_wildcard->setNameserver(m_dns->nameserver());
    m_dns_wildcard->setName(m_args->currentTarget);
    m_dns_wildcard->lookup();
    s3s_LookupTimeout::set(m_dns, m_args->config->timeout);
}

void brute::Scanner::lookupFinished_wildcard(){
    switch(m_dns_wildcard->error()){
    case QDnsLookup::NoError:
    {
        if(m_dns_wildcard->hostAddressRecords().isEmpty())
            break;

        s3s_struct::Wildcard wcard;
        wcard.wildcard = "*."+m_args->currentTarget;

        QString address = m_dns_wildcard->hostAddressRecords()[0].value().toString();
        if(m_dns_wildcard->type() == QDnsLookup::A)
            wcard.ipv4 = address;
        else
            wcard.ipv6 = address;

        has_wildcards = true;
        wildcard_ip = address;

        emit wildcard(wcard);
    }
        break;
    default:
        has_wildcards = false;
        break;
    }
}

///
/// getting targets...
///
RETVAL brute::getTarget_subdomain(brute::Scanner *scanner, QDnsLookup *dns, brute::ScanArgs *args){
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
        {
            if(args->config->multiLevelScan && // if it is a multi-level scan and
              !args->nextLevelTargets.isEmpty() && // the targets for next level aren't empty and
              (args->currentLevel < args->config->levels)) // it is not the last level
            {
                /* TODO:
                 *      Make sure all threads are done before continuing...
                 */
                args->currentLevel++;
                args->targets = args->nextLevelTargets;
                args->currentTarget = args->targets.dequeue();
                args->currentWordlist = 0;

                /* wildcard check */
                if(args->config->checkWildcard)
                    scanner->lookup_wildcard();

                return RETVAL::NEXT_LEVEL;
            }
            else
                return RETVAL::QUIT;
        }
        else{
            /* next target */
            args->currentWordlist = 0;
            args->currentTarget = args->targets.dequeue();

            /* wildcard check */
            if(args->config->checkWildcard)
                scanner->lookup_wildcard();

            return RETVAL::NEXT;
        }
    }
}

RETVAL brute::getTarget_tld(QDnsLookup *dns, brute::ScanArgs *args){
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
        if(args->targets.isEmpty()){
            if(args->config->multiLevelScan && // if it is a multi-level scan and
              !args->nextLevelTargets.isEmpty() && // the targets for next level aren't empty and
              (args->currentLevel < args->config->levels)) // it is not the last level
            {
                /* TODO:
                 *      Make sure all threads are done before continuing...
                 */
                args->currentLevel++;
                args->targets = args->nextLevelTargets;
                args->currentTarget = args->targets.dequeue();
                args->currentWordlist = 0;
                return RETVAL::NEXT_LEVEL;
            }
            else
                return RETVAL::QUIT;
        }
        else{
            /* next target */
            args->currentWordlist = 0;
            args->currentTarget = args->targets.dequeue();
            return RETVAL::NEXT;
        }
    }
}

RETVAL brute::getTarget_reScan(QDnsLookup *dns, brute::ScanArgs *args){
    /* lock */
    QMutexLocker(&args->mutex);

    if(!args->targets.isEmpty()){
        dns->setName(args->targets.dequeue());
        return RETVAL::LOOKUP;
    }
    else
        return RETVAL::QUIT;
}

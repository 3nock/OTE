/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "HostScanner.h"
#include "src/utils/s3s.h"


host::Scanner::Scanner(host::ScanArgs *args): AbstractScanner(nullptr),
      m_args(args),
      m_dns(new QDnsLookup(this))
{
    m_dns->setType(m_args->config->recordType);

    /* setting nameserver */
    QString nameserver = m_args->config->nameservers.dequeue();
    m_dns->setNameserver(QHostAddress(nameserver));
    m_args->config->nameservers.enqueue(nameserver);

    connect(m_dns, &QDnsLookup::finished, this, &host::Scanner::lookupFinished);
    connect(this, &host::Scanner::next, this, &host::Scanner::lookup);
}
host::Scanner::~Scanner(){
    delete m_dns;
}

void host::Scanner::lookupFinished(){
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
            s3s_struct::HOST host;
            host.host = m_dns->name();
            host.ipv4 = m_dns->hostAddressRecords().at(0).value().toString();
            emit scanResult(host);
        }
            break;
        case QDnsLookup::AAAA:
        {
            s3s_struct::HOST host;
            host.host = m_dns->name();
            host.ipv6 = m_dns->hostAddressRecords().at(0).value().toString();
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

    /* send results and continue scan */
    m_args->progress++;
    emit scanProgress(m_args->progress);
    emit next();
}

void host::Scanner::lookup(){
    switch (getTarget(m_dns, m_args)) {
    case RETVAL::LOOKUP:
        m_dns->lookup();
        if(m_args->config->setTimeout)
            s3s_LookupTimeout::set(m_dns, m_args->config->timeout);
        break;
    default:
        emit quitThread();
        break;
    }
}

RETVAL host::getTarget(QDnsLookup *dns, host::ScanArgs *args){
    QMutexLocker(&args->mutex);

    if(!args->targets.isEmpty()){
        dns->setName(args->targets.dequeue());
        return RETVAL::LOOKUP;
    }
    else
        return RETVAL::QUIT;
}

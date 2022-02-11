/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "ActiveScanner.h"


active::Scanner::Scanner(active::ScanArgs *args): AbstractScanner (nullptr),
      m_args(args),
      m_dns(new QDnsLookup(this)),
      m_socket(new QTcpSocket(this))
{
    m_dns->setType(m_args->config->recordType);
    m_dns->setNameserver(QHostAddress(m_args->config->nameservers.at(0)));

    connect(m_dns, &QDnsLookup::finished, this, &active::Scanner::lookupFinished);
    connect(this, &active::Scanner::next, this, &active::Scanner::lookup);
}
active::Scanner::~Scanner(){
    delete m_socket;
    delete m_dns;
}

void active::Scanner::lookupFinished(){
    switch(m_dns->error()){
    case QDnsLookup::NotFoundError:
        break;

    case QDnsLookup::NoError:
        if(m_dns->hostAddressRecords().isEmpty())
            break;
        else
        {
            s3s_struct::HOST host;
            host.host = m_dns->name();
            if(m_args->config->recordType == QDnsLookup::A)
                host.ipv4 = m_dns->hostAddressRecords().at(0).value().toString();
            else
                host.ipv6 = m_dns->hostAddressRecords().at(0).value().toString();

            emit scanResult(host);
            break;
        }
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

void active::Scanner::lookup(){
    switch (lookupActiveDNS(m_dns, m_args)) {
    case RETVAL::LOOKUP:
        m_dns->lookup();
        break;
    case RETVAL::QUIT:
        emit quitThread();
        break;
    default:
        emit quitThread();
    }
}

RETVAL active::lookupActiveDNS(QDnsLookup *dns, active::ScanArgs *args){
    /* lock */
    QMutexLocker(&args->mutex);

    if(!args->targets.isEmpty()){
        dns->setName(args->targets.dequeue());
        return RETVAL::LOOKUP;
    }
    else
        return RETVAL::QUIT;
}

RETVAL active::lookupActiveService(active::ScanArgs *args){
    /* lock

    m_socket->connectToHost(m_dns->name(), m_args->service);
    if(m_socket->waitForConnected(m_args->config->timeout))
    {
        m_socket->close();
        emit scanResult(m_dns->name(), m_dns->hostAddressRecords()[0].value().toString());
    }
    */
    return RETVAL::QUIT;
}

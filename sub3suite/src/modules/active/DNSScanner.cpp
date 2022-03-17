/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include <QDnsLookup>
#include "DNSScanner.h"
#include "src/utils/s3s.h"


dns::Scanner::Scanner(dns::ScanArgs *args): AbstractScanner (nullptr),
      m_args(args),
      m_dns_a(new QDnsLookup(this)),
      m_dns_aaaa(new QDnsLookup(this)),
      m_dns_mx(new QDnsLookup(this)),
      m_dns_ns(new QDnsLookup(this)),
      m_dns_txt(new QDnsLookup(this)),
      m_dns_cname(new QDnsLookup(this)),
      m_dns_srv(new QDnsLookup(this)),
      m_dns_any(new QDnsLookup(this))
{
    connect(this, &dns::Scanner::next, this, &dns::Scanner::lookup);

    m_dns_a->setType(QDnsLookup::A);
    m_dns_aaaa->setType(QDnsLookup::AAAA);
    m_dns_mx->setType(QDnsLookup::MX);
    m_dns_ns->setType(QDnsLookup::NS);
    m_dns_txt->setType(QDnsLookup::TXT);
    m_dns_cname->setType(QDnsLookup::CNAME);
    m_dns_srv->setType(QDnsLookup::SRV);
    m_dns_any->setType(QDnsLookup::ANY);

    /* setting nameserver */
    QString nameserver = m_args->config->nameservers.dequeue();
    m_dns_a->setNameserver(QHostAddress(nameserver));
    m_dns_a->setNameserver(QHostAddress(nameserver));
    m_dns_mx->setNameserver(QHostAddress(nameserver));
    m_dns_ns->setNameserver(QHostAddress(nameserver));
    m_dns_txt->setNameserver(QHostAddress(nameserver));
    m_dns_cname->setNameserver(QHostAddress(nameserver));
    m_dns_srv->setNameserver(QHostAddress(nameserver));
    m_dns_any->setNameserver(QHostAddress(nameserver));
    m_args->config->nameservers.enqueue(nameserver);

    connect(m_dns_srv, &QDnsLookup::finished, this, &dns::Scanner::lookupFinished_srv);
    connect(m_dns_a, &QDnsLookup::finished, this, &dns::Scanner::lookupFinished_a);
    connect(m_dns_aaaa, &QDnsLookup::finished, this, &dns::Scanner::lookupFinished_aaaa);
    connect(m_dns_mx, &QDnsLookup::finished, this, &dns::Scanner::lookupFinished_mx);
    connect(m_dns_ns, &QDnsLookup::finished, this, &dns::Scanner::lookupFinished_ns);
    connect(m_dns_txt, &QDnsLookup::finished, this, &dns::Scanner::lookupFinished_txt);
    connect(m_dns_cname, &QDnsLookup::finished, this, &dns::Scanner::lookupFinished_cname);
    connect(m_dns_any, &QDnsLookup::finished, this, &dns::Scanner::lookupFinished_any);
}
dns::Scanner::~Scanner(){
    delete m_dns_any;
    delete m_dns_srv;
    delete m_dns_cname;
    delete m_dns_txt;
    delete m_dns_ns;
    delete m_dns_mx;
    delete m_dns_aaaa;
    delete m_dns_a;
}

void dns::Scanner::lookupFinished_a(){
    switch(m_dns_a->error()){
    case QDnsLookup::NotFoundError:
        break;
    case QDnsLookup::OperationCancelledError:
        log.message = "Operation Cancelled due to Timeout";
        log.target = m_dns_any->name();
        log.nameserver = m_dns_any->nameserver().toString();
        log.recordType = "A";
        emit scanLog(log);
        break;
    case QDnsLookup::NoError:
        foreach(const QDnsHostAddressRecord &record, m_dns_a->hostAddressRecords())
            m_result.A.insert(record.value().toString());
        has_record = true;
        break;
    default:
        log.message = m_dns_a->errorString();
        log.target = m_dns_a->name();
        log.nameserver = m_dns_a->nameserver().toString();
        log.recordType = "A";
        emit scanLog(log);
        break;
    }

    /* if this is the last lookup, save send results then go to next lookup */
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        if(has_record)
            emit scanResult(m_result);
        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit next();
    }
}

void dns::Scanner::lookupFinished_aaaa(){
    switch(m_dns_aaaa->error()){
    case QDnsLookup::NotFoundError:
        break;
    case QDnsLookup::OperationCancelledError:
        log.message = "Operation Cancelled due to Timeout";
        log.target = m_dns_any->name();
        log.nameserver = m_dns_any->nameserver().toString();
        log.recordType = "AAAA";
        emit scanLog(log);
        break;
    case QDnsLookup::NoError:
        foreach(const QDnsHostAddressRecord &record, m_dns_aaaa->hostAddressRecords())
            m_result.AAAA.insert(record.value().toString());
        has_record = true;
        break;
    default:
        log.message = m_dns_aaaa->errorString();
        log.target = m_dns_aaaa->name();
        log.nameserver = m_dns_aaaa->nameserver().toString();
        log.recordType = "AAAA";
        emit scanLog(log);
        break;
    }

    /* if this is the last lookup, save send results then go to next lookup */
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        if(has_record)
            emit scanResult(m_result);
        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit next();
    }
}

void dns::Scanner::lookupFinished_mx(){
    switch(m_dns_mx->error()){
    case QDnsLookup::NotFoundError:
        break;
    case QDnsLookup::OperationCancelledError:
        log.message = "Operation Cancelled due to Timeout";
        log.target = m_dns_any->name();
        log.nameserver = m_dns_any->nameserver().toString();
        log.recordType = "MX";
        emit scanLog(log);
        break;
    case QDnsLookup::NoError:
        foreach(const QDnsMailExchangeRecord &record, m_dns_mx->mailExchangeRecords())
            m_result.MX.insert(record.exchange());
        has_record = true;
        break;
    default:
        log.message = m_dns_mx->errorString();
        log.target = m_dns_mx->name();
        log.nameserver = m_dns_mx->nameserver().toString();
        log.recordType = "MX";
        emit scanLog(log);
        break;
    }

    /* if this is the last lookup, save send results then go to next lookup */
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        if(has_record)
            emit scanResult(m_result);
        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit next();
    }
}

void dns::Scanner::lookupFinished_cname(){
    switch(m_dns_cname->error()){
    case QDnsLookup::NotFoundError:
        break;
    case QDnsLookup::OperationCancelledError:
        log.message = "Operation Cancelled due to Timeout";
        log.target = m_dns_any->name();
        log.nameserver = m_dns_any->nameserver().toString();
        log.recordType = "CNAME";
        emit scanLog(log);
        break;
    case QDnsLookup::NoError:
        foreach(const QDnsDomainNameRecord &record, m_dns_cname->canonicalNameRecords())
            m_result.CNAME.insert(record.value());
        has_record = true;
        break;
    default:
        log.message = m_dns_cname->errorString();
        log.target = m_dns_cname->name();
        log.nameserver = m_dns_cname->nameserver().toString();
        log.recordType = "CNAME";
        emit scanLog(log);
        break;
    }

    /* if this is the last lookup, save send results then go to next lookup */
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        if(has_record)
            emit scanResult(m_result);
        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit next();
    }
}

void dns::Scanner::lookupFinished_ns(){
    switch(m_dns_ns->error()){
    case QDnsLookup::NotFoundError:
        break;
    case QDnsLookup::OperationCancelledError:
        log.message = "Operation Cancelled due to Timeout";
        log.target = m_dns_any->name();
        log.nameserver = m_dns_any->nameserver().toString();
        log.recordType = "NS";
        emit scanLog(log);
        break;
    case QDnsLookup::NoError:
        foreach(const QDnsDomainNameRecord &record, m_dns_ns->nameServerRecords())
            m_result.NS.insert(record.value());
        has_record = true;
        break;
    default:
        log.message = m_dns_ns->errorString();
        log.target = m_dns_ns->name();
        log.nameserver = m_dns_ns->nameserver().toString();
        log.recordType = "NS";
        emit scanLog(log);
        break;
    }

    /* if this is the last lookup, save send results then go to next lookup */
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        if(has_record)
            emit scanResult(m_result);
        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit next();
    }
}

void dns::Scanner::lookupFinished_txt(){
    switch(m_dns_txt->error()){
    case QDnsLookup::NotFoundError:
        break;
    case QDnsLookup::OperationCancelledError:
        log.message = "Operation Cancelled due to Timeout";
        log.target = m_dns_any->name();
        log.nameserver = m_dns_any->nameserver().toString();
        log.recordType = "TXT";
        emit scanLog(log);
        break;
    case QDnsLookup::NoError:
        foreach(const QDnsTextRecord &record, m_dns_txt->textRecords()){
            foreach(const QByteArray &txt, record.values())
                m_result.TXT.insert(txt);
        }
        has_record = true;
        break;
    default:
        log.message = m_dns_txt->errorString();
        log.target = m_dns_txt->name();
        log.nameserver = m_dns_txt->nameserver().toString();
        log.recordType = "TXT";
        emit scanLog(log);
        break;
    }

    /* if this is the last lookup, save send results then go to next lookup */
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        if(has_record)
            emit scanResult(m_result);
        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit next();
    }
}

void dns::Scanner::lookupFinished_any(){
    switch(m_dns_any->error()){
    case QDnsLookup::NotFoundError:
        break;
    case QDnsLookup::OperationCancelledError:
        log.message = "Operation Cancelled due to Timeout";
        log.target = m_dns_any->name();
        log.nameserver = m_dns_any->nameserver().toString();
        log.recordType = "ANY";
        emit scanLog(log);
        break;
    case QDnsLookup::NoError:
        foreach(const QDnsHostAddressRecord &record, m_dns_any->hostAddressRecords()){
            if(record.value().protocol() == QAbstractSocket::IPv4Protocol)
                m_result.A.insert(record.value().toString());
            if(record.value().protocol() == QAbstractSocket::IPv6Protocol)
                m_result.AAAA.insert(record.value().toString());
        }
        foreach(const QDnsTextRecord &record, m_dns_any->textRecords()){
            foreach(const QByteArray &txt, record.values())
                m_result.TXT.insert(txt);
        }
        foreach(const QDnsDomainNameRecord &record, m_dns_any->nameServerRecords())
            m_result.NS.insert(record.value());
        foreach(const QDnsMailExchangeRecord &record, m_dns_any->mailExchangeRecords())
            m_result.MX.insert(record.exchange());
        foreach(const QDnsDomainNameRecord &record, m_dns_any->canonicalNameRecords())
            m_result.CNAME.insert(record.value());

        emit scanResult(m_result);
        break;
    default:
        log.message = m_dns_any->errorString();
        log.target = m_dns_any->name();
        log.nameserver = m_dns_any->nameserver().toString();
        log.recordType = "ANY";
        emit scanLog(log);
        break;
    }

    m_args->progress++;
    emit scanProgress(m_args->progress);
    emit next();
}

void dns::Scanner::lookupFinished_srv(){
    switch(m_dns_srv->error()){
    case QDnsLookup::NotFoundError:
        break;
    case QDnsLookup::OperationCancelledError:
        log.message = "Operation Cancelled due to Timeout";
        log.target = m_dns_any->name();
        log.nameserver = m_dns_any->nameserver().toString();
        log.recordType = "SRV";
        emit scanLog(log);
        break;
    case QDnsLookup::NoError:
    {
        s3s_struct::DNS dns;
        dns.dns = m_args->currentTarget;
        foreach(const QDnsServiceRecord &record, m_dns_srv->serviceRecords())
            dns.SRV.insert({record.name(), record.target(), QString::number(record.port())});
        emit scanResult(dns);
    }
        break;
    default:
        log.message = m_dns_srv->errorString();
        log.target = m_dns_srv->name();
        log.nameserver = m_dns_srv->nameserver().toString();
        log.recordType = "SRV";
        emit scanLog(log);
        break;
    }

    m_args->progress++;
    emit scanProgress(m_args->progress);
    emit next();
}

void dns::Scanner::lookup(){
    if(m_args->RecordType_srv){
        switch(dns::getTarget_srv(m_dns_srv, m_args)){
        case RETVAL::LOOKUP:
            m_dns_srv->lookup();
            s3s_LookupTimeout::set(m_dns_srv, m_args->config->timeout);
            break;
        case RETVAL::NEXT:
            emit next();
            break;
        case RETVAL::QUIT:
            emit quitThread();
            break;
        default:
            break;
        }
    }
    else {
        has_record = false;
        m_activeLookups = 0;
        m_currentTarget = dns::getTarget(m_args);

        m_result.dns = m_currentTarget;
        m_result.A.clear();
        m_result.AAAA.clear();
        m_result.MX.clear();
        m_result.NS.clear();
        m_result.TXT.clear();
        m_result.CNAME.clear();
        m_result.SRV.clear();

        /* quit if target is null */
        if(m_currentTarget.isNull()){
            emit quitThread();
            return;
        }

        if(m_args->RecordType_a){
            m_activeLookups++;
            m_dns_a->setName(m_currentTarget);
            m_dns_a->lookup();
            s3s_LookupTimeout::set(m_dns_a, m_args->config->timeout);
        }
        if(m_args->RecordType_aaaa){
            m_activeLookups++;
            m_dns_aaaa->setName(m_currentTarget);
            m_dns_aaaa->lookup();
            s3s_LookupTimeout::set(m_dns_aaaa, m_args->config->timeout);
        }
        if(m_args->RecordType_ns){
            m_activeLookups++;
            m_dns_ns->setName(m_currentTarget);
            m_dns_ns->lookup();
            s3s_LookupTimeout::set(m_dns_ns, m_args->config->timeout);
        }
        if(m_args->RecordType_mx){
            m_activeLookups++;
            m_dns_mx->setName(m_currentTarget);
            m_dns_mx->lookup();
            s3s_LookupTimeout::set(m_dns_mx, m_args->config->timeout);
        }
        if(m_args->RecordType_cname){
            m_activeLookups++;
            m_dns_cname->setName(m_currentTarget);
            m_dns_cname->lookup();
            s3s_LookupTimeout::set(m_dns_cname, m_args->config->timeout);
        }
        if(m_args->RecordType_txt){
            m_activeLookups++;
            m_dns_txt->setName(m_currentTarget);
            m_dns_txt->lookup();
            s3s_LookupTimeout::set(m_dns_txt, m_args->config->timeout);
        }
        if(m_args->RecordType_any){
            m_dns_any->setName(m_currentTarget);
            m_dns_any->lookup();
            s3s_LookupTimeout::set(m_dns_any, m_args->config->timeout);
        }
    }
}

QString dns::getTarget(dns::ScanArgs *args){
    /* lock */
    QMutexLocker(&args->mutex);

    if(!args->targets.isEmpty())
        return args->targets.dequeue();
    else
        return nullptr;
}

RETVAL dns::getTarget_srv(QDnsLookup *dns, dns::ScanArgs *args){
    /* lock */
    QMutexLocker(&args->mutex);

    /* check if Reached end of the wordlist */
    if(args->currentSRV < args->srvWordlist.length())
    {
        /* append to target then set the name */
        dns->setName(args->srvWordlist.at(args->currentSRV)+"."+args->currentTarget);

        /* next wordlist */
        args->currentSRV++;
        return RETVAL::LOOKUP;
    }
    else{
        if(args->targets.isEmpty())
            return RETVAL::QUIT;
        else {
            args->currentTarget = args->targets.dequeue();
            args->currentSRV = 0;
            return RETVAL::NEXT;
        }
    }
}

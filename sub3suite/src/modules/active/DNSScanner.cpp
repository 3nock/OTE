/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include <QDnsLookup>
#include "DNSScanner.h"


/* TODO:
 *      not well implemented yet
 */
dns::Scanner::Scanner(dns::ScanArgs *args): AbstractScanner (nullptr),
      m_args(args),
      m_dns_a(new QDnsLookup(this)),
      m_dns_aaaa(new QDnsLookup(this)),
      m_dns_mx(new QDnsLookup(this)),
      m_dns_ns(new QDnsLookup(this)),
      m_dns_txt(new QDnsLookup(this)),
      m_dns_cname(new QDnsLookup(this)),
      m_dns_srv(new QDnsLookup(this))
{
    connect(this, &dns::Scanner::next, this, &dns::Scanner::lookup);

    m_dns_a->setType(QDnsLookup::A);
    m_dns_aaaa->setType(QDnsLookup::AAAA);
    m_dns_mx->setType(QDnsLookup::MX);
    m_dns_ns->setType(QDnsLookup::NS);
    m_dns_txt->setType(QDnsLookup::TXT);
    m_dns_cname->setType(QDnsLookup::CNAME);
    m_dns_srv->setType(QDnsLookup::SRV);

    m_dns_a->setNameserver(QHostAddress(m_args->config->nameservers.at(0)));
    m_dns_a->setNameserver(QHostAddress(m_args->config->nameservers.at(0)));
    m_dns_mx->setNameserver(QHostAddress(m_args->config->nameservers.at(0)));
    m_dns_ns->setNameserver(QHostAddress(m_args->config->nameservers.at(0)));
    m_dns_txt->setNameserver(QHostAddress(m_args->config->nameservers.at(0)));
    m_dns_cname->setNameserver(QHostAddress(m_args->config->nameservers.at(0)));
    m_dns_srv->setNameserver(QHostAddress(m_args->config->nameservers.at(0)));

    connect(m_dns_srv, &QDnsLookup::finished, this, &dns::Scanner::srvLookupFinished);
    connect(m_dns_a, &QDnsLookup::finished, this, &dns::Scanner::aLookupFinished);
    connect(m_dns_aaaa, &QDnsLookup::finished, this, &dns::Scanner::aaaaLookupFinished);
    connect(m_dns_mx, &QDnsLookup::finished, this, &dns::Scanner::mxLookupFinished);
    connect(m_dns_ns, &QDnsLookup::finished, this, &dns::Scanner::nsLookupFinished);
    connect(m_dns_txt, &QDnsLookup::finished, this, &dns::Scanner::txtLookupFinished);
    connect(m_dns_cname, &QDnsLookup::finished, this, &dns::Scanner::cnameLookupFinished);
}
dns::Scanner::~Scanner(){
    delete m_dns_srv;
    delete m_dns_cname;
    delete m_dns_txt;
    delete m_dns_ns;
    delete m_dns_mx;
    delete m_dns_aaaa;
    delete m_dns_a;
}

void dns::Scanner::srvLookupFinished(){
    /* get records results */
    switch(m_dns_srv->error()){
    case QDnsLookup::NotFoundError:
        break;

    case QDnsLookup::NoError:
        foreach(const QDnsServiceRecord &record, m_dns_srv->serviceRecords())
            m_results.SRV.insert(record.target(), record.port());
        break;

    default:
        log.message = m_dns_srv->errorString();
        log.target = m_dns_srv->name();
        log.nameserver = m_dns_srv->nameserver().toString();
        log.recordType = "SRV";
        emit scanLog(log);
        break;
    }

    /* if this is the last lookup, save send results then go to next lookup */
    if(m_currentSrvWordlist == m_args->srvWordlist.count()){
        m_activeLookups--;
        if(!m_activeLookups){
            m_args->progress++;
            emit scanProgress(m_args->progress);
            emit scanResult(m_results);
            emit next();
        }
    }
}

void dns::Scanner::aLookupFinished(){
    /* get records results */
    switch(m_dns_a->error()){
    case QDnsLookup::NotFoundError:
        break;

    case QDnsLookup::NoError:
        foreach(const QDnsHostAddressRecord &record, m_dns_a->hostAddressRecords())
            m_results.A.append(record.value().toString());
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
    if(!m_activeLookups){
        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit scanResult(m_results);
        emit next();
    }
}

void dns::Scanner::aaaaLookupFinished(){
    /* get records results */
    switch(m_dns_aaaa->error()){
    case QDnsLookup::NotFoundError:
        break;

    case QDnsLookup::NoError:
        foreach(const QDnsHostAddressRecord &record, m_dns_aaaa->hostAddressRecords())
            m_results.AAAA.append(record.value().toString());
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
    if(!m_activeLookups){
        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit scanResult(m_results);
        emit next();
    }
}

void dns::Scanner::mxLookupFinished(){
    /* get records results */
    switch(m_dns_mx->error()){
    case QDnsLookup::NotFoundError:
        break;

    case QDnsLookup::NoError:
        foreach(const QDnsMailExchangeRecord &record, m_dns_mx->mailExchangeRecords())
            m_results.MX.append(record.exchange());
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
    if(!m_activeLookups){
        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit scanResult(m_results);
        emit next();
    }
}

void dns::Scanner::cnameLookupFinished(){
    /* get records results */
    switch(m_dns_cname->error()){
    case QDnsLookup::NotFoundError:
        break;

    case QDnsLookup::NoError:
        foreach(const QDnsDomainNameRecord &record, m_dns_cname->canonicalNameRecords())
            m_results.CNAME.append(record.value());
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
    if(!m_activeLookups){
        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit scanResult(m_results);
        emit next();
    }
}

void dns::Scanner::nsLookupFinished(){
    /* get records results */
    switch(m_dns_ns->error()){
    case QDnsLookup::NotFoundError:
        break;

    case QDnsLookup::NoError:
        foreach(const QDnsDomainNameRecord &record, m_dns_ns->nameServerRecords())
            m_results.NS.append(record.value());
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
    if(!m_activeLookups){
        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit scanResult(m_results);
        emit next();
    }
}

void dns::Scanner::txtLookupFinished(){
    /* get records results */
    switch(m_dns_txt->error()){
    case QDnsLookup::NotFoundError:
        break;

    case QDnsLookup::NoError:
        foreach(const QDnsTextRecord &record, m_dns_txt->textRecords()){
            foreach(const QByteArray &txt, record.values())
                m_results.TXT.append(txt);
        }
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
    if(!m_activeLookups){
        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit scanResult(m_results);
        emit next();
    }
}

void dns::Scanner::lookup(){
    m_activeLookups = 0;
    m_currentTarget = dns::getTarget(m_args);

    /* quit if target is null */
    if(m_currentTarget.isNull()){
        emit quitThread();
        return;
    }

    m_results.target = m_currentTarget;

    if(m_args->RecordType_a){
        m_activeLookups++;
        m_dns_a->setName(m_currentTarget);
        m_dns_a->lookup();
    }
    if(m_args->RecordType_aaaa){
        m_activeLookups++;
        m_dns_aaaa->setName(m_currentTarget);
        m_dns_aaaa->lookup();
    }
    if(m_args->RecordType_ns){
        m_activeLookups++;
        m_dns_ns->setName(m_currentTarget);
        m_dns_ns->lookup();
    }
    if(m_args->RecordType_mx){
        m_activeLookups++;
        m_dns_mx->setName(m_currentTarget);
        m_dns_mx->lookup();
    }
    if(m_args->RecordType_cname){
        m_activeLookups++;
        m_dns_cname->setName(m_currentTarget);
        m_dns_cname->lookup();
    }
    if(m_args->RecordType_txt){
        m_activeLookups++;
        m_dns_txt->setName(m_currentTarget);
        m_dns_txt->lookup();
    }
    if(m_args->RecordType_srv){
        m_activeLookups++;
        m_dns_srv->setName(m_currentTarget);
        m_dns_srv->lookup();
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

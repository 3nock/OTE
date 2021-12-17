#include "DnsRecordsScanner.h"
#include <QDnsLookup>


records::Scanner::Scanner(records::ScanArgs *args)
    : m_args(args),
      m_dns_srv(new QDnsLookup(this)),
      m_dns_a(new QDnsLookup(this)),
      m_dns_aaaa(new QDnsLookup(this)),
      m_dns_mx(new QDnsLookup(this)),
      m_dns_ns(new QDnsLookup(this)),
      m_dns_txt(new QDnsLookup(this)),
      m_dns_cname(new QDnsLookup(this))
{
    connect(this, SIGNAL(doLookup()), this, SLOT(lookup()));
    connect(this, SIGNAL(doLookup_srv()), this, SLOT(lookup_srv()));

    m_dns_srv->setType(QDnsLookup::SRV);
    m_dns_a->setType(QDnsLookup::A);
    m_dns_aaaa->setType(QDnsLookup::AAAA);
    m_dns_mx->setType(QDnsLookup::MX);
    m_dns_ns->setType(QDnsLookup::NS);
    m_dns_txt->setType(QDnsLookup::TXT);
    m_dns_cname->setType(QDnsLookup::CNAME);

    m_dns_srv->setNameserver(RandomNameserver(m_args->config->useCustomNameServers));
    m_dns_a->setNameserver(RandomNameserver(m_args->config->useCustomNameServers));
    m_dns_a->setNameserver(RandomNameserver(m_args->config->useCustomNameServers));
    m_dns_mx->setNameserver(RandomNameserver(m_args->config->useCustomNameServers));
    m_dns_ns->setNameserver(RandomNameserver(m_args->config->useCustomNameServers));
    m_dns_txt->setNameserver(RandomNameserver(m_args->config->useCustomNameServers));
    m_dns_cname->setNameserver(RandomNameserver(m_args->config->useCustomNameServers));

    connect(m_dns_srv, SIGNAL(finished()), this, SLOT(srvLookupFinished()));
    connect(m_dns_a, SIGNAL(finished()), this, SLOT(aLookupFinished()));
    connect(m_dns_aaaa, SIGNAL(finished()), this, SLOT(aaaaLookupFinished()));
    connect(m_dns_mx, SIGNAL(finished()), this, SLOT(mxLookupFinished()));
    connect(m_dns_ns, SIGNAL(finished()), this, SLOT(nsLookupFinished()));
    connect(m_dns_txt, SIGNAL(finished()), this, SLOT(txtLookupFinished()));
    connect(m_dns_cname, SIGNAL(finished()), this, SLOT(cnameLookupFinished()));
}
records::Scanner::~Scanner(){
    delete m_dns_srv;
    delete m_dns_a;
    delete m_dns_aaaa;
    delete m_dns_mx;
    delete m_dns_ns;
    delete m_dns_txt;
    delete m_dns_cname;
}

void records::Scanner::startScan_srv(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup_srv()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void records::Scanner::lookup(){
    /* ... */
    m_currentTargetToEnumerate = m_args->currentTargetToEnumerate;
    m_args->currentTargetToEnumerate++;

    /* ... */
    if(m_currentTargetToEnumerate < m_args->targetList.count())
    {
        m_currentTarget = m_args->targetList.at(m_currentTargetToEnumerate);
        m_results.domain = m_currentTarget;
        hasAtleastOneRecord = false;

        if(m_args->RecordType_a)
        {
            m_results.A.clear();

            m_dns_a->setName(m_currentTarget);
            m_dns_a->lookup();
            m_activeLookups++;
        }
        if(m_args->RecordType_aaaa)
        {
            m_results.AAAA.clear();

            m_dns_aaaa->setName(m_currentTarget);
            m_dns_aaaa->lookup();
            m_activeLookups++;
        }
        if(m_args->RecordType_mx)
        {
            m_results.MX.clear();

            m_dns_mx->setName(m_currentTarget);
            m_dns_mx->lookup();
            m_activeLookups++;
        }
        if(m_args->RecordType_ns)
        {
            m_results.NS.clear();

            m_dns_ns->setName(m_currentTarget);
            m_dns_ns->lookup();
            m_activeLookups++;
        }
        if(m_args->RecordType_txt)
        {
            m_results.TXT.clear();

            m_dns_txt->setName(m_currentTarget);
            m_dns_txt->lookup();
            m_activeLookups++;
        }
        if(m_args->RecordType_cname)
        {
            m_results.CNAME.clear();

            m_dns_cname->setName(m_currentTarget);
            m_dns_cname->lookup();
            m_activeLookups++;
        }
    }
    else
        /* No More wordlist to enumerate, enumeration Complete... */
        emit quitThread();
}

void records::Scanner::lookup_srv(){
    /* ... */
    m_currentSrvToEnumerate = m_args->currentSrvToEnumerate;
    m_currentTargetToEnumerate = m_args->currentTargetToEnumerate;
    m_args->currentSrvToEnumerate++;

    /* ... */
    if(m_currentSrvToEnumerate < m_args->srvWordlist.count())
    {
        m_currentTarget = m_args->srvWordlist.at(m_currentSrvToEnumerate)+"."+m_args->targetList.at(m_currentTargetToEnumerate);
        m_results.domain = m_args->targetList.at(m_currentTargetToEnumerate);
        m_dns_srv->setName(m_currentTarget);
        m_dns_srv->lookup();
    }
    /* reached end of the wordlist... */
    else
    {
        if(m_args->currentTargetToEnumerate < m_args->targetList.count()-1)
        {
            m_args->currentTargetToEnumerate++;
            m_args->currentSrvToEnumerate = 0;
            emit doLookup_srv();
        }
        else
            emit quitThread();
    }
}

void records::Scanner::srvLookupFinished(){
    if(m_dns_srv->error() == QDnsLookup::NoError)
    {
        const QList<QDnsServiceRecord> records = m_dns_srv->serviceRecords();
        if(records.count())
        {
            for(const QDnsServiceRecord &record : records){
                m_results.srvName = record.name();
                m_results.srvTarget = record.target();
                m_results.srvPort = record.port();
                emit scanResult(m_results);
            }
        }
    }

    m_args->progress++;
    emit scanProgress(m_args->progress);
    emit doLookup_srv();
}

void records::Scanner::finish(){
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        if(hasAtleastOneRecord)
            emit scanResult(m_results);

        m_args->progress++;
        emit scanProgress(m_args->progress);
        emit doLookup();
    }
}

void records::Scanner::aLookupFinished(){
    if(m_dns_a->error() == QDnsLookup::NoError)
    {
        const auto records = m_dns_a->hostAddressRecords();
        if(records.count())
        {
            for(const QDnsHostAddressRecord &record : records)
                m_results.A.append(record.value().toString());
            hasAtleastOneRecord = true;
        }
    }
    finish();
}

void records::Scanner::aaaaLookupFinished(){
    if(m_dns_aaaa->error() == QDnsLookup::NoError)
    {
        const QList<QDnsHostAddressRecord> records = m_dns_aaaa->hostAddressRecords();
        if(records.count())
        {
            for(const QDnsHostAddressRecord &record : records)
                m_results.AAAA.append(record.value().toString());
            hasAtleastOneRecord = true;
        }
    }
    finish();
}

void records::Scanner::mxLookupFinished(){
    if(m_dns_mx->error() == QDnsLookup::NoError)
    {
        const QList<QDnsMailExchangeRecord> records = m_dns_mx->mailExchangeRecords();
        if(records.count())
        {
            for(const QDnsMailExchangeRecord &record : records)
                m_results.MX.append(record.exchange());
            hasAtleastOneRecord = true;
        }
    }
    finish();
}

void records::Scanner::cnameLookupFinished(){
    if(m_dns_cname->error() == QDnsLookup::NoError)
    {
        const QList<QDnsDomainNameRecord> records = m_dns_cname->canonicalNameRecords();
        if(records.count())
        {
            for(const QDnsDomainNameRecord &record : records)
                m_results.CNAME.append(record.value());
            hasAtleastOneRecord = true;
        }
    }
    finish();
}

void records::Scanner::nsLookupFinished(){
    if(m_dns_ns->error() == QDnsLookup::NoError)
    {
        const QList<QDnsDomainNameRecord> records = m_dns_ns->nameServerRecords();
        if(records.count())
        {
            for(const QDnsDomainNameRecord &record : records)
                m_results.NS.append(record.value());
            hasAtleastOneRecord = true;
        }
    }
    finish();
}

void records::Scanner::txtLookupFinished(){
    if(m_dns_txt->error() == QDnsLookup::NoError)
    {
        const QList<QDnsTextRecord> records = m_dns_txt->textRecords();
        if(records.count())
        {
            for(const QDnsTextRecord &record : records)
            {
                for(int i = 0; i != record.values().size(); i++){
                    QString value(record.values()[i]);
                    m_results.TXT.append(value);
                }
            }
            hasAtleastOneRecord = true;
        }
    }
    finish();
}

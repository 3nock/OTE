#include "DnsRecordsScanner.h"

/********************************************************************************
                            DNS-RECORDS ENUMERATOR
*********************************************************************************/

records::Scanner::Scanner(ScanConfig *scanConfig, records::ScanArguments *scanArguments, records::ScanResults *scanResults)
    : m_scanConfig(scanConfig),
      m_scanArguments(scanArguments),
      m_scanResults(scanResults),
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
    //...
    m_dns_srv->setType(QDnsLookup::SRV);
    m_dns_a->setType(QDnsLookup::A);
    m_dns_aaaa->setType(QDnsLookup::AAAA);
    m_dns_mx->setType(QDnsLookup::MX);
    m_dns_ns->setType(QDnsLookup::NS);
    m_dns_txt->setType(QDnsLookup::TXT);
    m_dns_cname->setType(QDnsLookup::CNAME);
    //...
    m_dns_srv->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));
    m_dns_a->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));
    m_dns_a->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));
    m_dns_mx->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));
    m_dns_ns->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));
    m_dns_txt->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));
    m_dns_cname->setNameserver(RandomNameserver(m_scanConfig->useCustomNameServers));
    //...
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

void records::Scanner::startScan(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void records::Scanner::startScan_srv(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup_srv()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void records::Scanner::lookup(){
    //...
    m_currentTargetToEnumerate = m_scanArguments->currentTargetToEnumerate;
    m_scanArguments->currentTargetToEnumerate++;
    //...
    if(m_currentTargetToEnumerate < m_scanArguments->targetList->count())
    {
        m_currentTarget = m_scanArguments->targetList->item(m_currentTargetToEnumerate)->text();
        if(m_scanArguments->RecordType_a)
        {
            m_dns_a->setName(m_currentTarget);
            m_dns_a->lookup();
            m_activeLookups++;
        }
        if(m_scanArguments->RecordType_aaaa)
        {
            m_dns_aaaa->setName(m_currentTarget);
            m_dns_aaaa->lookup();
            m_activeLookups++;
        }
        if(m_scanArguments->RecordType_mx)
        {
            m_dns_mx->setName(m_currentTarget);
            m_dns_mx->lookup();
            m_activeLookups++;
        }
        if(m_scanArguments->RecordType_ns)
        {
            m_dns_ns->setName(m_currentTarget);
            m_dns_ns->lookup();
            m_activeLookups++;
        }
        if(m_scanArguments->RecordType_txt)
        {
            m_dns_txt->setName(m_currentTarget);
            m_dns_txt->lookup();
            m_activeLookups++;
        }
        if(m_scanArguments->RecordType_cname)
        {
            m_dns_cname->setName(m_currentTarget);
            m_dns_cname->lookup();
            m_activeLookups++;
        }
        m_firstToResolve = true;
    }
    else
    {
        ///
        /// No More wordlist to enumerate, enumeration Complete...
        ///
        emit quitThread();
    }
}

void records::Scanner::lookup_srv(){
    //...
    m_currentSrvToEnumerate = m_scanArguments->currentSrvToEnumerate;
    m_currentTargetToEnumerate = m_scanArguments->currentTargetToEnumerate;
    m_scanArguments->currentSrvToEnumerate++;
    //...
    if(m_currentSrvToEnumerate < m_scanArguments->srvWordlist->count())
    {
        m_currentTarget = m_scanArguments->srvWordlist->item(m_currentSrvToEnumerate)->text()+"."+m_scanArguments->targetList->item(m_currentTargetToEnumerate)->text();
        m_dns_srv->setName(m_currentTarget);
        m_dns_srv->lookup();
    }
    ///
    /// reached end of the wordlist...
    ///
    else
    {
        if(m_scanArguments->currentTargetToEnumerate < m_scanArguments->targetList->count()-1)
        {
            m_scanArguments->currentTargetToEnumerate++;
            m_scanArguments->currentSrvToEnumerate = 0;
            emit doLookup_srv();
        }
        else
        {
            emit quitThread();
        }
    }
}

void records::Scanner::stopScan(){
    emit quitThread();
}

/******************************* RECORD-TYPES LOOKUP FINISHED *********************************/

void records::Scanner::srvLookupFinished(){
    if(m_dns_srv->error() == QDnsLookup::NoError)
    {
        const auto records = m_dns_srv->serviceRecords();
        if(records.count())
        {
            for(const QDnsServiceRecord &record : records)
            {
                ///
                /// save to srv model...
                ///
                m_scanResults->resultsModel->records->model_srv->appendRow({new QStandardItem(record.name()), new QStandardItem(record.target()), new QStandardItem(QString::number(record.port()))});
                ///
                /// save to project model...
                ///
                m_scanResults->resultsModel->project->append(QList<QString>()<<record.name()<<record.target()<<m_dns_srv->name(), RESULTS::srv);
            }
            m_scanResults->srvResultsLabel->setNum(m_scanResults->resultsModel->records->model_srv->rowCount());
        }
    }
    ///
    /// scan progress...
    ///
    m_scanArguments->progress++;
    emit scanProgress(m_scanArguments->progress);
    //...
    emit doLookup_srv();
}

void records::Scanner::aLookupFinished(){
    if(m_dns_a->error() == QDnsLookup::NoError)
    {
        if(m_firstToResolve)
        {
            m_firstToResolve = false;
            //...
            m_dnsNameItem = new QStandardItem(m_scanArguments->targetList->item(m_currentTargetToEnumerate)->text());
            m_dnsNameItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
            m_scanResults->resultsModel->records->rootItem->appendRow(m_dnsNameItem);
            m_scanResults->resultsCountLabel->setNum(m_scanResults->resultsModel->records->rootItem->rowCount());
        }
        const auto records = m_dns_a->hostAddressRecords();
        if(records.count())
        {
            m_recordItem = new QStandardItem("A");
            m_recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
            for(const QDnsHostAddressRecord &record : records){
                m_recordItem->appendRow(new QStandardItem(record.value().toString()));
                ///
                /// save to project model...
                ///
                m_scanResults->resultsModel->project->append(QList<QString>()<<record.value().toString()<<m_dns_a->name(), RESULTS::a);
            }
            m_dnsNameItem->appendRow(m_recordItem);
        }
    }
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        ///
        /// scan progress...
        ///
        m_scanArguments->progress++;
        emit scanProgress(m_scanArguments->progress);
        //...
        emit doLookup();
    }
}

void records::Scanner::aaaaLookupFinished(){
    if(m_dns_aaaa->error() == QDnsLookup::NoError)
    {
        if(m_firstToResolve)
        {
            m_firstToResolve = false;
            //...
            m_dnsNameItem = new QStandardItem(m_scanArguments->targetList->item(m_currentTargetToEnumerate)->text());
            m_dnsNameItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
            m_scanResults->resultsModel->records->rootItem->appendRow(m_dnsNameItem);
            m_scanResults->resultsCountLabel->setNum(m_scanResults->resultsModel->records->rootItem->rowCount());
        }
        const auto records = m_dns_aaaa->hostAddressRecords();
        if(records.count())
        {
            m_recordItem = new QStandardItem("AAAA");
            m_recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
            for(const QDnsHostAddressRecord &record : records) {
                m_recordItem->appendRow(new QStandardItem(record.value().toString()));
                ///
                /// save to project model...
                ///
                m_scanResults->resultsModel->project->append(QList<QString>()<<record.value().toString()<<m_dns_aaaa->name(), RESULTS::aaaa);
            }
            m_dnsNameItem->appendRow(m_recordItem);
        }
    }
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        ///
        /// scan progress...
        ///
        m_scanArguments->progress++;
        emit scanProgress(m_scanArguments->progress);
        //...
        emit doLookup();
    }
}

void records::Scanner::mxLookupFinished(){
    if(m_dns_mx->error() == QDnsLookup::NoError)
    {
        if(m_firstToResolve)
        {
            m_firstToResolve = false;
            //...
            m_dnsNameItem = new QStandardItem(m_scanArguments->targetList->item(m_currentTargetToEnumerate)->text());
            m_dnsNameItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
            m_scanResults->resultsModel->records->rootItem->appendRow(m_dnsNameItem);
            m_scanResults->resultsCountLabel->setNum(m_scanResults->resultsModel->records->rootItem->rowCount());
        }
        const auto records = m_dns_mx->mailExchangeRecords();
        if(records.count())
        {
            m_recordItem = new QStandardItem("MX");
            m_recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
            for(const QDnsMailExchangeRecord &record : records) {
                m_recordItem->appendRow(new QStandardItem(record.exchange()));
                ///
                /// save to project model...
                ///
                m_scanResults->resultsModel->project->append(QList<QString>()<<record.exchange()<<m_dns_mx->name(), RESULTS::mx);
            }
            m_dnsNameItem->appendRow(m_recordItem);
        }
    }
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        ///
        /// scan progress...
        ///
        m_scanArguments->progress++;
        emit scanProgress(m_scanArguments->progress);
        //...
        emit doLookup();
    }
}

void records::Scanner::cnameLookupFinished()
{
    if(m_dns_cname->error() == QDnsLookup::NoError)
    {
        if(m_firstToResolve)
        {
            m_firstToResolve = false;
            //...
            m_dnsNameItem = new QStandardItem(m_scanArguments->targetList->item(m_currentTargetToEnumerate)->text());
            m_dnsNameItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
            m_scanResults->resultsModel->records->rootItem->appendRow(m_dnsNameItem);
            m_scanResults->resultsCountLabel->setNum(m_scanResults->resultsModel->records->rootItem->rowCount());
        }
        const auto records = m_dns_cname->canonicalNameRecords();
        if(records.count())
        {
            m_recordItem = new QStandardItem("CNAME");
            m_recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
            for(const QDnsDomainNameRecord &record : records) {
                m_recordItem->appendRow(new QStandardItem(record.value()));
                ///
                /// save to project model...
                ///
                m_scanResults->resultsModel->project->append(QList<QString>()<<record.value()<<m_dns_cname->name(), RESULTS::cname);
            }
            m_dnsNameItem->appendRow(m_recordItem);
        }
    }
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        ///
        /// scan progress...
        ///
        m_scanArguments->progress++;
        emit scanProgress(m_scanArguments->progress);
        //...
        emit doLookup();
    }
}

void records::Scanner::nsLookupFinished(){
    if(m_dns_ns->error() == QDnsLookup::NoError)
    {
        if(m_firstToResolve)
        {
            m_firstToResolve = false;
            //...
            m_dnsNameItem = new QStandardItem(m_scanArguments->targetList->item(m_currentTargetToEnumerate)->text());
            m_dnsNameItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
            m_scanResults->resultsModel->records->rootItem->appendRow(m_dnsNameItem);
            m_scanResults->resultsCountLabel->setNum(m_scanResults->resultsModel->records->rootItem->rowCount());
        }
        const auto records = m_dns_ns->nameServerRecords();
        if(records.count())
        {
            m_recordItem = new QStandardItem("NS");
            m_recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
            for(const QDnsDomainNameRecord &record : records) {
                m_recordItem->appendRow(new QStandardItem(record.value()));
                ///
                /// save to project model...
                ///
                m_scanResults->resultsModel->project->append(QList<QString>()<<record.value()<<m_dns_ns->name(), RESULTS::ns);
            }
            m_dnsNameItem->appendRow(m_recordItem);
        }
    }
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        ///
        /// scan progress...
        ///
        m_scanArguments->progress++;
        emit scanProgress(m_scanArguments->progress);
        //...
        emit doLookup();
    }
}

void records::Scanner::txtLookupFinished(){
    if(m_dns_txt->error() == QDnsLookup::NoError)
    {
        if(m_firstToResolve)
        {
            m_firstToResolve = false;
            //...
            m_dnsNameItem = new QStandardItem(m_scanArguments->targetList->item(m_currentTargetToEnumerate)->text());
            m_dnsNameItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
            m_scanResults->resultsModel->records->rootItem->appendRow(m_dnsNameItem);
            m_scanResults->resultsCountLabel->setNum(m_scanResults->resultsModel->records->rootItem->rowCount());
        }
        const auto records = m_dns_txt->textRecords();
        if(records.count())
        {
            m_recordItem = new QStandardItem("TXT");
            m_recordItem->setIcon(QIcon(":/img/res/icons/folder2.png"));
            for(const QDnsTextRecord &record : records)
            {
                for(int i = 0; i != record.values().size(); i++){
                    QString value(record.values()[i]);
                    m_recordItem->appendRow(new QStandardItem(value));
                    ///
                    /// save to project model...
                    ///
                    m_scanResults->resultsModel->project->append(QList<QString>()<<value<<m_dns_txt->name(), RESULTS::txt);
                }
            }
            m_dnsNameItem->appendRow(m_recordItem);
        }
    }
    m_activeLookups--;
    if(m_activeLookups == 0)
    {
        ///
        /// scan progress...
        ///
        m_scanArguments->progress++;
        emit scanProgress(m_scanArguments->progress);
        //...
        emit doLookup();
    }
}

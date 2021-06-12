#include "DnsEnumerator.h"

/********************************************************************************
                            DNS-RECORDS ENUMERATOR
*********************************************************************************/

///
/// \brief Enumerator_dnsRecords::Enumerator_dnsRecords
/// \param scanArguments
/// \param scanResults
/// TODO: fix unnecessary initialization of the QDnsLookup objects...
///
Enumerator_dnsRecords::Enumerator_dnsRecords(scanArguments_dnsRecords *scanArguments, scanResults_dnsRecords *scanResults)
    : m_scanArguments(scanArguments), m_scanResults(scanResults),
      m_nameserver(RandomNameserver(false)),
      m_dns_mx(new QDnsLookup(this)),
      m_dns_ns(new QDnsLookup(this)),
      m_dns_txt(new QDnsLookup(this)),
      m_dns_srv(new QDnsLookup(this)),
      m_dns_cname(new QDnsLookup(this))
{
    connect(this, SIGNAL(done()), this, SLOT(trackFinishedLookups()));
    connect(this, SIGNAL(doLookup()), this, SLOT(lookup()));
    //...
    m_dns_mx->setType(QDnsLookup::MX);
    m_dns_ns->setType(QDnsLookup::NS);
    m_dns_srv->setType(QDnsLookup::SRV);
    m_dns_txt->setType(QDnsLookup::TXT);
    m_dns_cname->setType(QDnsLookup::CNAME);
    //...
    m_dns_mx->setNameserver(m_nameserver);
    m_dns_ns->setNameserver(m_nameserver);
    m_dns_srv->setNameserver(m_nameserver);
    m_dns_txt->setNameserver(m_nameserver);
    m_dns_cname->setNameserver(m_nameserver);
    //...
    connect(m_dns_mx, SIGNAL(finished()), this, SLOT(mxLookupFinished()));
    connect(m_dns_ns, SIGNAL(finished()), this, SLOT(nsLookupFinished()));
    connect(m_dns_srv, SIGNAL(finished()), this, SLOT(srvLookupFinished()));
    connect(m_dns_txt, SIGNAL(finished()), this, SLOT(txtLookupFinished()));
    connect(m_dns_cname, SIGNAL(finished()), this, SLOT(cnameLookupFinished()));
}
Enumerator_dnsRecords::~Enumerator_dnsRecords(){
    delete m_dns_mx;
    delete m_dns_ns;
    delete m_dns_srv;
    delete m_dns_txt;
    delete m_dns_cname;
}

void Enumerator_dnsRecords::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void Enumerator_dnsRecords::trackFinishedLookups(){
    m_finishedLookups++;
    if(m_finishedLookups == m_scanArguments->choiceCount){
        emit doLookup();
    }
}

void Enumerator_dnsRecords::lookup(){
    //..
    m_currentTargetToEnumerate = m_scanArguments->currentTargetToEnumerate;
    m_scanArguments->currentTargetToEnumerate++;
    if(m_currentTargetToEnumerate < m_scanArguments->targetWordlist->count()){
        //...
        m_currentTarget = m_scanArguments->targetWordlist->item(m_currentTargetToEnumerate)->text();
        m_scanArguments->targetWordlist->item(m_currentTargetToEnumerate)->setForeground(Qt::gray);
        if(m_scanArguments->RecordType_mx){
            m_dns_mx->setName(m_currentTarget);
            m_dns_mx->lookup();
        }
        if(m_scanArguments->RecordType_ns){
            m_dns_ns->setName(m_currentTarget);
            m_dns_ns->lookup();
        }
        if(m_scanArguments->RecordType_srv){
            m_dns_srv->setName(m_currentTarget);
            m_dns_srv->lookup();
        }
        if(m_scanArguments->RecordType_txt){
            m_dns_txt->setName(m_currentTarget);
            m_dns_txt->lookup();
        }
        if(m_scanArguments->RecordType_cname){
            m_dns_cname->setName(m_currentTarget);
            m_dns_cname->lookup();
        }
        m_firstToResolve = true;
        m_finishedLookups = 0;
    }else{
        ///
        /// No More wordlist to enumerate, enumeration Complete...
        ///
        emit quitThread();
    }
}

void Enumerator_dnsRecords::onStop(){
    emit quitThread();
}

/******************************* RECORD-TYPES LOOKUP FINISHED *********************************/
void Enumerator_dnsRecords::mxLookupFinished(){
    if(m_dns_mx->error() == QDnsLookup::NoError){
        if(m_firstToResolve){
            m_firstToResolve = false;
            //...
            m_dnsNameItem = new QStandardItem(m_scanArguments->targetWordlist->item(m_currentTargetToEnumerate)->text());
            m_scanResults->root_item->appendRow(m_dnsNameItem);
            m_scanResults->resultsCount++;
            m_scanResults->resultsCountLabel->setNum(m_scanResults->resultsCount);
        }
        const auto records = m_dns_mx->mailExchangeRecords();
        if(records.count()){
            m_recordItem = new QStandardItem("MX");
            m_recordItem->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
            for(const QDnsMailExchangeRecord &record : records) {
                m_recordItem->appendRow(new QStandardItem(record.exchange()));
            }
            m_dnsNameItem->appendRow(m_recordItem);
        }
    }
    emit done();
}

void Enumerator_dnsRecords::cnameLookupFinished(){
    if(m_dns_cname->error() == QDnsLookup::NoError){
        if(m_firstToResolve){
            m_firstToResolve = false;
            //...
            m_dnsNameItem = new QStandardItem(m_scanArguments->targetWordlist->item(m_currentTargetToEnumerate)->text());
            m_scanResults->root_item->appendRow(m_dnsNameItem);
            m_scanResults->resultsCount++;
            m_scanResults->resultsCountLabel->setNum(m_scanResults->resultsCount);
        }
        const auto records = m_dns_cname->canonicalNameRecords();
        if(records.count()){
            m_recordItem = new QStandardItem("CNAME");
            m_recordItem->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
            for(const QDnsDomainNameRecord &record : records) {
                m_recordItem->appendRow(new QStandardItem(record.value()));
            }
            m_dnsNameItem->appendRow(m_recordItem);
        }
    }
    emit done();
}

void Enumerator_dnsRecords::nsLookupFinished(){
    if(m_dns_ns->error() == QDnsLookup::NoError){
        if(m_firstToResolve){
            m_firstToResolve = false;
            //...
            m_dnsNameItem = new QStandardItem(m_scanArguments->targetWordlist->item(m_currentTargetToEnumerate)->text());
            m_scanResults->root_item->appendRow(m_dnsNameItem);
            m_scanResults->resultsCount++;
            m_scanResults->resultsCountLabel->setNum(m_scanResults->resultsCount);
        }
        const auto records = m_dns_ns->nameServerRecords();
        if(records.count()){
            m_recordItem = new QStandardItem("NS");
            m_recordItem->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
            for(const QDnsDomainNameRecord &record : records) {
                m_recordItem->appendRow(new QStandardItem(record.value()));
            }
            m_dnsNameItem->appendRow(m_recordItem);
        }
    }
    emit done();
}

void Enumerator_dnsRecords::txtLookupFinished(){
    if(m_dns_txt->error() == QDnsLookup::NoError){
        if(m_firstToResolve){
            m_firstToResolve = false;
            //...
            m_dnsNameItem = new QStandardItem(m_scanArguments->targetWordlist->item(m_currentTargetToEnumerate)->text());
            m_scanResults->root_item->appendRow(m_dnsNameItem);
            m_scanResults->resultsCount++;
            m_scanResults->resultsCountLabel->setNum(m_scanResults->resultsCount);
        }
        const auto records = m_dns_txt->textRecords();
        if(records.count()){
            m_recordItem = new QStandardItem("TXT");
            m_recordItem->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
            for(const QDnsTextRecord &record : records) {
                for(int i = 0; i != record.values().size(); i++){
                    m_recordItem->appendRow(new QStandardItem(QString(record.values()[i])));
                }
            }
            m_dnsNameItem->appendRow(m_recordItem);
        }
    }
    emit done();
}

void Enumerator_dnsRecords::srvLookupFinished(){
    if(m_dns_srv->error() == QDnsLookup::NoError){
        if(m_firstToResolve){
            m_firstToResolve = false;
            //...
            m_dnsNameItem = new QStandardItem(m_scanArguments->targetWordlist->item(m_currentTargetToEnumerate)->text());
            m_scanResults->root_item->appendRow(m_dnsNameItem);
            m_scanResults->resultsCount++;
            m_scanResults->resultsCountLabel->setNum(m_scanResults->resultsCount);
        }
        const auto records = m_dns_srv->serviceRecords();
        if(records.count()){
            m_recordItem = new QStandardItem("SRV");
            m_recordItem->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
            for(const QDnsServiceRecord &record : records) {
                m_recordItem->appendRow(new QStandardItem(record.target()+" Weight: "+QString::number(record.weight())+" Port: "+QString::number(record.port())));
            }
            m_dnsNameItem->appendRow(m_recordItem);
        }
    }
    emit done();
}

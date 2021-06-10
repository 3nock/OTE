#include "DnsEnumerator.h"

/********************************************************************************
                            DNS-RECORDS ENUMERATOR
*********************************************************************************/

Enumerator_dnsRecords::Enumerator_dnsRecords(scanArguments_dnsRecords *_scanArguments, scanResults_dnsRecords *_scanResults){
    //nameserver = RandomNameserver(false);
    nameserver = QHostAddress("8.8.8.8");
    scanArguments = _scanArguments;
    scanResults = _scanResults;
    //...
    connect(this, SIGNAL(done()), this, SLOT(trackFinishedLookups()));
    connect(this, SIGNAL(doLookup()), this, SLOT(lookup()));
    //...
    dns_mx = new QDnsLookup(this);
    dns_ns = new QDnsLookup(this);
    dns_srv = new QDnsLookup(this);
    dns_txt = new QDnsLookup(this);
    dns_cname = new QDnsLookup(this);
    //...
    dns_mx->setType(QDnsLookup::MX);
    dns_ns->setType(QDnsLookup::NS);
    dns_srv->setType(QDnsLookup::SRV);
    dns_txt->setType(QDnsLookup::TXT);
    dns_cname->setType(QDnsLookup::CNAME);
    //...
    dns_mx->setNameserver(nameserver);
    dns_ns->setNameserver(nameserver);
    dns_srv->setNameserver(nameserver);
    dns_txt->setNameserver(nameserver);
    dns_cname->setNameserver(nameserver);
    //...
    connect(dns_mx, SIGNAL(finished()), this, SLOT(mxLookupFinished()));
    connect(dns_ns, SIGNAL(finished()), this, SLOT(nsLookupFinished()));
    connect(dns_srv, SIGNAL(finished()), this, SLOT(srvLookupFinished()));
    connect(dns_txt, SIGNAL(finished()), this, SLOT(txtLookupFinished()));
    connect(dns_cname, SIGNAL(finished()), this, SLOT(cnameLookupFinished()));
}
Enumerator_dnsRecords::~Enumerator_dnsRecords(){
    delete dns_mx;
    delete dns_ns;
    delete dns_srv;
    delete dns_txt;
    delete dns_cname;
}

void Enumerator_dnsRecords::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void Enumerator_dnsRecords::trackFinishedLookups(){
    finishedLookups++;
    if(finishedLookups == scanArguments->choiceCount){
        emit doLookup();
    }
}

void Enumerator_dnsRecords::lookup(){
    //..
    currentTargetToEnumerate = scanArguments->currentTargetToEnumerate;
    scanArguments->currentTargetToEnumerate++;
    if(currentTargetToEnumerate < scanArguments->targetWordlist->count()){
        firstToResolve = true;
        //...
        currentTarget = scanArguments->targetWordlist->item(currentTargetToEnumerate)->text();
        scanArguments->targetWordlist->item(currentTargetToEnumerate)->setForeground(Qt::gray);
        if(scanArguments->RecordType_mx){
            dns_mx->setName(currentTarget);
            dns_mx->lookup();
        }
        if(scanArguments->RecordType_ns){
            dns_ns->setName(currentTarget);
            dns_ns->lookup();
        }
        if(scanArguments->RecordType_srv){
            dns_srv->setName(currentTarget);
            dns_srv->lookup();
        }
        if(scanArguments->RecordType_txt){
            dns_txt->setName(currentTarget);
            dns_txt->lookup();
        }
        if(scanArguments->RecordType_cname){
            dns_cname->setName(currentTarget);
            dns_cname->lookup();
        }
        finishedLookups = 0;
    }else{
        // enumeration Complete...
        emit quitThread();
    }
}

void Enumerator_dnsRecords::onStop(){
    emit quitThread();
}

/******************************* RECORD-TYPES LOOKUP FINISHED *********************************/
void Enumerator_dnsRecords::mxLookupFinished(){
    if(dns_mx->error() == QDnsLookup::NoError){
        if(firstToResolve){
            firstToResolve = false;
            //...
            dnsNameItem = new QStandardItem(scanArguments->targetWordlist->item(currentTargetToEnumerate)->text());
            scanResults->root_item->appendRow(dnsNameItem);
            scanResults->resultsCount++;
            scanResults->resultsCountLabel->setNum(scanResults->resultsCount);
        }
        const auto records = dns_mx->mailExchangeRecords();
        if(records.count()){
            recordItem = new QStandardItem("MX");
            recordItem->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
            for(const QDnsMailExchangeRecord &record : records) {
                recordItem->appendRow(new QStandardItem(record.exchange()));
            }
            dnsNameItem->appendRow(recordItem);
        }
    }
    emit done();
}

void Enumerator_dnsRecords::cnameLookupFinished(){
    if(dns_cname->error() == QDnsLookup::NoError){
        if(firstToResolve){
            firstToResolve = false;
            //...
            dnsNameItem = new QStandardItem(scanArguments->targetWordlist->item(currentTargetToEnumerate)->text());
            scanResults->root_item->appendRow(dnsNameItem);
            scanResults->resultsCount++;
            scanResults->resultsCountLabel->setNum(scanResults->resultsCount);
        }
        const auto records = dns_cname->canonicalNameRecords();
        if(records.count()){
            recordItem = new QStandardItem("CNAME");
            recordItem->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
            for(const QDnsDomainNameRecord &record : records) {
                recordItem->appendRow(new QStandardItem(record.value()));
            }
            dnsNameItem->appendRow(recordItem);
        }
    }
    emit done();
}

void Enumerator_dnsRecords::nsLookupFinished(){
    if(dns_ns->error() == QDnsLookup::NoError){
        if(firstToResolve){
            firstToResolve = false;
            //...
            dnsNameItem = new QStandardItem(scanArguments->targetWordlist->item(currentTargetToEnumerate)->text());
            scanResults->root_item->appendRow(dnsNameItem);
            scanResults->resultsCount++;
            scanResults->resultsCountLabel->setNum(scanResults->resultsCount);
        }
        const auto records = dns_ns->nameServerRecords();
        if(records.count()){
            recordItem = new QStandardItem("NS");
            recordItem->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
            for(const QDnsDomainNameRecord &record : records) {
                recordItem->appendRow(new QStandardItem(record.value()));
            }
            dnsNameItem->appendRow(recordItem);
        }
    }
    emit done();
}

void Enumerator_dnsRecords::txtLookupFinished(){
    if(dns_txt->error() == QDnsLookup::NoError){
        if(firstToResolve){
            firstToResolve = false;
            //...
            dnsNameItem = new QStandardItem(scanArguments->targetWordlist->item(currentTargetToEnumerate)->text());
            scanResults->root_item->appendRow(dnsNameItem);
            scanResults->resultsCount++;
            scanResults->resultsCountLabel->setNum(scanResults->resultsCount);
        }
        const auto records = dns_txt->textRecords();
        if(records.count()){
            recordItem = new QStandardItem("TXT");
            recordItem->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
            for(const QDnsTextRecord &record : records) {
                for(int i = 0; i != record.values().size(); i++){
                    recordItem->appendRow(new QStandardItem(QString(record.values()[i])));
                }
            }
            dnsNameItem->appendRow(recordItem);
        }
    }
    emit done();
}

void Enumerator_dnsRecords::srvLookupFinished(){
    if(dns_srv->error() == QDnsLookup::NoError){
        if(firstToResolve){
            firstToResolve = false;
            //...
            dnsNameItem = new QStandardItem(scanArguments->targetWordlist->item(currentTargetToEnumerate)->text());
            scanResults->root_item->appendRow(dnsNameItem);
            scanResults->resultsCount++;
            scanResults->resultsCountLabel->setNum(scanResults->resultsCount);
        }
        const auto records = dns_srv->serviceRecords();
        if(records.count()){
            recordItem = new QStandardItem("SRV");
            recordItem->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
            for(const QDnsServiceRecord &record : records) {
                recordItem->appendRow(new QStandardItem(record.target()+" Weight: "+QString::number(record.weight())+" Port: "+QString::number(record.port())));
            }
            dnsNameItem->appendRow(recordItem);
        }
    }
    emit done();
}

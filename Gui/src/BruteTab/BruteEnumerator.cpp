#include "BruteEnumerator.h"

/********************************************************************************************
 *                              BRUTE-ENUMERATOR
 ********************************************************************************************/
Enumerator_subBrute::Enumerator_subBrute(ScanArguments_Brute *_scanArguments){
    scanArguments = _scanArguments;
    nameserver = RandomNameserver(scanArguments->useCustomNameServers);
    //...
    dns = new QDnsLookup(this);
    dns->setNameserver(nameserver);
    dns->setType(scanArguments->dnsRecordType);
    //...
    connect(dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
}
Enumerator_subBrute::~Enumerator_subBrute(){
    delete dns;
}

void Enumerator_subBrute::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

// When Lookup is finished...
void Enumerator_subBrute::onLookupFinished(){
    // target subdomain couldnt be resolved hence false...
    if(dns->error() == QDnsLookup::NotFoundError){
        goto Finish;
    }
    // subdomain was resolved...
    if(dns->error() == QDnsLookup::NoError){
        if(scanArguments->usesWildcards){
            if(!(dns->hostAddressRecords()[0].value().toString() == scanArguments->foundWildcardIp)){
                emit resolvedSubdomain(dns->name(), dns->hostAddressRecords()[0].value().toString());
            }
        }else{
            emit resolvedSubdomain(dns->name(), dns->hostAddressRecords()[0].value().toString());
        }
        goto Finish;
    }
    if(dns->error() == QDnsLookup::InvalidReplyError){
        emit scanLog("[*] InvalidReplyError! SUBDOMAIN: "+dns->name()+"  NAMESERVER: "+dns->nameserver().toString());
        goto Finish;
    }
    if(dns->error() == QDnsLookup::InvalidRequestError){
        emit scanLog("[*] InvalidRequestError! SUBDOMAIN: "+dns->name()+"  NAMESERVER: "+dns->nameserver().toString());
        goto Finish;
    }
    if(dns->error() == QDnsLookup::ResolverError){
        emit scanLog("[*] ResolverError! SUBDOMAIN: "+dns->name()+"  NAMESERVER: "+dns->nameserver().toString());
        goto Finish;
    }
Finish:
    lookup();
}

void Enumerator_subBrute::lookup(){
    itemToEnumerate = scanArguments->enumeratedWordlists;
    scanArguments->enumeratedWordlists++;
    if(itemToEnumerate < scanArguments->wordlist->count()){
        dns->setName(nameProcessor_subBrute(scanArguments->wordlist->item(itemToEnumerate)->text(), scanArguments->targetDomain));
        //...
        scanArguments->wordlist->item(itemToEnumerate)->setForeground(Qt::gray);
        //...
        dns->lookup();
    }else{
        // at the end of the wordlist, signal the thread to Quit...
        emit quitThread();
    }
}

// quiting all running threads upon receiving stop signal...
void Enumerator_subBrute::onStop(){
    emit quitThread();
}

/******************************************************************************************************
 *                                      TLD-BRUTE ENUMERATOR
 ******************************************************************************************************/
Enumerator_tldBrute::Enumerator_tldBrute(ScanArguments_Brute *_scanArguments){
    scanArguments = _scanArguments;
    nameserver = RandomNameserver(scanArguments->useCustomNameServers);
}

void Enumerator_tldBrute::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void Enumerator_tldBrute::onLookupFinished(){
    if(dns->error() == QDnsLookup::NotFoundError){
        goto Finish;
    }
    if(dns->error() == QDnsLookup::NoError){
        emit resolvedSubdomain(dns->name(), dns->hostAddressRecords()[0].value().toString());
        goto Finish;
    }
    if(dns->error() == QDnsLookup::InvalidReplyError){
        emit scanLog("[*] InvalidReplyError! SUBDOMAIN: "+dns->name()+"  NAMESERVER: "+dns->nameserver().toString());
        goto Finish;
    }
    if(dns->error() == QDnsLookup::InvalidRequestError){
        emit scanLog("[*] InvalidRequestError! SUBDOMAIN: "+dns->name()+"  NAMESERVER: "+dns->nameserver().toString());
        goto Finish;
    }
    if(dns->error() == QDnsLookup::ResolverError){
        emit scanLog("[*] ResolverError! SUBDOMAIN: "+dns->name()+"  NAMESERVER: "+dns->nameserver().toString());
        goto Finish;
    }
Finish:
    delete dns;
    lookup();
}

void Enumerator_tldBrute::lookup(){
    itemToEnumerate = scanArguments->enumeratedWordlists;
    scanArguments->enumeratedWordlists++;
    if(itemToEnumerate < scanArguments->wordlist->count()){
        dns = new QDnsLookup(this);
        dns->setType(scanArguments->dnsRecordType);
        dns->setNameserver(nameserver);
        dns->setName(nameProcessor_tldBrute(scanArguments->wordlist->item(itemToEnumerate)->text(), scanArguments->targetDomain));
        //...
        connect(dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
        scanArguments->wordlist->item(itemToEnumerate)->setForeground(Qt::gray);
        //...
        dns->lookup();
    }else{
        // at the end of the wordlist, signal the thread to Quit...
        emit quitThread();
    }
}

// quiting all running threads upon receiving stop signal...
void Enumerator_tldBrute::onStop(){
    emit quitThread();
}


/******************************************************************************************************
 *                                      ACTIVE_SUBDOMAINS ENUMERATOR
 ******************************************************************************************************/
Enumerator_activeSubdomains::Enumerator_activeSubdomains(ScanArguments_Brute *_scanArguments){
    scanArguments = _scanArguments;
    nameserver = RandomNameserver(scanArguments->useCustomNameServers);
}

void Enumerator_activeSubdomains::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void Enumerator_activeSubdomains::onLookupFinished(){
    if(dns->error() == QDnsLookup::NotFoundError){
        goto Finish;
    }
    if(dns->error() == QDnsLookup::NoError){
        emit resolvedSubdomain(dns->name(), dns->hostAddressRecords()[0].value().toString());
        goto Finish;
    }
    if(dns->error() == QDnsLookup::InvalidReplyError){
        emit scanLog("[*] InvalidReplyError! SUBDOMAIN: "+dns->name()+"  NAMESERVER: "+dns->nameserver().toString());
        goto Finish;
    }
    if(dns->error() == QDnsLookup::InvalidRequestError){
        emit scanLog("[*] InvalidRequestError! SUBDOMAIN: "+dns->name()+"  NAMESERVER: "+dns->nameserver().toString());
        goto Finish;
    }
    if(dns->error() == QDnsLookup::ResolverError){
        emit scanLog("[*] ResolverError! SUBDOMAIN: "+dns->name()+"  NAMESERVER: "+dns->nameserver().toString());
        goto Finish;
    }
Finish:
    delete dns;
    lookup();
}

void Enumerator_activeSubdomains::lookup(){
    itemToEnumerate = scanArguments->enumeratedWordlists;
    scanArguments->enumeratedWordlists++;
    if(itemToEnumerate < scanArguments->wordlist->count()){
        dns = new QDnsLookup(this);
        dns->setType(scanArguments->dnsRecordType);
        dns->setNameserver(nameserver);
        dns->setName(scanArguments->wordlist->item(itemToEnumerate)->text());
        //...
        connect(dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
        scanArguments->wordlist->item(itemToEnumerate)->setForeground(Qt::gray);
        //...
        dns->lookup();
    }else{
        // at the end of the wordlist, signal the thread to Quit...
        emit quitThread();
    }
}

// quiting all running threads upon receiving stop signal...
void Enumerator_activeSubdomains::onStop(){
    emit quitThread();
}



/******************************************************************************************************
 *                                      CHECK WILDCARDS
 ******************************************************************************************************/

Enumerator_Wildcards::Enumerator_Wildcards(ScanArguments_Brute *_scanArguments){
    scanArguments = _scanArguments;
}

void Enumerator_Wildcards::Enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void Enumerator_Wildcards::onLookupFinished(){
    if(dns->error() == QDnsLookup::NoError){
        scanArguments->usesWildcards = true;
        scanArguments->foundWildcardIp = dns->hostAddressRecords()[0].value().toString();
    }else{
        scanArguments->usesWildcards = false;
    }
    delete dns;
    emit quitThread();
}

void Enumerator_Wildcards::lookup(){
    dns = new QDnsLookup(this);
    dns->setName("3nocknicolas."+scanArguments->targetDomain);
    dns->setType(scanArguments->dnsRecordType);
    dns->setNameserver(RandomNameserver(scanArguments->useCustomNameServers));
    //...
    connect(dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
    //...
    dns->lookup();
}

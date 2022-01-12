/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "WildcardScanner.h"

WildcardScanner::WildcardScanner(brute::ScanArgs *args)
    : m_args(args),
      m_dns(new QDnsLookup(this))
{
    m_dns->setType(m_args->config->recordType);
    m_dns->setNameserver(QHostAddress(m_args->config->nameservers.at(0)));

    connect(m_dns, SIGNAL(finished()), this, SLOT(onLookupFinished()));
}
WildcardScanner::~WildcardScanner(){
    delete m_dns;
}

void WildcardScanner::enumerate(QThread *cThread){
    connect(cThread, SIGNAL(started()), this, SLOT(lookup()));
    connect(this, SIGNAL(quitThread()), cThread, SLOT(quit()));
}

void WildcardScanner::onLookupFinished(){
    switch(m_dns->error()){

    case QDnsLookup::NoError:
        //m_args->config->hasWildcard = true;
        //m_args->config->wildcardIp = m_dns->hostAddressRecords()[0].value().toString();
        break;

    case QDnsLookup::NotFoundError:
        //m_args->config->hasWildcard = false;
        break;

    default:
        break;
    }

    emit quitThread();
}

void WildcardScanner::lookup(){
    /*
     check for random non-existent subdomains name
     TODO: make it more advanced...
    */

    m_dns->lookup();
}

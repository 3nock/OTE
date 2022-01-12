/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "IpScanner.h"


ip::Scanner::Scanner(ip::ScanArgs *args)
    : m_args(args),
      hostInfo(new QHostInfo)
{
    connect(this, SIGNAL(anotherLookup()), this, SLOT(lookup()));
}
ip::Scanner::~Scanner(){
    delete hostInfo;
}

///
/// TODO: test the results if its is not simply an address using regular expression...
///
void ip::Scanner::lookupFinished(QHostInfo info){
    /*
     check the results of the lookup if no error occurred save the results
     if error occurred emit appropriate response...
    */
    if(info.error() ==  QHostInfo::NoError)
    {
        if(!info.hostName().isNull() && !info.addresses().isEmpty())
            emit scanResult(info.hostName(), info.addresses()[0].toString());
    }

    /* scan progress... */
    m_args->progress++;

    /* send results and continue scan */
    emit scanProgress(m_args->progress);
    emit anotherLookup();
}

void ip::Scanner::lookup(){
    m_currentTargetToEnumerate = m_args->currentTargetToEnumerate;
    m_args->currentTargetToEnumerate++;

    if(m_currentTargetToEnumerate < m_args->targetList.count())
        hostInfo->lookupHost(m_args->targetList.at(m_currentTargetToEnumerate), this, SLOT(lookupFinished(QHostInfo)));

    else
        /* at the end of the targetList, signal the thread to Quit... */
        emit quitThread();
}

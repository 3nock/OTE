/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "IPScanner.h"
#include "src/utils/s3s.h"


ip::Scanner::Scanner(ip::ScanArgs *args): AbstractScanner(nullptr),
      m_args(args)
{
    connect(this, &ip::Scanner::next, this, &ip::Scanner::lookup);
}
ip::Scanner::~Scanner(){
}

void ip::Scanner::lookupFinished(QHostInfo info){
    switch(info.error()){
    case QHostInfo::UnknownError:
        break;

    case QHostInfo::NoError:
        if(info.addresses().isEmpty() || info.hostName().isNull())
            break;

        if(info.addresses().at(0).toString() == info.hostName())
            break;

        emit scanResult(info.addresses().at(0).toString(), info.hostName());
        break;

    default:
        log.message = info.errorString();
        log.target = info.addresses().at(0).toString();
        emit scanLog(log);
        break;
    }

    /* send results and continue scan */
    m_args->progress++;
    emit scanProgress(m_args->progress);
    emit next();
}

void ip::Scanner::lookup(){
    QString target = getTarget(m_args);

    if(!(target == nullptr))
        QHostInfo::lookupHost(target,this, &ip::Scanner::lookupFinished);
    else
        emit quitThread();
}

QString ip::getTarget(ip::ScanArgs *args){
    QMutexLocker(&args->mutex);

    if(!args->targets.isEmpty())
        return args->targets.dequeue();
    else
        return nullptr;
}

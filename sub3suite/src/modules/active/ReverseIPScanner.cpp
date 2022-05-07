/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "ReverseIPScanner.h"
#include "src/utils/s3s.h"


reverseip::Scanner::Scanner(reverseip::ScanArgs *args): AbstractScanner(nullptr),
      m_args(args)
{
}
reverseip::Scanner::~Scanner(){
}

void reverseip::Scanner::lookup(){
    m_target = getTarget(m_args);
    while(m_target != nullptr) {
        m_mutex.lock();

        /* if received pause signal lock the thread, dont unlock until resume signal*/
        if(pause)
            m_wait.wait(&m_mutex);

        /* check if received stop signal */
        if(stop)
            break;

        m_mutex.unlock();

        QHostInfo info = QHostInfo::fromName(m_target);
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

        /* scan progress */
        m_args->progress++;
        emit scanProgress(m_args->progress);

        /* get next target */
        m_target = getTarget(m_args);
    }
    emit quitThread();
}

QString reverseip::getTarget(reverseip::ScanArgs *args){
    QMutexLocker(&args->mutex);

    if(!args->targets.isEmpty())
        return args->targets.dequeue();
    else
        return nullptr;
}

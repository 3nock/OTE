#include "HostActiveCheck.h"
#include <QHostAddress>


HostActiveCheck::HostActiveCheck(QObject *parent): QObject (parent)
{
    connect(m_dns, SIGNAL(finished()), this, SLOT(lookupFinished()));
}

void HostActiveCheck::lookupFinished(){
    if(m_dns->error() == QDnsLookup::NoError)
    {
        emit isActive();
        return;
    }
    else
    {
        lookup();
    }
}

void HostActiveCheck::lookup(){
    if(m_check == 0){
        m_dns->setType(QDnsLookup::A);
        m_dns->setNameserver(QHostAddress("8.8.8.8"));
    }
    if(m_check == 1){
        m_dns->setType(QDnsLookup::AAAA);
        m_dns->setNameserver(QHostAddress("8.8.8.8"));
    }
    if(m_check == 2){
        emit notActive();
        return;
    }
    m_dns->lookup();
}

void HostActiveCheck::isTargetActive(QString target){
    Q_UNUSED(target);
}

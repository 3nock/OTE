#include "Brute.h"
#include "ui_Brute.h"


void Brute::onResultSubdomain(QString subdomain, QString ip){
    if(m_subdomainSet.contains(subdomain))
        return;

    m_subdomainSet.insert(subdomain);

    /* save to model... */
    m_resultModelSubdomain->appendRow(QList<QStandardItem*>() <<new QStandardItem(subdomain) <<new QStandardItem(ip));
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());

    /* save to Project model... */
    project->addActiveSubdomain(QStringList()<<subdomain<<ip);
}

void Brute::onResultTLD(QString tld, QString ip){
    if(m_tldSet.contains(tld))
        return;

    m_tldSet.insert(tld);

    /* save to model */
    m_resultModelTld->appendRow(QList<QStandardItem*>() <<new QStandardItem(tld) <<new QStandardItem(ip));
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());

    /* save to Project model... */
    project->addActiveTLD(QStringList()<<tld<<ip);
}

#include "Brute.h"
#include "ui_Brute.h"


void Brute::onScanLog(brute::ScanLog log){
    ui->plainTextEditLogs->appendHtml("[Error]        : <font color=\"red\">"+log.message+"</font>");
    ui->plainTextEditLogs->appendHtml("[Target]   : <font color=\"red\">"+log.target+"</font>");
    ui->plainTextEditLogs->appendHtml("[Nameserver] : <font color=\"red\">"+log.nameserver+"</font>");
    /* add a new line... */
    ui->plainTextEditLogs->appendPlainText("");
}

void Brute::onResultSubdomain(QString subdomain, QString ip){
    /* save to model... */
    m_resultModelSubdomain->appendRow(QList<QStandardItem*>() <<new QStandardItem(subdomain) <<new QStandardItem(ip));
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());

    /* save to Project model... */
    project->addActiveSubdomain(QStringList()<<subdomain<<ip);
}

void Brute::onResultTLD(QString tld, QString ip){
    /* save to model */
    m_resultModelTld->appendRow(QList<QStandardItem*>() <<new QStandardItem(tld) <<new QStandardItem(ip));
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());

    /* save to Project model... */
    project->addActiveTLD(QStringList()<<tld<<ip);
}

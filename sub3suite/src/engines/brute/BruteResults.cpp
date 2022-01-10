#include "Brute.h"
#include "ui_Brute.h"


void Brute::onScanLog(scan::Log log){
    ui->plainTextEditLogs->appendHtml("[Error]      : <font color=\"red\">"+log.message+"</font>");
    ui->plainTextEditLogs->appendHtml("[Target]     : <font color=\"red\">"+log.target+"</font>");
    ui->plainTextEditLogs->appendHtml("[Nameserver] : <font color=\"red\">"+log.nameserver+"</font>");
    /* add a new line... */
    ui->plainTextEditLogs->appendPlainText("");
    m_scanStats->failed++;
}

void Brute::onResultSubdomain(QString subdomain, QString ip){
    /* if no-duplicates is allowed, prevent duplicate results using a set */
    if(m_scanConfig->noDuplicates){
        int prevSize = m_subdomainSet.size();
        m_subdomainSet.insert(subdomain+ip);
        if(prevSize == m_subdomainSet.size())
            return;
    }

    /* if multilevel scan add results to the nextleveltargets queue */
    if(m_scanConfig->multiLevelScan){
        if(m_scanArgs->currentLevel < m_scanConfig->levels)
            m_scanArgs->nextLevelTargets.enqueue(subdomain);
    }

    /* save to subdomain result model */
    m_resultModelSubdomain->appendRow(QList<QStandardItem*>() <<new QStandardItem(subdomain) <<new QStandardItem(ip));
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    m_scanStats->resolved++;

    /* save to Project model... */
    if(m_scanConfig->autoSaveToProject)
        project->addActiveSubdomain(QStringList()<<subdomain<<ip);
}

void Brute::onResultTLD(QString tld, QString ip){
    /* if no-duplicates is allowed, prevent duplicate results using a set */
    if(m_scanConfig->noDuplicates){
        int prevSize = m_tldSet.size();
        m_tldSet.insert(tld+ip);
        if(prevSize == m_tldSet.size())
            return;
    }

    /* if multilevel scan add results to the nextleveltargets queue */
    if(m_scanConfig->multiLevelScan){
        if(m_scanArgs->currentLevel < m_scanConfig->levels)
            m_scanArgs->nextLevelTargets.enqueue(tld);
    }

    /* save to model */
    m_resultModelTld->appendRow(QList<QStandardItem*>() <<new QStandardItem(tld) <<new QStandardItem(ip));
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    m_scanStats->resolved++;

    /* save to Project model... */
    if(m_scanConfig->autoSaveToProject)
        project->addActiveTLD(QStringList()<<tld<<ip);
}

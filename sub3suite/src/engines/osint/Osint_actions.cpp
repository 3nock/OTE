/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : implementation of the actions from the result' treeview's contextmenu
*/

#include "Osint.h"
#include "ui_Osint.h"

#include <QClipboard>
#include <QDesktopServices>


void Osint::openInBrowser(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Osint::clearResults(){
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        set_subdomainIP.clear();
        m_model_subdomainIp->clear();
        m_model_subdomainIp->setHorizontalHeaderLabels({tr(" Subdomains"), tr(" IpAddresses")});
        break;
    case osint::OUTPUT::SUBDOMAIN:
        set_subdomain.clear();
        m_model_subdomain->clear();
        m_model_subdomain->setHorizontalHeaderLabels({tr(" Subdomains")});
        break;
    case osint::OUTPUT::IP:
        set_ip.clear();
        m_model_ip->clear();
        m_model_ip->setHorizontalHeaderLabels({tr(" IpAddresses")});
        break;
    case osint::OUTPUT::EMAIL:
        set_email.clear();
        m_model_email->clear();
        m_model_email->setHorizontalHeaderLabels({tr(" Emails")});
        break;
    case osint::OUTPUT::URL:
        set_url.clear();
        m_model_url->clear();
        m_model_url->setHorizontalHeaderLabels({tr(" URL")});
        break;
    case osint::OUTPUT::ASN:
        set_asn.clear();
        m_model_asn->clear();
        m_model_asn->setHorizontalHeaderLabels({tr(" ASN"), tr(" Name")});
        break;
    case osint::OUTPUT::CERT:
        set_ssl.clear();
        m_model_ssl->clear();
        m_model_ssl->setHorizontalHeaderLabels({tr(" SSL")});
        break;
    case osint::OUTPUT::CIDR:
        set_cidr.clear();
        m_model_cidr->clear();
        m_model_cidr->setHorizontalHeaderLabels({tr(" CIDR")});
        break;
    }
    ui->labelResultsCount->clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Osint::removeResults(){
    QStandardItemModel *model = nullptr;
    QSet<QString> set;
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        model = m_model_subdomainIp;
        set = set_subdomainIP;
        break;
    case osint::OUTPUT::SUBDOMAIN:
        model = m_model_subdomain;
        set = set_subdomain;
        break;
    case osint::OUTPUT::IP:
        model = m_model_ip;
        set = set_ip;
        break;
    case osint::OUTPUT::EMAIL:
        model = m_model_email;
        set = set_email;
        break;
    case osint::OUTPUT::URL:
        model = m_model_url;
        set = set_url;
        break;
    case osint::OUTPUT::ASN:
        model = m_model_asn;
        set = set_asn;
        break;
    case osint::OUTPUT::CERT:
        model = m_model_ssl;
        set = set_ssl;
        break;
    case osint::OUTPUT::CIDR:
        model = m_model_cidr;
        set = set_cidr;
    }

    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        QModelIndex index = proxyModel->mapToSource(proxyIndex);
        set.remove(index.data().toString());
        model->removeRow(index.row());
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void Osint::saveResults(RESULT_TYPE result_type){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "OSINT: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "OSINT: Failed to open " << filename << " For saving Results";
        return;
    }

    switch(ui->comboBoxOutput->currentIndex())
    {
    case osint::OUTPUT::SUBDOMAINIP:
        switch(result_type){
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                file.write(proxyModel->index(i, 0).data().toString().append(NEWLINE).toUtf8());
            break;

        case RESULT_TYPE::IP:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                file.write(proxyModel->index(i, 1).data().toString().append(NEWLINE).toUtf8());
            break;

        case RESULT_TYPE::SUBDOMAINIP:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                file.write(proxyModel->index(i, 0).data().toString().append(",") \
                           .append(proxyModel->index(i, 1).data().toString()).append(NEWLINE).toUtf8());
            break;

        default:
            break;
        }
        break;

    case osint::OUTPUT::ASN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            file.write(proxyModel->index(i, 0).data().toString().append(",") \
                       .append(proxyModel->index(i, 1).data().toString()).append(NEWLINE).toUtf8());
        break;

    default:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            file.write(proxyModel->index(i, 0).data().toString().append(NEWLINE).toUtf8());
        break;
    }

    file.close();
}

void Osint::saveSelectedResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "OSINT: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "OSINT: Failed to open " << filename << " For saving Results";
        return;
    }

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        file.write(index.data().toString().append(NEWLINE).toUtf8());

    file.close();
}


void Osint::copyResults(RESULT_TYPE result_type){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;

    switch(ui->comboBoxOutput->currentIndex())
    {
    case osint::OUTPUT::SUBDOMAINIP:
        switch(result_type){
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                clipboardData.append(proxyModel->index(i, 0).data().toString().append(NEWLINE));
            break;

        case RESULT_TYPE::IP:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                clipboardData.append(proxyModel->index(i, 1).data().toString().append(NEWLINE));
            break;

        case RESULT_TYPE::SUBDOMAINIP:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                clipboardData.append(proxyModel->index(i, 0).data().toString().append(",") \
                           .append(proxyModel->index(i, 1).data().toString()).append(NEWLINE));
            break;

        default:
            break;
        }
        break;

    case osint::OUTPUT::ASN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            clipboardData.append(proxyModel->index(i, 0).data().toString().append(",") \
                       .append(proxyModel->index(i, 1).data().toString()).append(NEWLINE));
        break;

    default:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            clipboardData.append(proxyModel->index(i, 0).data().toString().append(NEWLINE));
        break;
    }

    clipboard->setText(clipboardData.trimmed());
}

void Osint::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        data.append(index.data().toString().append(NEWLINE));

    clipboard->setText(data.trimmed());
}

void Osint::sendToProject(){
    switch (ui->comboBoxOutput->currentIndex()) {
    case osint::OUTPUT::SUBDOMAINIP:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            project->addPassiveSubdomainIp(proxyModel->index(i, 0).data().toString(),
                                           proxyModel->index(i, 1).data().toString());
        break;
    case osint::OUTPUT::ASN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            project->addPassiveAsn(proxyModel->index(i, 0).data().toString(),
                                           proxyModel->index(i, 1).data().toString());
        break;
    case osint::OUTPUT::SUBDOMAIN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            project->addPassiveSubdomain(proxyModel->index(i, 0).data().toString());
        break;
    case osint::OUTPUT::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            project->addPassiveIp(proxyModel->index(i, 0).data().toString());
        break;
    case osint::OUTPUT::EMAIL:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            project->addPassiveEMail(proxyModel->index(i, 0).data().toString());
        break;
    case osint::OUTPUT::URL:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            project->addPassiveUrl(proxyModel->index(i, 0).data().toString());
        break;
    case osint::OUTPUT::CIDR:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            project->addPassiveCidr(proxyModel->index(i, 0).data().toString());
        break;
    case osint::OUTPUT::CERT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            project->addPassiveSSL(proxyModel->index(i, 0).data().toString());
        break;
    }
    emit changeTabToProject();
}

///
/// Sending results...
///
void Osint::sendToEngine(ENGINE engine, RESULT_TYPE result_type){
    int column = 0;
    if(result_type == RESULT_TYPE::IP && ui->comboBoxOutput->currentIndex() == 1)
        column = 1;

    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToOsint(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToRaw(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToBrute(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToActive(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToDns(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToCert(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToSSL();
        break;
    case ENGINE::URL:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToUrl(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

void Osint::sendSelectedToEngine(ENGINE engine, RESULT_TYPE result_type){
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToOsint(index.data().toString(), result_type);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToOsint(index.data().toString(), result_type);
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToBrute(index.data().toString(), result_type);
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToActive(index.data().toString(), result_type);
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToDns(index.data().toString(), result_type);
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToCert(index.data().toString(), result_type);
        emit changeTabToSSL();
        break;
    case ENGINE::URL:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToUrl(index.data().toString(), result_type);
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

void Osint::sendToEnum(TOOL tool, RESULT_TYPE result_type){
    int column = 0;
    if(result_type == RESULT_TYPE::IP && ui->comboBoxOutput->currentIndex() == 1)
        column = 1;

    switch (tool) {
    case TOOL::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToIpEnum(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToIpEnum();
        break;
    case TOOL::ASN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToAsnEnum(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToAsnEnum();
        break;
    case TOOL::CIDR:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToCidrEnum(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToCidrEnum();
        break;
    case TOOL::NS:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToNSEnum(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToNSEnum();
        break;
    case TOOL::MX:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToMXEnum(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToMXEnum();
        break;
    case TOOL::CERT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToSSLEnum(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToSSLEnum();
        break;
    case TOOL::EMAIL:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToEmailEnum(proxyModel->index(i, column).data().toString(), result_type);
        emit changeTabToEmailEnum();
        break;
    default:
        break;
    }
}

void Osint::sendSelectedToProject(){
    switch (ui->comboBoxOutput->currentIndex()) {
    case osint::OUTPUT::SUBDOMAINIP:
    {
        QList<QModelIndex> indexList = selectionModel->selectedIndexes();
        for(int i = 0; i < indexList.length(); i++)
        {
            QModelIndex index_subdomain = indexList.at(i);
            QModelIndex index_ip = indexList.at(i+1);
            if(index_subdomain.column() == 0 && index_ip.column() == 1)
            {
                project->addPassiveSubdomainIp(index_subdomain.data().toString(),
                                               index_ip.data().toString());
                i++;
            }
        }
        emit changeTabToProject();
    }
        break;
    case osint::OUTPUT::ASN:
    {
        QList<QModelIndex> indexList = selectionModel->selectedIndexes();
        for(int i = 0; i < indexList.length(); i++)
        {
            QModelIndex index_subdomain = indexList.at(i);
            QModelIndex index_ip = indexList.at(i+1);
            if(index_subdomain.column() == 0 && index_ip.column() == 1)
            {
                project->addPassiveAsn(index_subdomain.data().toString(),
                                               index_ip.data().toString());
                i++;
            }
        }
        emit changeTabToProject();
    }
        break;
    case osint::OUTPUT::SUBDOMAIN:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            project->addPassiveSubdomain(index.data().toString());
        break;
    case osint::OUTPUT::IP:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            project->addPassiveIp(index.data().toString());
        break;
    case osint::OUTPUT::EMAIL:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            project->addPassiveEMail(index.data().toString());
        break;
    case osint::OUTPUT::URL:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            project->addPassiveUrl(index.data().toString());
        break;
    case osint::OUTPUT::CIDR:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            project->addPassiveCidr(index.data().toString());
        break;
    case osint::OUTPUT::CERT:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            project->addPassiveSSL(index.data().toString());
        break;
    }
    emit changeTabToProject();
}

void Osint::sendSelectedToEnum(TOOL tool , RESULT_TYPE result_type){
    switch (tool) {
    case TOOL::IP:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToIpEnum(index.data().toString(), result_type);
        emit changeTabToIpEnum();
        break;
    case TOOL::ASN:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToAsnEnum(index.data().toString(), result_type);
        emit changeTabToAsnEnum();
        break;
    case TOOL::CIDR:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToCidrEnum(index.data().toString(), result_type);
        emit changeTabToCidrEnum();
        break;
    case TOOL::NS:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToNSEnum(index.data().toString(), result_type);
        emit changeTabToNSEnum();
        break;
    case TOOL::MX:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToMXEnum(index.data().toString(), result_type);
        emit changeTabToMXEnum();
        break;
    case TOOL::CERT:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToSSLEnum(index.data().toString(), result_type);
        emit changeTabToSSLEnum();
        break;
    case TOOL::EMAIL:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToEmailEnum(index.data().toString(), result_type);
        emit changeTabToEmailEnum();
        break;
    default:
        break;
    }
}

///
/// receive targets...
///
void Osint::onReceiveTargets(QString target, RESULT_TYPE result_type){
    switch (result_type) {
    case RESULT_TYPE::SUBDOMAIN:
        ui->comboBoxInput->setCurrentIndex(INPUT::HOSTNAME);
        break;
    case RESULT_TYPE::IP:
        ui->comboBoxInput->setCurrentIndex(INPUT::IP);
        break;
    case RESULT_TYPE::ASN:
        ui->comboBoxInput->setCurrentIndex(INPUT::ASN);
        break;
    case RESULT_TYPE::CIDR:
        ui->comboBoxInput->setCurrentIndex(INPUT::CIDR);
        break;
    case RESULT_TYPE::CERT_ID:
        ui->comboBoxInput->setCurrentIndex(INPUT::CERT);
        break;
    default:
        break;
    }

    ui->targets->add(target);
    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

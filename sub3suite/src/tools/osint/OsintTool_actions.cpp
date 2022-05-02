/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : implementation of the actions from the result' treeview's contextmenu
*/

#include "OsintTool.h"
#include "ui_OsintTool.h"

#include <QClipboard>
#include <QDesktopServices>


void OsintTool::openInBrowser(){
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::URL:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            QDesktopServices::openUrl(QUrl(index.data().toString(), QUrl::TolerantMode));
        break;
    default:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
        break;
    }
}

void OsintTool::clearResults(){
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

void OsintTool::removeResults(){
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

    auto model_selectedIndexes = proxyModel->mapSelectionToSource(selectionModel->selection());
    QModelIndexList selectedIndexes = model_selectedIndexes.indexes();
    for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
        set.remove(i->data().toString());
        model->removeRow(i->row());
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void OsintTool::extract(bool subdomain, bool tld){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QSet<QString> extracts;

    /* extracting and saving to a set to avoid repeatition */
    for(int i = 0; i != proxyModel->rowCount(); ++i){
        if(subdomain)
            extracts.insert(proxyModel->index(i, 0).data().toString().split(".").at(0));
        if(tld)
            extracts.insert(proxyModel->index(i, 0).data().toString().split(".").last());
    }

    /* setting the data to clipboard */
    QString data;
    foreach(const QString &extract, extracts)
        data.append(extract).append(NEWLINE);
    clipboard->setText(data.trimmed());
}

void OsintTool::extractSelected(bool subdomain, bool tld){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QSet<QString> extracts;

    /* extracting and saving to a set to avoid repeatition */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column())
            continue;

        if(subdomain)
            extracts.insert(index.data().toString().split(".").at(0));
        if(tld)
            extracts.insert(index.data().toString().split(".").last());
    }

    /* setting the data to clipboard */
    QString data;
    foreach(const QString &extract, extracts)
        data.append(extract).append(NEWLINE);
    clipboard->setText(data.trimmed());
}

void OsintTool::saveResults(const RESULT_TYPE& result_type){
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

void OsintTool::saveSelectedResults(){
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


void OsintTool::copyResults(const RESULT_TYPE &result_type){
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

void OsintTool::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        data.append(index.data().toString().append(NEWLINE));

    clipboard->setText(data.trimmed());
}

///
/// Sending results...
///

void OsintTool::sendToProject(){
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

void OsintTool::sendSelectedToProject(){
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

void OsintTool::sendToEngine(const TOOL &engine, const RESULT_TYPE  &result_type){
    QSet<QString> targets;

    /* getting targets */
    switch (result_type) {
    case RESULT_TYPE::IP:
        if(ui->comboBoxOutput->currentIndex() == 1){ // subdomain_ip
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                targets.insert(proxyModel->index(i, 1).data().toString());
        }else {
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                targets.insert(proxyModel->index(i, 0).data().toString());
        }
        break;

    default:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            targets.insert(proxyModel->index(i, 0).data().toString());
        break;
    }

    /* sending the targets */
    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(targets, result_type);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(targets, result_type);
        emit changeTabToRaw();
        break;
    case TOOL::BRUTE:
        emit sendToBrute(targets, result_type);
        emit changeTabToBrute();
        break;
    case TOOL::HOST:
        emit sendToHost(targets, result_type);
        emit changeTabToHost();
        break;
    case TOOL::IP:
        emit sendToIP(targets, result_type);
        emit changeTabToIP();
        break;
    case TOOL::DNS:
        emit sendToDns(targets, result_type);
        emit changeTabToDns();
        break;
    case TOOL::SSL:
        emit sendToSsl(targets, result_type);
        emit changeTabToSSL();
        break;
    case TOOL::URL:
        emit sendToUrl(targets, result_type);
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

void OsintTool::sendSelectedToEngine(const TOOL &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting targets */
    switch (result_type) {
    case RESULT_TYPE::IP:
        if(ui->comboBoxOutput->currentIndex() == 1){ // subdomain_ip
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                if(index.column())
                    targets.insert(index.data().toString());
        }else {
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                targets.insert(index.data().toString());
        }
        break;

    default:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            targets.insert(index.data().toString());
        break;
    }

    /* sending the targets */
    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(targets, result_type);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(targets, result_type);
        emit changeTabToRaw();
        break;
    case TOOL::BRUTE:
        emit sendToBrute(targets, result_type);
        emit changeTabToBrute();
        break;
    case TOOL::HOST:
        emit sendToHost(targets, result_type);
        emit changeTabToHost();
        break;
    case TOOL::IP:
        emit sendToIP(targets, result_type);
        emit changeTabToIP();
        break;
    case TOOL::DNS:
        emit sendToDns(targets, result_type);
        emit changeTabToDns();
        break;
    case TOOL::SSL:
        emit sendToSsl(targets, result_type);
        emit changeTabToSSL();
        break;
    case TOOL::URL:
        emit sendToUrl(targets, result_type);
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

void OsintTool::sendToEnum(const ENUMERATOR &tool, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting targets */
    switch (result_type) {
    case RESULT_TYPE::IP:
        if(ui->comboBoxOutput->currentIndex() == 1){ // subdomain_ip
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                targets.insert(proxyModel->index(i, 1).data().toString());
        }else {
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                targets.insert(proxyModel->index(i, 0).data().toString());
        }
        break;

    default:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            targets.insert(proxyModel->index(i, 0).data().toString());
        break;
    }

    /* sending targets */
    switch (tool) {
    case ENUMERATOR::IP:
        emit sendToIpEnum(targets, result_type);
        emit changeTabToIpEnum();
        break;
    case ENUMERATOR::ASN:
        emit sendToAsnEnum(targets, result_type);
        emit changeTabToAsnEnum();
        break;
    case ENUMERATOR::CIDR:
        emit sendToCidrEnum(targets, result_type);
        emit changeTabToCidrEnum();
        break;
    case ENUMERATOR::NS:
        emit sendToNSEnum(targets, result_type);
        emit changeTabToNSEnum();
        break;
    case ENUMERATOR::MX:
        emit sendToMXEnum(targets, result_type);
        emit changeTabToMXEnum();
        break;
    case ENUMERATOR::SSL:
        emit sendToSSLEnum(targets, result_type);
        emit changeTabToSSLEnum();
        break;
    case ENUMERATOR::EMAIL:
        emit sendToEmailEnum(targets, result_type);
        emit changeTabToEmailEnum();
        break;
    }
}

void OsintTool::sendSelectedToEnum(const ENUMERATOR &tool , const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting targets */
    switch (result_type) {
    case RESULT_TYPE::IP:
        if(ui->comboBoxOutput->currentIndex() == 1){ // subdomain_ip
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                if(index.column())
                    targets.insert(index.data().toString());
        }else {
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                targets.insert(index.data().toString());
        }
        break;

    default:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            targets.insert(index.data().toString());
        break;
    }

    /* sending targets */
    switch (tool) {
    case ENUMERATOR::IP:
        emit sendToIpEnum(targets, result_type);
        emit changeTabToIpEnum();
        break;
    case ENUMERATOR::ASN:
        emit sendToAsnEnum(targets, result_type);
        emit changeTabToAsnEnum();
        break;
    case ENUMERATOR::CIDR:
        emit sendToCidrEnum(targets, result_type);
        emit changeTabToCidrEnum();
        break;
    case ENUMERATOR::NS:
        emit sendToNSEnum(targets, result_type);
        emit changeTabToNSEnum();
        break;
    case ENUMERATOR::MX:
        emit sendToMXEnum(targets, result_type);
        emit changeTabToMXEnum();
        break;
    case ENUMERATOR::SSL:
        emit sendToSSLEnum(targets, result_type);
        emit changeTabToSSLEnum();
        break;
    case ENUMERATOR::EMAIL:
        emit sendToEmailEnum(targets, result_type);
        emit changeTabToEmailEnum();
        break;
    }
}

///
/// receive targets...
///

void OsintTool::onReceiveTargets(QSet<QString> targets, RESULT_TYPE result_type){
    switch (result_type) {
    case RESULT_TYPE::SUBDOMAIN:
        ui->comboBoxInput->setCurrentIndex(OSINT_INPUT::HOSTNAME);
        break;
    case RESULT_TYPE::IP:
        ui->comboBoxInput->setCurrentIndex(OSINT_INPUT::IP);
        break;
    case RESULT_TYPE::ASN:
        ui->comboBoxInput->setCurrentIndex(OSINT_INPUT::ASN);
        break;
    case RESULT_TYPE::CIDR:
        ui->comboBoxInput->setCurrentIndex(OSINT_INPUT::CIDR);
        break;
    case RESULT_TYPE::CERT_ID:
        ui->comboBoxInput->setCurrentIndex(OSINT_INPUT::CERT);
        break;
    case RESULT_TYPE::EMAIL:
        ui->comboBoxInput->setCurrentIndex(OSINT_INPUT::EMAIL);
        break;
    case RESULT_TYPE::URL:
        ui->comboBoxInput->setCurrentIndex(OSINT_INPUT::URL);
        break;
    default:
        break;
    }

    ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

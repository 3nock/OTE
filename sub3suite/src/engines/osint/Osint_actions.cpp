/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Osint.h"
#include "ui_Osint.h"

#include <QClipboard>
#include <QDesktopServices>


void Osint::m_openInBrowser(QItemSelectionModel *selectionModel){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Osint::m_removeResults(QItemSelectionModel *selectionModel){
    QStandardItemModel *model = nullptr;
    QSet<QString> set;
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        model = m_resultModelSubdomainIp;
        set = m_subdomainIpSet;
        break;
    case osint::OUTPUT::SUBDOMAIN:
        model = m_resultModelSubdomain;
        set = m_subdomainSet;
        break;
    case osint::OUTPUT::IP:
        model = m_resultModelIp;
        set = m_ipSet;
        break;
    case osint::OUTPUT::EMAIL:
        model = m_resultModelEmail;
        set = m_emailSet;
        break;
    case osint::OUTPUT::URL:
        model = m_resultModelUrl;
        set = m_urlSet;
        break;
    case osint::OUTPUT::ASN:
        model = m_resultModelAsn;
        set = m_asnSet;
        break;
    case osint::OUTPUT::CERT:
        model = m_resultModelCert;
        set = m_sslCertSet;
        break;
    case osint::OUTPUT::CIDR:
        model = m_resultModelCidr;
        set = m_cidrSet;
    }

    QModelIndex index;
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        index = m_resultProxyModel->mapToSource(proxyIndex);
        set.remove(index.data().toString());
        model->removeRow(index.row());
    }
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Osint::m_saveResults(RESULT_TYPE resultType){
    /* checks */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())
        return;

    /* for subdomainIp model, since it has subdomain & ip */
    QString item;
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        switch(resultType){
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
            {
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
                file.write(item.toUtf8());
            }
            break;

        case RESULT_TYPE::IP:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
            {
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString().append(NEWLINE);
                file.write(item.toUtf8());
            }
            break;

        case RESULT_TYPE::SUBDOMAINIP:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
            {
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString()+":"+m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
                file.write(item.toUtf8());
            }
            break;

        default:
            break;
        }
        file.close();
        return;
    }

    /* for other result type */
    for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
    {
        item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString()+":"+m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
        file.write(item.toUtf8());
    }

    file.close();
}

void Osint::m_saveResults(QItemSelectionModel *selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    /* saving to file */
    QString data;
    QString item;
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            data.append(item.append(NEWLINE));
        }
        file.write(data.toUtf8());
        file.close();
    }
}


void Osint::m_copyResults(RESULT_TYPE resultType){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QString item;

    /* for subdomainIp result model */
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        switch(resultType){
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
            {
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
                clipboardData.append(item);
            }
            break;

        case RESULT_TYPE::IP:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
            {
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString().append(NEWLINE);
                clipboardData.append(item);
            }
            break;

        case RESULT_TYPE::SUBDOMAINIP:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
            {
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString()+":"+m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
                clipboardData.append(item);
            }
            break;

        default:
            break;
        }
        clipboard->setText(clipboardData.trimmed());
        return;
    }

    /* for other result types */
    for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
    {
        item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString()+":"+m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
        clipboardData.append(item);
    }

    clipboard->setText(clipboardData.trimmed());
}

void Osint::m_copyResults(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;
    QString item;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        data.append(item.append(NEWLINE));
    }

    clipboard->setText(data.trimmed());
}

void Osint::onReceiveTargets(QString target, RESULT_TYPE resultType){
    switch (resultType) {
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

/*****************************************************************************
                            SENDING RESULTS
******************************************************************************/
void Osint::m_sendSubdomainToEngine(ENGINE engine){
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToOsint(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToRaw(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToBrute(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToActive(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToDns(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToCert(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Osint::m_sendIpToEngine(ENGINE engine){
    QString item;
    switch (ui->comboBoxOutput->currentIndex()) {
    case osint::OUTPUT::IP:
        switch (engine) {
        case ENGINE::OSINT:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
                emit sendResultsToOsint(item, RESULT_TYPE::IP);
            }
            emit changeTabToOsint();
            break;
        case ENGINE::RAW:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
                emit sendResultsToRaw(item, RESULT_TYPE::IP);
            }
            emit changeTabToRaw();
            break;
        }
        break;

    case osint::OUTPUT::SUBDOMAINIP:
        switch (engine) {
        case ENGINE::OSINT:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString();
                emit sendResultsToOsint(item, RESULT_TYPE::IP);
            }
            emit changeTabToOsint();
            break;
        case ENGINE::RAW:
            for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
                item = m_resultProxyModel->data(m_resultProxyModel->index(i, 1)).toString();
                emit sendResultsToRaw(item, RESULT_TYPE::IP);
            }
            emit changeTabToRaw();
            break;

        default:
            break;
        }
    }
}

void Osint::m_sendToEngine(ENGINE engine){
    /* getting the result type*/
    RESULT_TYPE resultType;
    switch (ui->comboBoxOutput->currentIndex()) {
    case osint::OUTPUT::ASN:
        resultType = RESULT_TYPE::ASN;
        break;
    case osint::OUTPUT::CIDR:
        resultType = RESULT_TYPE::CIDR;
        break;
    case osint::OUTPUT::CERT:
        resultType = RESULT_TYPE::CERT_ID;
        break;
    case osint::OUTPUT::EMAIL:
        resultType = RESULT_TYPE::EMAIL;
        break;
    case osint::OUTPUT::URL:
        resultType = RESULT_TYPE::URL;
    }

    /* sending the results */
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToOsint(item, resultType);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
            item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
            emit sendResultsToRaw(item, resultType);
        }
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Osint::m_sendSubdomainToEngine(ENGINE engine, QItemSelectionModel *selection){
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToOsint(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToRaw(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToBrute(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToActive(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToDns(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToCert(item, RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Osint::m_sendIpToEngine(ENGINE engine, QItemSelectionModel *selection){
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToOsint(item, RESULT_TYPE::IP);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToRaw(item, RESULT_TYPE::IP);
        }
        emit changeTabToRaw();
        break;

    default:
        break;
    }
}

void Osint::m_sendToEngine(ENGINE engine, QItemSelectionModel *selection){
    /* getting the result type */
    RESULT_TYPE resultType;
    switch (ui->comboBoxOutput->currentIndex()) {
    case osint::OUTPUT::ASN:
        resultType = RESULT_TYPE::ASN;
        break;
    case osint::OUTPUT::CIDR:
        resultType = RESULT_TYPE::CIDR;
        break;
    case osint::OUTPUT::CERT:
        resultType = RESULT_TYPE::CERT_ID;
        break;
    case osint::OUTPUT::EMAIL:
        resultType = RESULT_TYPE::EMAIL;
        break;
    case osint::OUTPUT::URL:
        resultType = RESULT_TYPE::URL;
    }

    /* sending the results */
    QString item;
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToOsint(item, resultType);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            item = index.data().toString();
            emit sendResultsToRaw(item, resultType);
        }
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Osint::m_sendSubdomainToTool(TOOL){
    /* not yet */
}

void Osint::m_sendIpToTool(TOOL){
    /* not yet */
}

void Osint::m_sendToTool(TOOL){
    /* not yet */
}

void Osint::m_sendSubdomainToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}

void Osint::m_sendIpToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}

void Osint::m_sendToTool(TOOL, QItemSelectionModel *){
    /* not yet */
}


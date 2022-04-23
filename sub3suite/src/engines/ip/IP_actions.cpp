/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "IP.h"
#include "ui_IP.h"

#include <QClipboard>
#include <QFileDialog>
#include <QDesktopServices>


void IP::clearResults(){
    /* clear the results */
    switch (ui->comboBoxOption->currentIndex()) {
    case 0: // DNS
        m_model_dns->clear();
        m_model_dns->setHorizontalHeaderLabels({tr(" IP"), tr(" Hostname")});
        set_subdomain.clear();
        break;
    case 1:
        m_model_port->clear();
        m_model_ping->setHorizontalHeaderLabels({tr(" IP"), tr(" Open Ports")});
        break;
    case 2:
        m_model_ping->clear();
        m_model_ping->setHorizontalHeaderLabels({tr(" IP"), tr(" Bytes"), tr(" Time(ms)"), tr(" TTL")});
        break;
    }
    ui->labelResultsCount->clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void IP::openInBrowser(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void IP::removeResults(){
    auto model_selectedIndexes = proxyModel->mapSelectionToSource(selectionModel->selection());
    QModelIndexList selectedIndexes = model_selectedIndexes.indexes();
    switch (ui->comboBoxOption->currentIndex()) {
    case 0: // DNS
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            set_subdomain.remove(i->data().toString());
            m_model_dns->removeRow(i->row());
        }
        break;
    case 1: // PORT
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            set_subdomain.remove(i->data().toString());
            m_model_port->removeRow(i->row());
        }
        break;
    case 2: // PING
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            set_subdomain.remove(i->data().toString());
            m_model_ping->removeRow(i->row());
        }
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void IP::saveResults(const RESULT_TYPE &result_type){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "ACTIVE: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "ACTIVE: Failed to open " << filename << " For saving Results";
        return;
    }

    switch(result_type){
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            file.write(proxyModel->index(i, 0).data().toString().append(NEWLINE).toUtf8());
        break;

    case RESULT_TYPE::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QString ipv4(proxyModel->index(i, 1).data().toString());
            QString ipv6(proxyModel->index(i, 2).data().toString());
            if(!ipv4.isEmpty())
                file.write(ipv4.append(NEWLINE).toUtf8());
            if(!ipv6.isEmpty())
                file.write(ipv6.append(NEWLINE).toUtf8());
        }
        break;

    case RESULT_TYPE::CSV:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QString host(proxyModel->index(i, 0).data().toString());
            QString ipv4(proxyModel->index(i, 1).data().toString());
            QString ipv6(proxyModel->index(i, 2).data().toString());

            if(!ipv4.isEmpty())
                host.append(",").append(ipv4);
            if(!ipv6.isEmpty())
                host.append(",").append(ipv6);

            file.write(host.append(NEWLINE).toUtf8());
        }
        break;

    case RESULT_TYPE::JSON:
    {
        QJsonDocument document;
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::HOST *item = static_cast<s3s_item::HOST*>(m_model_dns->itemFromIndex(model_index));
            array.append(host_to_json(item));
        }
        document.setArray(array);
        file.write(document.toJson());
        break;
    }

    default:
        break;
    }

    file.close();
}

void IP::saveSelectedResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "ACTIVE: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "ACTIVE: Failed to open " << filename << " For saving Results";
        return;
    }

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        file.write(index.data().toString().append(NEWLINE).toUtf8());

    file.close();
}


void IP::copyResults(const RESULT_TYPE &result_type){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;

    switch(result_type){
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            clipboardData.append(proxyModel->index(i, 0).data().toString()).append(NEWLINE);
        break;

    case RESULT_TYPE::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QString ipv4(proxyModel->index(i, 1).data().toString());
            QString ipv6(proxyModel->index(i, 2).data().toString());
            if(!ipv4.isEmpty())
                clipboardData.append(ipv4).append(NEWLINE);
            if(!ipv6.isEmpty())
                clipboardData.append(ipv6).append(NEWLINE);
        }
        break;

    case RESULT_TYPE::CSV:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QString host(proxyModel->index(i, 0).data().toString());
            QString ipv4(proxyModel->index(i, 1).data().toString());
            QString ipv6(proxyModel->index(i, 2).data().toString());

            if(!ipv4.isEmpty())
                host.append(",").append(ipv4);
            if(!ipv6.isEmpty())
                host.append(",").append(ipv6);

            clipboardData.append(host).append(NEWLINE);
        }
        break;

    case RESULT_TYPE::JSON:
    {
        QJsonDocument document;
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::HOST *item = static_cast<s3s_item::HOST*>(m_model_dns->itemFromIndex(model_index));
            array.append(host_to_json(item));
        }
        document.setArray(array);
        clipboardData.append(document.toJson());
    }
        break;

    default:
        break;
    }

    clipboard->setText(clipboardData.trimmed());
}

void IP::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        data.append(index.data().toString().append(NEWLINE));

    clipboard->setText(data.trimmed());
}

void IP::extract(bool subdomain, bool tld){
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

void IP::extractSelected(bool subdomain, bool tld){
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

///
/// sending results...
///

void IP::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::HOST *item = static_cast<s3s_item::HOST*>(m_model_dns->itemFromIndex(index));
        project->addActiveHost(host_to_struct(item));
    }
}

void IP::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column())
            continue;
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::HOST *item = static_cast<s3s_item::HOST*>(m_model_dns->itemFromIndex(model_index));
        project->addActiveHost(host_to_struct(item));
    }
}

void IP::sendToEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting the targets */
    switch (result_type) {
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            targets.insert(proxyModel->index(i, 0).data().toString());
        break;
    case RESULT_TYPE::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            targets.insert(proxyModel->index(i, 1).data().toString());
            targets.insert(proxyModel->index(i, 2).data().toString());
        }
        break;
    default:
        break;
    }

    /* sending the targets */
    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(targets, result_type);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(targets, result_type);
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        emit sendToBrute(targets, result_type);
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        emit sendToActive(targets, result_type);
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        emit sendToDns(targets, result_type);
        emit changeTabToDns();
        break;
    case ENGINE::SSL:
        emit sendToSsl(targets, result_type);
        emit changeTabToSSL();
        break;
    case ENGINE::URL:
        emit sendToUrl(targets, result_type);
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

void IP::sendSelectedToEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting the targets */
    switch (result_type) {
    case RESULT_TYPE::SUBDOMAIN:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column() == 0)
                targets.insert(index.data().toString());
        }
        break;
    case RESULT_TYPE::IP:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                targets.insert(index.data().toString());
        }
        break;
    default:
        break;
    }

    /* sending the targets */
    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(targets, result_type);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(targets, result_type);
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        emit sendToBrute(targets, result_type);
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        emit sendToActive(targets, result_type);
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        emit sendToDns(targets, result_type);
        emit changeTabToDns();
        break;
    case ENGINE::SSL:
        emit sendToSsl(targets, result_type);
        emit changeTabToSSL();
        break;
    case ENGINE::URL:
        emit sendToUrl(targets, result_type);
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

void IP::sendToEnum(const ENUMERATOR &tool){
    QSet<QString> targets;

    /* getting the targets */
    for(int i = 0; i != proxyModel->rowCount(); ++i){
        targets.insert(proxyModel->index(i, 1).data().toString());
        targets.insert(proxyModel->index(i, 2).data().toString());
    }

    /* sending the targets */
    switch (tool) {
    case ENUMERATOR::IP:
        emit sendToIpEnum(targets, RESULT_TYPE::IP);
        emit changeTabToIpEnum();
        break;
    default:
        break;
    }
}

void IP::sendSelectedToEnum(const ENUMERATOR &tool){
    QSet<QString> targets;

    /* getting the targets */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column())
            targets.insert(index.data().toString());
    }

    /* sending the targets */
    switch (tool) {
    case ENUMERATOR::IP:
        emit sendToIpEnum(targets, RESULT_TYPE::IP);
        emit changeTabToIpEnum();
        break;
    default:
        break;
    }
}

///
/// receiving targets...
///

void IP::onReceiveTargets(QSet<QString> targets, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::SUBDOMAIN)
        ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "IPTool.h"
#include "ui_IPTool.h"

#include <QClipboard>
#include <QFileDialog>
#include <QDesktopServices>


void IPTool::clearResults(){
    /* clear the results */
    switch (ui->comboBoxOption->currentIndex()) {
    case 0: // REVERSE IP
        m_model_dns->clear();
        m_model_dns->setHorizontalHeaderLabels({tr(" IP"), tr(" Hostname")});
        break;
    case 1: // PORT
        m_model_port->clear();
        m_model_port->setHorizontalHeaderLabels({tr(" IP"), tr(" Ports")});
        set_ports.clear();
        break;
    case 2: // PING
        m_model_ping->clear();
        m_model_ping->setHorizontalHeaderLabels({tr(" IP"), tr(" Time(ms)")});
        break;
    }
    ui->labelResultsCount->clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void IPTool::openInBrowser(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void IPTool::removeResults(){
    auto model_selectedIndexes = proxyModel->mapSelectionToSource(selectionModel->selection());
    QModelIndexList selectedIndexes = model_selectedIndexes.indexes();
    switch (ui->comboBoxOption->currentIndex()) {
    case 0: // DNS
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            m_model_dns->removeRow(i->row());
        break;
    case 1: // PORT
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            set_ports.remove(i->data().toString());
            m_model_port->removeRow(i->row());
        }
        break;
    case 2: // PING
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            m_model_ping->removeRow(i->row());
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void IPTool::saveResults(const RESULT_TYPE &result_type){
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
            file.write(proxyModel->index(i, 1).data().toString().append(NEWLINE).toUtf8());
        break;

    case RESULT_TYPE::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            file.write(proxyModel->index(i, 0).data().toString().append(NEWLINE).toUtf8());
        break;

    default:
        break;
    }

    file.close();
}

void IPTool::saveSelectedResults(){
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


void IPTool::copyResults(const RESULT_TYPE &result_type){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;

    switch(result_type){
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            clipboardData.append(proxyModel->index(i, 1).data().toString()).append(NEWLINE);
        break;

    case RESULT_TYPE::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            clipboardData.append(proxyModel->index(i, 0).data().toString()).append(NEWLINE);
        break;

    default:
        break;
    }

    clipboard->setText(clipboardData.trimmed());
}

void IPTool::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        data.append(index.data().toString().append(NEWLINE));

    clipboard->setText(data.trimmed());
}

///
/// sending results...
///

void IPTool::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::IPTool *item = static_cast<s3s_item::IPTool*>(m_model_port->itemFromIndex(index));
        project->addActiveIP(iptool_to_struct(item));
    }
}

void IPTool::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column())
            continue;
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::IPTool *item = static_cast<s3s_item::IPTool*>(m_model_port->itemFromIndex(model_index));
        project->addActiveIP(iptool_to_struct(item));
    }
}

void IPTool::sendToEngine(const TOOL &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting the targets */
    switch (result_type) {
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            targets.insert(proxyModel->index(i, 1).data().toString());
        break;
    case RESULT_TYPE::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            targets.insert(proxyModel->index(i, 0).data().toString());
        break;
    default:
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

void IPTool::sendSelectedToEngine(const TOOL &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting the targets */
    switch (result_type) {
    case RESULT_TYPE::SUBDOMAIN:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column() == 1)
                targets.insert(index.data().toString());
        }
        break;
    case RESULT_TYPE::IP:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column() == 0)
                targets.insert(index.data().toString());
        }
        break;
    default:
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

void IPTool::sendToEnum(const ENUMERATOR &tool){
    QSet<QString> targets;

    /* getting the targets */
    for(int i = 0; i != proxyModel->rowCount(); ++i)
        targets.insert(proxyModel->index(i, 0).data().toString());

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

void IPTool::sendSelectedToEnum(const ENUMERATOR &tool){
    QSet<QString> targets;

    /* getting the targets */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column() == 0)
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

void IPTool::onReceiveTargets(QSet<QString> targets, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::IP)
        ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

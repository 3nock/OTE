/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Active.h"
#include "ui_Active.h"

#include <QClipboard>
#include <QFileDialog>
#include <QDesktopServices>

/*
 * TODO:
 *      insert the send/copy/save targets to set before appending to clipboard or file
 */
void Active::clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({tr(" Host"), tr(" Ipv4"), tr(" Ipv6"), tr(" Ports")});
    set_subdomain.clear();

    ui->tableViewResults->horizontalHeader()->resizeSection(0, 200);
    ui->tableViewResults->horizontalHeader()->resizeSection(1, 100);
    ui->tableViewResults->horizontalHeader()->resizeSection(2, 150);
    ui->tableViewResults->horizontalHeader()->resizeSection(3, 100);
    ui->tableViewResults->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Active::openInBrowser(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Active::removeResults(){
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        QModelIndex index = proxyModel->mapToSource(proxyIndex);
        set_subdomain.remove(index.data().toString());
        m_model->removeRow(index.row());
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void Active::saveResults(const RESULT_TYPE &result_type){
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
            s3s_item::HOST *item = static_cast<s3s_item::HOST*>(m_model->itemFromIndex(model_index));
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

void Active::saveSelectedResults(){
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


void Active::copyResults(const RESULT_TYPE &result_type){
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
            s3s_item::HOST *item = static_cast<s3s_item::HOST*>(m_model->itemFromIndex(model_index));
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

void Active::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        data.append(index.data().toString().append(NEWLINE));

    clipboard->setText(data.trimmed());
}

void Active::extract(bool subdomain, bool tld){
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

void Active::extractSelected(bool subdomain, bool tld){
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

void Active::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::HOST *item = static_cast<s3s_item::HOST*>(m_model->itemFromIndex(index));
        project->addActiveHost(host_to_struct(item));
    }
}

void Active::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column())
            continue;
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::HOST *item = static_cast<s3s_item::HOST*>(m_model->itemFromIndex(model_index));
        project->addActiveHost(host_to_struct(item));
    }
}

void Active::sendToEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
    switch (engine) {
    case ENGINE::OSINT:
        switch (result_type) {
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                emit sendResultsToOsint(proxyModel->index(i, 0).data().toString(), result_type);
            emit changeTabToOsint();
            break;
        case RESULT_TYPE::IP:
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QString ipv4(proxyModel->index(i, 1).data().toString());
                QString ipv6(proxyModel->index(i, 2).data().toString());
                if(!ipv4.isEmpty())
                    emit sendResultsToOsint(ipv4, result_type);
                if(!ipv6.isEmpty())
                    emit sendResultsToOsint(ipv6, result_type);
            }
            emit changeTabToOsint();
            break;
        default:
            break;
        }
        break;
    case ENGINE::RAW:
        switch (result_type) {
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                emit sendResultsToRaw(proxyModel->index(i, 0).data().toString(), result_type);
            emit changeTabToRaw();
            break;
        case RESULT_TYPE::IP:
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QString ipv4(proxyModel->index(i, 1).data().toString());
                QString ipv6(proxyModel->index(i, 2).data().toString());
                if(!ipv4.isEmpty())
                    emit sendResultsToRaw(ipv4, result_type);
                if(!ipv6.isEmpty())
                    emit sendResultsToRaw(ipv6, result_type);
            }
            emit changeTabToRaw();
            break;
        default:
            break;
        }
        break;
    case ENGINE::BRUTE:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToBrute(proxyModel->index(i, 0).data().toString(), result_type);
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToActive(proxyModel->index(i, 0).data().toString(), result_type);
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToDns(proxyModel->index(i, 0).data().toString(), result_type);
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToCert(proxyModel->index(i, 0).data().toString(), result_type);
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Active::sendSelectedToEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(result_type == RESULT_TYPE::SUBDOMAIN){
                if(index.column() == 0)
                    emit sendResultsToOsint(index.data().toString(), result_type);
            }
            else{
                if(index.column())
                    emit sendResultsToOsint(index.data().toString(), result_type);
            }
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(result_type == RESULT_TYPE::SUBDOMAIN){
                if(index.column() == 0)
                    emit sendResultsToRaw(index.data().toString(), result_type);
            }
            else{
                if(index.column())
                    emit sendResultsToRaw(index.data().toString(), result_type);
            }
        }
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                continue;
            emit sendResultsToBrute(index.data().toString(), result_type);
        }
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                continue;
            emit sendResultsToActive(index.data().toString(), result_type);
        }
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                continue;
            emit sendResultsToDns(index.data().toString(), result_type);
        }
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                continue;
            emit sendResultsToCert(index.data().toString(), result_type);
        }
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Active::sendToEnum(const TOOL &tool){
    switch (tool) {
    case TOOL::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QString ipv4(proxyModel->index(i, 1).data().toString());
            QString ipv6(proxyModel->index(i, 2).data().toString());

            if(!ipv4.isEmpty())
                emit sendResultsToIpEnum(ipv4, RESULT_TYPE::IP);
            if(!ipv6.isEmpty())
                emit sendResultsToIpEnum(ipv6, RESULT_TYPE::IP);
        }
        emit changeTabToIpEnum();
        break;
    default:
        break;
    }
}

void Active::sendSelectedToEnum(const TOOL &tool){
    switch (tool) {
    case TOOL::IP:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                emit sendResultsToIpEnum(index.data().toString(), RESULT_TYPE::IP);
        }
        emit changeTabToIpEnum();
        break;
    default:
        break;
    }

}

///
/// receiving targets...
///

void Active::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::SUBDOMAIN)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

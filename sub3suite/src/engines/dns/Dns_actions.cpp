/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Dns.h"
#include "ui_Dns.h"

#include <QClipboard>
#include <QDesktopServices>


void Dns::clearResults(){
    /* clear the results... */
    m_model->clear();
    m_model->setHorizontalHeaderLabels({tr(" DNS"), tr(" Values")});
    ui->labelResultsCount->clear();
    m_resultSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Dns::openInBrowser(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Dns::removeResults(){
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        QModelIndex index = proxyModel->mapToSource(proxyIndex);
        m_resultSet.remove(index.data().toString());
        m_model->removeRow(index.row());
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void Dns::saveResults(const RESULT_TYPE &result_type){
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
    case RESULT_TYPE::DNS:
    {
        QJsonDocument document;
        QJsonArray dns_array;
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));
            dns_array.append(dns_to_json(item));
        }
        document.setArray(dns_array);
        file.write(document.toJson());
    }
        break;

    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                file.write(item->CNAME->child(j, 1)->text().append(NEWLINE).toUtf8());
            for(int j = 0; j < item->NS->rowCount(); j++)
                file.write(item->NS->child(j, 1)->text().append(NEWLINE).toUtf8());
            for(int j = 0; j < item->MX->rowCount(); j++)
                file.write(item->MX->child(j, 1)->text().append(NEWLINE).toUtf8());
        }
        break;

    case RESULT_TYPE::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->A->rowCount(); j++)
                file.write(item->A->child(j, 1)->text().append(NEWLINE).toUtf8());
            for(int j = 0; j < item->AAAA->rowCount(); j++)
                file.write(item->AAAA->child(j, 1)->text().append(NEWLINE).toUtf8());
        }
        break;

    case RESULT_TYPE::A:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->A->rowCount(); j++)
                file.write(item->A->child(j, 1)->text().append(NEWLINE).toUtf8());
        }
        break;

    case RESULT_TYPE::AAAA:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->AAAA->rowCount(); j++)
                file.write(item->AAAA->child(j, 1)->text().append(NEWLINE).toUtf8());
        }
        break;

    case RESULT_TYPE::NS:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->NS->rowCount(); j++)
                file.write(item->NS->child(j, 1)->text().append(NEWLINE).toUtf8());
        }
        break;

    case RESULT_TYPE::MX:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->MX->rowCount(); j++)
                file.write(item->MX->child(j, 1)->text().append(NEWLINE).toUtf8());
        }
        break;

    case RESULT_TYPE::CNAME:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                file.write(item->CNAME->child(j, 1)->text().append(NEWLINE).toUtf8());
        }
        break;

    case RESULT_TYPE::TXT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->TXT->rowCount(); j++)
                file.write(item->TXT->child(j, 1)->text().append(NEWLINE).toUtf8());
        }
        break;

    default:
        break;
    }

    file.close();
}

void Dns::saveSelectedResults(){
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


void Dns::copyResults(const RESULT_TYPE &result_type){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;

    switch(result_type){
    case RESULT_TYPE::DNS:
    {
        QJsonDocument document;
        QJsonArray dns_array;
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));
            dns_array.append(dns_to_json(item));
        }
        document.setArray(dns_array);
        clipboardData.append(document.toJson());
    }
        break;

    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                clipboardData.append(item->CNAME->child(j, 1)->text().append(NEWLINE));
            for(int j = 0; j < item->NS->rowCount(); j++)
                clipboardData.append(item->NS->child(j, 1)->text().append(NEWLINE));
            for(int j = 0; j < item->MX->rowCount(); j++)
                clipboardData.append(item->MX->child(j, 1)->text().append(NEWLINE));
        }
        break;

    case RESULT_TYPE::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->A->rowCount(); j++)
                clipboardData.append(item->A->child(j, 1)->text().append(NEWLINE));
            for(int j = 0; j < item->AAAA->rowCount(); j++)
                clipboardData.append(item->AAAA->child(j, 1)->text().append(NEWLINE));
        }
        break;

    case RESULT_TYPE::A:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->A->rowCount(); j++)
                clipboardData.append(item->A->child(j, 1)->text().append(NEWLINE));
        }
        break;

    case RESULT_TYPE::AAAA:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->AAAA->rowCount(); j++)
                clipboardData.append(item->AAAA->child(j, 1)->text().append(NEWLINE));
        }
        break;

    case RESULT_TYPE::NS:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->NS->rowCount(); j++)
                clipboardData.append(item->NS->child(j, 1)->text().append(NEWLINE));
        }
        break;

    case RESULT_TYPE::MX:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->MX->rowCount(); j++)
                clipboardData.append(item->MX->child(j, 1)->text().append(NEWLINE));
        }
        break;

    case RESULT_TYPE::CNAME:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                clipboardData.append(item->CNAME->child(j, 1)->text().append(NEWLINE));
        }
        break;

    case RESULT_TYPE::TXT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->TXT->rowCount(); j++)
                clipboardData.append(item->TXT->child(j, 1)->text().append(NEWLINE));
        }
        break;

    default:
        break;
    }

    clipboard->setText(clipboardData.trimmed());
}

void Dns::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        data.append(index.data().toString().append(NEWLINE));

    clipboard->setText(data.trimmed());
}

///
/// sending results...
///

void Dns::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));
        project->addActiveDNS(dns_to_struct(item));
    }
}

void Dns::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(!(index.parent() == m_model->invisibleRootItem()->index()))
            continue;
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));
        project->addActiveDNS(dns_to_struct(item));
    }
}

void Dns::sendToEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
    switch (engine) {
    case ENGINE::OSINT:
        if(result_type == RESULT_TYPE::IP) {
            for(int i = 0; i < proxyModel->rowCount(); i++){
                QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
                s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

                for(int j = 0; j < item->A->rowCount(); j++)
                    emit sendResultsToOsint(item->A->child(j, 1)->text(), result_type);
                for(int j = 0; j < item->AAAA->rowCount(); j++)
                    emit sendResultsToOsint(item->AAAA->child(j, 1)->text(), result_type);
            }
            emit changeTabToOsint();
        }
        if(result_type == RESULT_TYPE::SUBDOMAIN) {
            for(int i = 0; i < proxyModel->rowCount(); i++){
                QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
                s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

                for(int j = 0; j < item->CNAME->rowCount(); j++)
                    emit sendResultsToOsint(item->CNAME->child(j, 1)->text(), result_type);
                for(int j = 0; j < item->NS->rowCount(); j++)
                    emit sendResultsToOsint(item->NS->child(j, 1)->text(), result_type);
                for(int j = 0; j < item->MX->rowCount(); j++)
                    emit sendResultsToOsint(item->MX->child(j, 1)->text(), result_type);
            }
            emit changeTabToOsint();
        }
        break;
    case ENGINE::RAW:
        if(result_type == RESULT_TYPE::IP) {
            for(int i = 0; i < proxyModel->rowCount(); i++){
                QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
                s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

                for(int j = 0; j < item->A->rowCount(); j++)
                    emit sendResultsToRaw(item->A->child(j, 1)->text(), result_type);
                for(int j = 0; j < item->AAAA->rowCount(); j++)
                    emit sendResultsToRaw(item->AAAA->child(j, 1)->text(), result_type);
            }
            emit changeTabToRaw();
        }
        if(result_type == RESULT_TYPE::SUBDOMAIN) {
            for(int i = 0; i < proxyModel->rowCount(); i++){
                QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
                s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

                for(int j = 0; j < item->CNAME->rowCount(); j++)
                    emit sendResultsToRaw(item->CNAME->child(j, 1)->text(), result_type);
                for(int j = 0; j < item->NS->rowCount(); j++)
                    emit sendResultsToRaw(item->NS->child(j, 1)->text(), result_type);
                for(int j = 0; j < item->MX->rowCount(); j++)
                    emit sendResultsToRaw(item->MX->child(j, 1)->text(), result_type);
            }
            emit changeTabToRaw();
        }
        break;

    case ENGINE::BRUTE:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                emit sendResultsToBrute(item->CNAME->child(j, 1)->text(), result_type);
            for(int j = 0; j < item->NS->rowCount(); j++)
                emit sendResultsToBrute(item->NS->child(j, 1)->text(), result_type);
            for(int j = 0; j < item->MX->rowCount(); j++)
                emit sendResultsToBrute(item->MX->child(j, 1)->text(), result_type);
        }
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                emit sendResultsToActive(item->CNAME->child(j, 1)->text(), result_type);
            for(int j = 0; j < item->NS->rowCount(); j++)
                emit sendResultsToActive(item->NS->child(j, 1)->text(), result_type);
            for(int j = 0; j < item->MX->rowCount(); j++)
                emit sendResultsToActive(item->MX->child(j, 1)->text(), result_type);
        }
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                emit sendResultsToDns(item->CNAME->child(j, 1)->text(), result_type);
            for(int j = 0; j < item->NS->rowCount(); j++)
                emit sendResultsToDns(item->NS->child(j, 1)->text(), result_type);
            for(int j = 0; j < item->MX->rowCount(); j++)
                emit sendResultsToDns(item->MX->child(j, 1)->text(), result_type);
        }
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                emit sendResultsToCert(item->CNAME->child(j, 1)->text(), result_type);
            for(int j = 0; j < item->NS->rowCount(); j++)
                emit sendResultsToCert(item->NS->child(j, 1)->text(), result_type);
            for(int j = 0; j < item->MX->rowCount(); j++)
                emit sendResultsToCert(item->MX->child(j, 1)->text(), result_type);
        }
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Dns::sendSelectedToEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                emit sendResultsToOsint(index.data().toString(), result_type);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(!index.column())
                emit sendResultsToRaw(index.data().toString(), result_type);
        }
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                emit sendResultsToBrute(index.data().toString(), result_type);
        }
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                emit sendResultsToActive(index.data().toString(), result_type);
        }
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                emit sendResultsToDns(index.data().toString(), result_type);
        }
        emit changeTabToDns();
        break;
    case ENGINE::CERT:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                emit sendResultsToCert(index.data().toString(), result_type);
        }
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Dns::sendToEnum(const TOOL &tool){
    switch (tool) {
    case TOOL::IP:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

            for(int j = 0; j < item->A->rowCount(); j++)
                emit sendResultsToIpEnum(item->A->child(j, 1)->text(), RESULT_TYPE::IP);
            for(int j = 0; j < item->AAAA->rowCount(); j++)
                emit sendResultsToIpEnum(item->AAAA->child(j, 1)->text(), RESULT_TYPE::IP);
        }
        emit changeTabToIpEnum();
        break;
    case TOOL::NS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

            for(int j = 0; j < item->NS->rowCount(); j++)
                emit sendResultsToNSEnum(item->NS->child(j, 1)->text(), RESULT_TYPE::NS);
        }
        emit changeTabToNSEnum();
        break;
    case TOOL::MX:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

            for(int j = 0; j < item->MX->rowCount(); j++)
                emit sendResultsToMXEnum(item->MX->child(j, 1)->text(), RESULT_TYPE::MX);
        }
        emit changeTabToMXEnum();
        break;
    default:
        break;
    }
}

void Dns::sendSelectedToEnum(const TOOL &tool){
    switch (tool) {
    case TOOL::IP:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                emit sendResultsToIpEnum(index.data().toString(), RESULT_TYPE::IP);
        }
        emit changeTabToIpEnum();
        break;
    case TOOL::NS:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                emit sendResultsToNSEnum(index.data().toString(), RESULT_TYPE::NS);
        }
        emit changeTabToNSEnum();
        break;
    case TOOL::MX:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                emit sendResultsToMXEnum(index.data().toString(), RESULT_TYPE::MX);
        }
        emit changeTabToMXEnum();
        break;
    default:
        break;
    }

}

///
/// receiving targets...
///

void Dns::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::SUBDOMAIN)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

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
    m_model->setHorizontalHeaderLabels({tr(" DNS"), tr(" Values"), tr(" Values")});
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

    QSet<QString> set;
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
        file.close();
    }
        return;

    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                set.insert(item->CNAME->child(j, 1)->text());
            for(int j = 0; j < item->NS->rowCount(); j++)
                set.insert(item->NS->child(j, 1)->text());
            for(int j = 0; j < item->MX->rowCount(); j++)
                set.insert(item->MX->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->A->rowCount(); j++)
                set.insert(item->A->child(j, 1)->text());
            for(int j = 0; j < item->AAAA->rowCount(); j++)
                set.insert(item->AAAA->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::A:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->A->rowCount(); j++)
                set.insert(item->A->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::AAAA:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->AAAA->rowCount(); j++)
                set.insert(item->AAAA->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::NS:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->NS->rowCount(); j++)
                set.insert(item->NS->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::MX:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->MX->rowCount(); j++)
                set.insert(item->MX->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::CNAME:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                set.insert(item->CNAME->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::TXT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->TXT->rowCount(); j++)
                set.insert(item->TXT->child(j, 1)->text());
        }
        break;

    default:
        break;
    }

    /* saving list to file */
    if(set.count()){
        foreach(const QString &target, set)
            file.write(target.toUtf8().append(NEWLINE));
    }else
        QMessageBox::warning(this, tr("Warning!"), tr("Nothing to save!"));

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

    QSet<QString> set;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        set.insert(index.data().toString());

    /* saving list to file */
    if(set.count()){
        foreach(const QString &target, set)
            file.write(target.toUtf8().append(NEWLINE));
    }else
        QMessageBox::warning(this, tr("Warning!"), tr("Nothing to save!"));

    file.close();
}


void Dns::copyResults(const RESULT_TYPE &result_type){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QSet<QString> set;
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
        clipboard->setText(document.toJson());
    }
        return;

    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                set.insert(item->CNAME->child(j, 1)->text());
            for(int j = 0; j < item->NS->rowCount(); j++)
                set.insert(item->NS->child(j, 1)->text());
            for(int j = 0; j < item->MX->rowCount(); j++)
                set.insert(item->MX->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::IP:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->A->rowCount(); j++)
                set.insert(item->A->child(j, 1)->text());
            for(int j = 0; j < item->AAAA->rowCount(); j++)
                set.insert(item->AAAA->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::A:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->A->rowCount(); j++)
                set.insert(item->A->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::AAAA:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->AAAA->rowCount(); j++)
                set.insert(item->AAAA->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::NS:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->NS->rowCount(); j++)
                set.insert(item->NS->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::MX:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->MX->rowCount(); j++)
                set.insert(item->MX->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::CNAME:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                set.insert(item->CNAME->child(j, 1)->text());
        }
        break;

    case RESULT_TYPE::TXT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(model_index));

            for(int j = 0; j < item->TXT->rowCount(); j++)
                set.insert(item->TXT->child(j, 1)->text());
        }
        break;

    default:
        break;
    }

    /* setting targets to clipdoard */
    QString clipboardData;
    foreach(const QString &target, set)
        clipboardData.append(target+NEWLINE);

    clipboard->setText(clipboardData.trimmed());
}

void Dns::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QSet<QString> set;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        set.insert(index.data().toString());

    /* setting targets to clipdoard */
    QString clipboardData;
    foreach(const QString &target, set)
        clipboardData.append(target+NEWLINE);

    clipboard->setText(clipboardData.trimmed());
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
    QSet<QString> targets;

    /* getting targets */
    switch (result_type) {
    case RESULT_TYPE::IP:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

            for(int j = 0; j < item->A->rowCount(); j++)
                targets.insert(item->A->child(j, 1)->text());
            for(int j = 0; j < item->AAAA->rowCount(); j++)
                targets.insert(item->A->child(j, 1)->text());
        }
        break;
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                targets.insert(item->CNAME->child(j, 1)->text());
            for(int j = 0; j < item->NS->rowCount(); j++)
                targets.insert(item->NS->child(j, 1)->text());
            for(int j = 0; j < item->MX->rowCount(); j++)
                targets.insert(item->MX->child(j, 1)->text());
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
    default:
        break;
    }
}

void Dns::sendSelectedToEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting targets */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column())
            targets.insert(index.data().toString());
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
    default:
        break;
    }
}

void Dns::sendToEnum(const ENUMERATOR &tool){
    QSet<QString> targets;

    /* getting & sending targets */
    switch (tool) {
    case ENUMERATOR::IP:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

            for(int j = 0; j < item->A->rowCount(); j++)
                targets.insert(item->A->child(j, 1)->text());
            for(int j = 0; j < item->AAAA->rowCount(); j++)
                targets.insert(item->AAAA->child(j, 1)->text());
        }
        emit sendToIpEnum(targets, RESULT_TYPE::IP);
        emit changeTabToIpEnum();
        break;

    case ENUMERATOR::NS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

            for(int j = 0; j < item->NS->rowCount(); j++)
                targets.insert(item->NS->child(j, 1)->text());
        }
        emit sendToNSEnum(targets, RESULT_TYPE::NS);
        emit changeTabToNSEnum();
        break;

    case ENUMERATOR::MX:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(m_model->itemFromIndex(index));

            for(int j = 0; j < item->MX->rowCount(); j++)
                targets.insert(item->MX->child(j, 1)->text());
        }
        emit sendToMXEnum(targets, RESULT_TYPE::MX);
        emit changeTabToMXEnum();
        break;

    default:
        break;
    }
}

void Dns::sendSelectedToEnum(const ENUMERATOR &tool){
    QSet<QString> targets;

    /* getting targets */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column())
            targets.insert(index.data().toString());
    }

    /* sending targets */
    switch (tool) {
    case ENUMERATOR::IP:
        emit sendToIpEnum(targets, RESULT_TYPE::IP);
        emit changeTabToIpEnum();
        break;
    case ENUMERATOR::NS:
        emit sendToNSEnum(targets, RESULT_TYPE::NS);
        emit changeTabToNSEnum();
        break;
    case ENUMERATOR::MX:
        emit sendToMXEnum(targets, RESULT_TYPE::MX);
        emit changeTabToMXEnum();
        break;
    default:
        break;
    }
}

///
/// receiving targets...
///

void Dns::onReceiveTargets(QSet<QString> targets, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::SUBDOMAIN)
        ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

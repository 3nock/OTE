/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "SSLTool.h"
#include "ui_SSLTool.h"

#include <QClipboard>
#include <QDesktopServices>


void SSLTool::clearResults(){
    /* clear appropriate model */
    switch (ui->comboBoxOutput->currentIndex()) {
    case 0: // subdomain
        m_model_subdomain->clear();
        set_subdomain.clear();
        m_model_subdomain->setHorizontalHeaderLabels({tr(" Associated Names")});
        break;
    case 1: // cert hash
        m_model_hash->clear();
        set_hash.clear();
        m_model_hash->setHorizontalHeaderLabels({tr(" Certificate Hash")});
        break;
    case 2: // ssl
        m_model_ssl->clear();
        set_ssl.clear();
        m_model_ssl->setHorizontalHeaderLabels({tr(" ASN"), tr(" Values")});
    }

    /* clear the filter and the result count */
    ui->lineEditFilter->clear();
    ui->labelResultsCount->clear();
    ui->plainTextEditCert->clear();
    ui->plainTextEditKey->clear();

    /* clear the progressbar */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void SSLTool::openInBrowser(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void SSLTool::removeResults(){
    auto model_selectedIndexes = proxyModel->mapSelectionToSource(selectionModel->selection());
    QModelIndexList selectedIndexes = model_selectedIndexes.indexes();

    switch (ui->comboBoxOutput->currentIndex()) {
    case 0: // subdomain
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            set_subdomain.remove(i->data().toString());
            m_model_subdomain->removeRow(i->row());
        }
        break;
    case 1: // cert hash
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            set_hash.remove(i->data().toString());
            m_model_hash->removeRow(i->row());
        }
        break;
    case 2: // ssl
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            set_ssl.remove(i->data().toString());
            m_model_ssl->removeRow(i->row());
        }
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void SSLTool::saveResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "SSL: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "SSL: Failed to open " << filename << " For saving Results";
        return;
    }

    switch(ui->comboBoxOutput->currentIndex()){
    case 0: // Subddomains
    case 1: // SSL HASH
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            file.write(proxyModel->index(i, 0).data().toString().append(NEWLINE).toUtf8());
        break;

    case 2:
    {
        QJsonDocument document;
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model_ssl->itemFromIndex(model_index));
            array.append(ssl_to_json(item));
        }
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    default:
        break;
    }

    file.close();
}

void SSLTool::saveSelectedResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "SSL: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "SSL: Failed to open " << filename << " For saving Results";
        return;
    }

    switch(ui->comboBoxOutput->currentIndex()){
    case 0: // Subddomains
    case 1: // SSL HASH
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            file.write(index.data().toString().append(NEWLINE).toUtf8());
        break;

    case 2:
    {
        QJsonDocument document;
        QJsonArray array;

        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        {
            if(index.parent() == m_model_ssl->invisibleRootItem()->index()){
                QModelIndex model_index = proxyModel->mapToSource(index);
                s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model_ssl->itemFromIndex(model_index));
                array.append(ssl_to_json(item));
            }
            else
                file.write(index.data().toString().append(NEWLINE).toUtf8());
        }
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    default:
        break;
    }

    file.close();
}


void SSLTool::copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;

    switch(ui->comboBoxOutput->currentIndex()){
    case 0: // Subddomains
    case 1: // SSL HASH
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            clipboardData.append(proxyModel->index(i, 0).data().toString().append(NEWLINE));
        break;

    case 2:
    {
        QJsonDocument document;
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model_ssl->itemFromIndex(model_index));
            array.append(ssl_to_json(item));
        }
        if(!array.isEmpty()){
            document.setArray(array);
            clipboardData.append(document.toJson());
        }
    }
        break;
    default:
        break;
    }

    clipboard->setText(clipboardData.trimmed());
}

void SSLTool::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;

    switch(ui->comboBoxOutput->currentIndex()){
    case 0: // Subddomains
    case 1: // SSL HASH
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            clipboardData.append(index.data().toString().append(NEWLINE));
        break;

    case 2:
    {
        QJsonDocument document;
        QJsonArray array;

        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        {
            if(index.parent() == m_model_ssl->invisibleRootItem()->index()){
                QModelIndex model_index = proxyModel->mapToSource(index);
                s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model_ssl->itemFromIndex(model_index));
                array.append(ssl_to_json(item));
            }
            else
                clipboardData.append(index.data().toString().append(NEWLINE).toUtf8());
        }
        if(!array.isEmpty()){
            document.setArray(array);
            clipboardData.append(document.toJson());
        }
    }
        break;
    default:
        break;
    }

    clipboard->setText(clipboardData.trimmed());
}

///
/// sending results...
///

void SSLTool::sendToProject(){
    switch (ui->comboBoxOutput->currentIndex()) {
    case 0:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            project->addActiveSSL_altNames(proxyModel->index(i, 0).data().toString());
        break;
    case 1:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            project->addActiveSSL_hash(proxyModel->index(i, 0).data().toString());
        break;
    case 2:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model_ssl->itemFromIndex(model_index));
            foreach(const QSslCertificate &cert, QSslCertificate::fromData(item->raw, QSsl::Pem))
                project->addActiveSSL(item->text(), cert);
        }
        break;
    }
}

void SSLTool::sendSelectedToProject(){
    switch (ui->comboBoxOutput->currentIndex()) {
    case 0:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            project->addActiveSSL_altNames(index.data().toString());
        break;
    case 1:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            project->addActiveSSL_hash(index.data().toString());
        break;
    case 2:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        {
            if(index.parent() == m_model_ssl->invisibleRootItem()->index()){
                QModelIndex model_index = proxyModel->mapToSource(index);
                s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model_ssl->itemFromIndex(model_index));
                foreach(const QSslCertificate &cert, QSslCertificate::fromData(item->raw, QSsl::Pem))
                    project->addActiveSSL(item->text(), cert);
            }
        }
        break;
    }
}

void SSLTool::sendToEngine(const TOOL &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting targets */
    for(int i = 0; i != proxyModel->rowCount(); ++i)
        targets.insert(proxyModel->index(i, 0).data().toString());

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

void SSLTool::sendSelectedToEngine(const TOOL &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting targets */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        targets.insert(index.data().toString());

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

void SSLTool::sendToEnum(const ENUMERATOR &tool){
    QSet<QString> targets;

    /* getting targets */
    if(ui->comboBoxOutput->currentIndex() == 1){
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            targets.insert(proxyModel->index(i, 0).data().toString());
    }

    /* sending targets */
    if(tool == ENUMERATOR::SSL){
        emit sendToSSLEnum(targets, RESULT_TYPE::CERT_ID);
        emit changeTabToSSLEnum();
    }
}

void SSLTool::sendSelectedToEnum(const ENUMERATOR &tool){
    QSet<QString> targets;

    /* getting targets */
    if(ui->comboBoxOutput->currentIndex() == 1){
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            targets.insert(index.data().toString());
    }

    /* sending targets */
    if(tool == ENUMERATOR::SSL){
        emit sendToSSLEnum(targets, RESULT_TYPE::CERT_ID);
        emit changeTabToSSLEnum();
    }
}

///
/// receiving targets...
///

void SSLTool::onReceiveTargets(QSet<QString> targets, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::SUBDOMAIN)
        ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

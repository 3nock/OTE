/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Ssl.h"
#include "ui_Ssl.h"

#include <QClipboard>
#include <QDesktopServices>

/*
 * TODO:
 *      insert the send/copy/save targets to set before appending to clipboard or file
 */
void Ssl::clearResults(){
    /* clear appropriate model */
    switch (ui->comboBoxOutput->currentIndex()) {
    case 0: // subdomain
        m_model_subdomain->clear();
        set_subdomain.clear();
        m_model_subdomain->setHorizontalHeaderLabels({tr(" Alternative Names")});
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

void Ssl::openInBrowser(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Ssl::removeResults(){
    switch (ui->comboBoxOutput->currentIndex()) {
    case 0: // subdomain
        foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            set_subdomain.remove(index.data().toString());
            m_model_subdomain->removeRow(index.row());
        }
        break;
    case 1: // cert hash
        foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            set_hash.remove(index.data().toString());
            m_model_hash->removeRow(index.row());
        }
        break;
    case 2: // ssl
        foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            set_ssl.remove(index.data().toString());
            m_model_ssl->removeRow(index.row());
        }
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void Ssl::saveResults(){
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

void Ssl::saveSelectedResults(){
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


void Ssl::copyResults(){
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

void Ssl::copySelectedResults(){
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

void Ssl::sendToProject(){
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
            project->addActiveSSL(item->raw);
        }
        break;
    }
}

void Ssl::sendSelectedToProject(){
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
                project->addActiveSSL(item->raw);
            }
        }
        break;
    }
}

void Ssl::sendToEngine(const ENGINE &engine){
    if(ui->comboBoxOutput->currentIndex() == 0){
        switch (engine) {
        case ENGINE::OSINT:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                emit sendResultsToOsint(proxyModel->index(i, 0).data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToOsint();
            break;
        case ENGINE::RAW:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                emit sendResultsToRaw(proxyModel->index(i, 0).data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToRaw();
            break;
        case ENGINE::BRUTE:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                emit sendResultsToBrute(proxyModel->index(i, 0).data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToBrute();
            break;
        case ENGINE::ACTIVE:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                emit sendResultsToActive(proxyModel->index(i, 0).data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToActive();
            break;
        case ENGINE::DNS:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                emit sendResultsToDns(proxyModel->index(i, 0).data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToDns();
            break;
        case ENGINE::CERT:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                emit sendResultsToCert(proxyModel->index(i, 0).data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToSSL();
            break;
        case ENGINE::URL:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                emit sendResultsToUrl(proxyModel->index(i, 0).data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToURL();
            break;
        default:
            break;
        }
    }

    if(ui->comboBoxOutput->currentIndex() == 1){
        switch (engine) {
        case ENGINE::OSINT:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                emit sendResultsToOsint(proxyModel->index(i, 0).data().toString(), RESULT_TYPE::CERT_ID);
            emit changeTabToOsint();
            break;
        case ENGINE::RAW:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                emit sendResultsToRaw(proxyModel->index(i, 0).data().toString(), RESULT_TYPE::CERT_ID);
            emit changeTabToRaw();
            break;
        default:
            break;
        }
    }
}

void Ssl::sendSelectedToEngine(const ENGINE &engine){
    if(ui->comboBoxOutput->currentIndex() == 0){
        switch (engine) {
        case ENGINE::OSINT:
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                emit sendResultsToOsint(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToOsint();
            break;
        case ENGINE::RAW:
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                emit sendResultsToRaw(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToRaw();
            break;
        case ENGINE::BRUTE:
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                emit sendResultsToBrute(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToBrute();
            break;
        case ENGINE::ACTIVE:
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                emit sendResultsToActive(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToActive();
            break;
        case ENGINE::DNS:
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                emit sendResultsToDns(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToDns();
            break;
        case ENGINE::CERT:
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                emit sendResultsToCert(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToSSL();
            break;
        case ENGINE::URL:
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                emit sendResultsToUrl(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
            emit changeTabToURL();
            break;
        default:
            break;
        }
    }

    if(ui->comboBoxOutput->currentIndex() == 1){
        switch (engine) {
        case ENGINE::OSINT:
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                emit sendResultsToOsint(index.data().toString(), RESULT_TYPE::CERT_ID);
            emit changeTabToOsint();
            break;
        case ENGINE::RAW:
            foreach(const QModelIndex &index, selectionModel->selectedIndexes())
                emit sendResultsToRaw(index.data().toString(), RESULT_TYPE::CERT_ID);
            emit changeTabToRaw();
            break;
        default:
            break;
        }
    }
}

void Ssl::sendToEnum(const TOOL &tool){
    Q_UNUSED(tool);
    if(!(ui->comboBoxOutput->currentIndex() == 1))
        return;

    for(int i = 0; i != proxyModel->rowCount(); ++i)
        emit sendResultsToSSLEnum(proxyModel->index(i, 0).data().toString(), RESULT_TYPE::CERT_ID);
    emit changeTabToSSLEnum();
}

void Ssl::sendSelectedToEnum(const TOOL &tool){
    Q_UNUSED(tool);
    if(!(ui->comboBoxOutput->currentIndex() == 1))
        return;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        emit sendResultsToSSLEnum(index.data().toString(), RESULT_TYPE::CERT_ID);
    emit changeTabToSSLEnum();
}

///
/// receiving targets...
///

void Ssl::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::SUBDOMAIN)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

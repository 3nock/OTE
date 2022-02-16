/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Raw.h"
#include "ui_Raw.h"

#include <QClipboard>
#include <QDesktopServices>


void Raw::openInBrowser(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Raw::clearResults(){
    m_model->clear();
    m_model->setHorizontalHeaderLabels({tr(" Properties"), tr(" Values")});
    ui->labelResultsCountTree->clear();

    /* clear the progressbar */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Raw::clearResults_txt(){
    ui->plainTextEditResults->clear();
    ui->labelResultsCount->clear();
    m_resultsCountJson = 0;
}

void Raw::removeResults(){
    QModelIndex index;
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        index = proxyModel->mapToSource(proxyIndex);
        m_model->removeRow(index.row());
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void Raw::saveResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "RAW: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    if(!file.isOpen()){
        qDebug() << "RAW: Failed to open " << filename << " For saving Results";
        return;
    }

    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::RAW *item = static_cast<s3s_item::RAW*>(m_model->item(index.row(), index.column()));
        file.write(item->json);
        file.write(NEWLINE);
    }
    file.close();
}

void Raw::saveSelectedResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "RAW: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    if(!file.isOpen()){
        qDebug() << "RAW: Failed to open " << filename << " For saving Results";
        return;
    }

    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes())
    {
        QModelIndex index = proxyModel->mapToSource(proxyIndex);
        s3s_item::RAW *item = static_cast<s3s_item::RAW*>(m_model->item(index.row(), index.column()));
        file.write(item->json);
        file.write(NEWLINE);
    }
    file.close();
}

void Raw::saveResults_txt(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "RAW: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "RAW: Failed to open " << filename << " For saving Results";
        return;
    }

    file.write(ui->plainTextEditResults->toPlainText().toUtf8());
    file.close();
}

void Raw::copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;

    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::RAW *item = static_cast<s3s_item::RAW*>(m_model->item(index.row(), index.column()));
        data.append(item->json);
        data.append(NEWLINE);
    }

    clipboard->setText(data.trimmed());
}

void Raw::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;

    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes())
    {
        QModelIndex index = proxyModel->mapToSource(proxyIndex);
        s3s_item::RAW *item = static_cast<s3s_item::RAW*>(m_model->item(index.row(), index.column()));
        data.append(item->json);
        data.append(NEWLINE);
    }

    clipboard->setText(data.trimmed());
}

void Raw::copyResults_txt(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->plainTextEditResults->toPlainText());
}

///
/// sending results...
///
void Raw::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::RAW *item = static_cast<s3s_item::RAW*>(m_model->item(index.row(), index.column()));
        project->addRaw(raw_to_struct(item));
    }
}

void Raw::sendToEngine(){

}

void Raw::sendToEnum(){

}

void Raw::sendSelectedToProject(){
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes())
    {
        QModelIndex index = proxyModel->mapToSource(proxyIndex);
        s3s_item::RAW *item = static_cast<s3s_item::RAW*>(m_model->item(index.row(), index.column()));
        project->addRaw(raw_to_struct(item));
    }
}

void Raw::sendSelectedToEngine(){

}

void Raw::sendSelectedToEnum(){

}

///
/// receive targets...
///
void Raw::onReceiveTargets(QString target, RESULT_TYPE result_type){
    switch (result_type) {
    case RESULT_TYPE::SUBDOMAIN:
        if(m_targetListModel_host->insertRow(m_targetListModel_host->rowCount()))
            m_targetListModel_host->setData(m_targetListModel_host->index(m_targetListModel_host->rowCount()-1, 0), target);
        break;
    case RESULT_TYPE::IP:
        if(m_targetListModel_ip->insertRow(m_targetListModel_ip->rowCount()))
            m_targetListModel_ip->setData(m_targetListModel_ip->index(m_targetListModel_ip->rowCount()-1, 0), target);
        break;
    case RESULT_TYPE::ASN:
        if(m_targetListModel_asn->insertRow(m_targetListModel_asn->rowCount()))
            m_targetListModel_asn->setData(m_targetListModel_asn->index(m_targetListModel_asn->rowCount()-1, 0), target);
        break;
    case RESULT_TYPE::CIDR:
        if(m_targetListModel_cidr->insertRow(m_targetListModel_cidr->rowCount()))
            m_targetListModel_cidr->setData(m_targetListModel_cidr->index(m_targetListModel_cidr->rowCount()-1, 0), target);
        break;
    case RESULT_TYPE::CERT_ID:
        if(m_targetListModel_ssl->insertRow(m_targetListModel_ssl->rowCount()))
            m_targetListModel_ssl->setData(m_targetListModel_ssl->index(m_targetListModel_ssl->rowCount()-1, 0), target);
        break;
    case RESULT_TYPE::URL:
        if(m_targetListModel_url->insertRow(m_targetListModel_url->rowCount()))
            m_targetListModel_url->setData(m_targetListModel_url->index(m_targetListModel_url->rowCount()-1, 0), target);
        break;
    case RESULT_TYPE::EMAIL:
        if(m_targetListModel_email->insertRow(m_targetListModel_email->rowCount()))
            m_targetListModel_email->setData(m_targetListModel_email->index(m_targetListModel_email->rowCount()-1, 0), target);
        break;
    default:
        break;
    }
}

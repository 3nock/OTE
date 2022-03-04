/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Url.h"
#include "ui_Url.h"

#include <QClipboard>
#include <QFileDialog>
#include <QDesktopServices>


void Url::openInBrowser(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl(index.data().toString(), QUrl::TolerantMode));
}

void Url::clearResults(){
    /* clear the results */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({tr(" URL"), tr(" Status code"), tr(" banner"), tr(" Content type")});
    set_results.clear();

    ui->tableViewResults->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableViewResults->horizontalHeader()->resizeSection(0, 230);
    ui->tableViewResults->horizontalHeader()->resizeSection(1, 10);
    ui->tableViewResults->horizontalHeader()->resizeSection(2, 120);

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Url::extract(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QSet<QString> extracts;

    /* extracting and saving to a set to avoid repeatition */
    for(int i = 0; i != proxyModel->rowCount(); ++i){
        QString url(proxyModel->index(i, 0).data().toString());
        QString domain = url.remove("https://").remove("http://").split("/").at(0);
        extracts.insert(domain);
    }

    /* setting the data to clipboard */
    QString data;
    foreach(const QString &extract, extracts)
        data.append(extract).append(NEWLINE);
    clipboard->setText(data.trimmed());
}

void Url::extractSelected(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QSet<QString> extracts;

    /* extracting and saving to a set to avoid repeatition */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column())
            continue;

        QString url(index.data().toString());
        QString domain = url.remove("https://").remove("http://").split("/").at(0);
        extracts.insert(domain);
    }

    /* setting the data to clipboard */
    QString data;
    foreach(const QString &extract, extracts)
        data.append(extract).append(NEWLINE);
    clipboard->setText(data.trimmed());
}

void Url::removeResults(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        set_results.remove(model_index.data().toString());
        m_model->removeRow(model_index.row());
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void Url::saveResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "URL: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "URL: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
        s3s_item::URL *item = static_cast<s3s_item::URL*>(m_model->itemFromIndex(model_index));
        array.append(url_to_json(item));
    }

    QJsonDocument document;
    document.setArray(array);
    file.write(document.toJson());

    file.close();
}

void Url::saveSelectedResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "URL: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "URL: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray array;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column()){
            file.write(index.data().toString().append(NEWLINE).toUtf8());
            continue;
        }

        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::URL *item = static_cast<s3s_item::URL*>(m_model->itemFromIndex(model_index));
        array.append(url_to_json(item));
    }

    if(!array.isEmpty()){
        QJsonDocument document;
        document.setArray(array);
        file.write(document.toJson());
    }
    file.close();
}


void Url::copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
        s3s_item::URL *item = static_cast<s3s_item::URL*>(m_model->itemFromIndex(model_index));
        array.append(url_to_json(item));
    }

    QJsonDocument document;
    document.setArray(array);
    clipboard->setText(document.toJson());
}

void Url::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray array;
    QString clipboardData;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column()){
            clipboardData.append(index.data().toString().append(NEWLINE));
            continue;
        }

        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::URL *item = static_cast<s3s_item::URL*>(m_model->itemFromIndex(model_index));
        array.append(url_to_json(item));
    }

    if(!array.isEmpty()){
        QJsonDocument document;
        document.setArray(array);
        clipboard->setText(document.toJson());
    }
    else
        clipboard->setText(clipboardData.trimmed());
}

///
/// sending results
///

void Url::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
        s3s_item::URL *item = static_cast<s3s_item::URL*>(m_model->itemFromIndex(model_index));
        project->addActiveURL(url_to_struct(item));
    }
}

void Url::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column())
            continue;

        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::URL *item = static_cast<s3s_item::URL*>(m_model->itemFromIndex(model_index));
        project->addActiveURL(url_to_struct(item));
    }
}

void Url::sendToEngine(const ENGINE &engine){
    switch (engine) {
    case ENGINE::URL:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToUrl(proxyModel->index(i, 0).data().toString(), RESULT_TYPE::URL);
        emit changeTabToURL();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            emit sendResultsToRaw(proxyModel->index(i, 0).data().toString(), RESULT_TYPE::URL);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Url::sendSelectedToEngine(const ENGINE &engine){
    switch (engine) {
    case ENGINE::URL:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                continue;
            emit sendResultsToUrl(index.data().toString(), RESULT_TYPE::URL);
        }
        emit changeTabToURL();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column())
                continue;
            emit sendResultsToRaw(index.data().toString(), RESULT_TYPE::URL);
        }
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

///
/// receiving targets
///

void Url::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::URL)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

void Url::onReceiveTargets(QSet<QString> targets, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::URL)
        ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

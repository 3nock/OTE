/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "URLTool.h"
#include "ui_URLTool.h"

#include <QClipboard>
#include <QFileDialog>
#include <QDesktopServices>


void URLTool::openInBrowser(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl(index.data().toString(), QUrl::TolerantMode));
}

void URLTool::clearResults(){
    /* clear the results */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({tr(" URL"), tr(" Status"), tr(" Title"), tr(" banner"), tr(" Content type")});
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

void URLTool::extract(){
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

void URLTool::extractSelected(){
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

void URLTool::removeResults(){
    auto model_selectedIndexes = proxyModel->mapSelectionToSource(selectionModel->selection());
    QModelIndexList selectedIndexes = model_selectedIndexes.indexes();
    for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
        set_results.remove(i->data().toString());
        m_model->removeRow(i->row());
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void URLTool::saveResults(){
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

void URLTool::saveSelectedResults(){
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


void URLTool::copyResults(){
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

void URLTool::copySelectedResults(){
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

void URLTool::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
        s3s_item::URL *item = static_cast<s3s_item::URL*>(m_model->itemFromIndex(model_index));
        project->addActiveURL(url_to_struct(item));
    }
}

void URLTool::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column())
            continue;

        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::URL *item = static_cast<s3s_item::URL*>(m_model->itemFromIndex(model_index));
        project->addActiveURL(url_to_struct(item));
    }
}

void URLTool::sendToEngine(const TOOL &engine){
    QSet<QString> targets;

    /* getting targets */
    for(int i = 0; i != proxyModel->rowCount(); ++i)
        targets.insert(proxyModel->index(i, 0).data().toString());

    /* sending targets */
    switch (engine) {
    case TOOL::URL:
        emit sendToUrl(targets, RESULT_TYPE::URL);
        emit changeTabToURL();
        break;
    case TOOL::RAW:
        emit sendToRaw(targets, RESULT_TYPE::URL);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void URLTool::sendSelectedToEngine(const TOOL &engine){
    QSet<QString> targets;

    /* getting targets */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column())
            continue;
        targets.insert(index.data().toString());
    }

    /* sending targets */
    switch (engine) {
    case TOOL::URL:
        emit sendToUrl(targets, RESULT_TYPE::URL);
        emit changeTabToURL();
        break;
    case TOOL::RAW:
        emit sendToRaw(targets, RESULT_TYPE::URL);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

///
/// receiving targets
///

void URLTool::onReceiveTargets(QSet<QString> targets, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::URL || resultType == RESULT_TYPE::SUBDOMAIN)
        ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

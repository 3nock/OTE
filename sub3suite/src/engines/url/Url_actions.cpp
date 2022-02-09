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


void Url::m_openInBrowser(QItemSelectionModel *selectionModel){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Url::m_clearResults(){
    /* clear the results... */
    m_resultModel->clear();
    ui->labelResultsCount->clear();
    m_resultModel->setHorizontalHeaderLabels({"URLs"});
    m_activeDns.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Url::m_removeResults(QItemSelectionModel *selectionModel){
    QModelIndex index;
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        index = m_resultProxyModel->mapToSource(proxyIndex);
        m_activeDns.remove(index.data().toString());
        m_resultModel->removeRow(index.row());
    }

    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Url::m_saveResults(){
    /* checks... */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())
        return;

    /* writing to file */
    QString item;
    for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
    {
        item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
        file.write(item.toUtf8());
    }

    file.close();
}

void Url::m_saveResults(QItemSelectionModel *selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    /* saving to file */
    QString data;
    QString item;
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            data.append(item.append(NEWLINE));
        }
        file.write(data.toUtf8());
        file.close();
    }
}


void Url::m_copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QString item;

    /* copying result type */
    for(int i = 0; i != m_resultProxyModel->rowCount(); ++i)
    {
        item = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString().append(NEWLINE);
        clipboardData.append(item);
    }
    clipboard->setText(clipboardData.trimmed());
}

void Url::m_copyResults(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;
    QString item;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        data.append(item.append(NEWLINE));
    }

    clipboard->setText(data.trimmed());
}

void Url::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::URL){
        ui->targets->add(target);
    }

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

/*****************************************************************************
                            SENDING RESULTS
******************************************************************************/
void Url::m_sendToProject(){
    QString url;
    for(int i = 0; i != m_resultProxyModel->rowCount(); ++i){
        url = m_resultProxyModel->data(m_resultProxyModel->index(i, 0)).toString();
        /*project->addActiveURL(url);*/
    }
}

void Url::m_sendToProject(QItemSelectionModel *selection){
    QString url;
    QModelIndexList indexList(selection->selectedIndexes());

    for(int i = 0; i < indexList.size();){
        url = indexList.at(i).data().toString();
        /*project->addActiveURL(url);*/
    }
}

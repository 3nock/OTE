#include "SSLEnum.h"
#include "ui_SSLEnum.h"

#include <QDesktopServices>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


void SSLEnum::clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({tr(" SSL"), tr(" Value")});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void SSLEnum::openInBrowser(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void SSLEnum::removeResults(){
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        QModelIndex index = proxyModel->mapToSource(proxyIndex);
        m_resultsSet.remove(index.data().toString());
        m_model->removeRow(index.row());
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void SSLEnum::saveResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "SSL-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "SSL-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonDocument document;
    QJsonArray array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
        s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model->itemFromIndex(model_index));
        array.append(ssl_to_json(item));
    }
    document.setArray(array);
    file.write(document.toJson());

    file.close();
}

void SSLEnum::saveSelectedResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "SSL-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "SSL-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonDocument document;
    QJsonArray array;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.parent() == m_model->invisibleRootItem()->index()){
            QModelIndex model_index = proxyModel->mapToSource(index);
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model->itemFromIndex(model_index));
            array.append(ssl_to_json(item));
        }
        else
            file.write(index.data().toString().append(NEWLINE).toUtf8());
    }
    document.setArray(array);
    file.write(document.toJson());

    file.close();
}


void SSLEnum::copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;

    QJsonDocument document;
    QJsonArray array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
        s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model->itemFromIndex(model_index));
        array.append(ssl_to_json(item));
    }
    if(!array.isEmpty()){
        document.setArray(array);
        clipboardData.append(document.toJson());
    }

    clipboard->setText(clipboardData.trimmed());
}

void SSLEnum::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;

    QJsonDocument document;
    QJsonArray array;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.parent() == m_model->invisibleRootItem()->index()){
            QModelIndex model_index = proxyModel->mapToSource(index);
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model->itemFromIndex(model_index));
            array.append(ssl_to_json(item));
        }
        else
            clipboardData.append(index.data().toString().append(NEWLINE).toUtf8());
    }
    if(!array.isEmpty()){
        document.setArray(array);
        clipboardData.append(document.toJson());
    }

    clipboard->setText(clipboardData.trimmed());
}

///
/// sending results...
///

void SSLEnum::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
        s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model->itemFromIndex(model_index));
        project->addActiveSSL(item->raw);
    }
}

void SSLEnum::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.parent() == m_model->invisibleRootItem()->index()){
            QModelIndex model_index = proxyModel->mapToSource(index);
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model->itemFromIndex(model_index));
            project->addActiveSSL(item->raw);
        }
    }
}

void SSLEnum::sendToEngine(const ENGINE &engine){

}

void SSLEnum::sendSelectedToEngine(const ENGINE &engine){

}

///
/// receiving targets...
///

void SSLEnum::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::CERT_ID)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

void SSLEnum::onReceiveTargets(QSet<QString> targets, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::CERT_ID)
        ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

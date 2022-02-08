#include "SSLEnum.h"
#include "ui_SSLEnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


/* TODO:
 *     validate if selected index is ip...
 */

void SSLEnum::m_clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({"    SSL", "    Value"});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void SSLEnum::m_removeResults(QItemSelectionModel *selectionModel){
    /* loop to delete all selected items */
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes())
    {
        QModelIndex index = proxyModel->mapToSource(proxyIndex);

        /* remove entire ssl */
        if(index.parent() == m_model->invisibleRootItem()->index()){
            m_resultsSet.remove(index.data().toString());
            m_model->removeRow(index.row());
        }
        /* remove a certain row in the ssl item */
        else{
            m_model->removeRow(index.row());
        }
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void SSLEnum::m_saveResults(){
    /* checks... */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    if(!file.isOpen())
        return;

    QJsonArray ssl_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::SSL *ssl = static_cast<s3s_item::SSL*>(m_model->item(index.row(), index.column()));

        ssl_array.append(ssl_to_json(ssl));
    }

    QJsonDocument document;
    document.setArray(ssl_array);

    qDebug() << "Saving SSL results to File: " << file.fileName();
    file.write(document.toJson());
    file.close();
}

void SSLEnum::m_saveResults(QItemSelectionModel *selection){
    /* checks... */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    if(!file.isOpen())
        return;

    QJsonArray ssl_array;
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::SSL *ssl = static_cast<s3s_item::SSL*>(m_model->item(model_index.row(), model_index.column()));

        ssl_array.append(ssl_to_json(ssl));
    }

    QJsonDocument document;
    document.setArray(ssl_array);

    qDebug() << "Saving SSL results to File: " << file.fileName();
    file.write(document.toJson());
    file.close();
}

void SSLEnum::m_copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray ssl_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::SSL *ssl = static_cast<s3s_item::SSL*>(m_model->item(index.row(), index.column()));

        ssl_array.append(ssl_to_json(ssl));
    }

    QJsonDocument document;
    document.setArray(ssl_array);

    qDebug() << "Copying SSL results to clipboard...";
    clipboard->setText(document.toJson());
}

void SSLEnum::m_copyResults(QItemSelectionModel *selection){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray ssl_array;
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::SSL *ssl = static_cast<s3s_item::SSL*>(m_model->item(model_index.row(), model_index.column()));

        ssl_array.append(ssl_to_json(ssl));
    }

    QJsonDocument document;
    document.setArray(ssl_array);

    qDebug() << "Copying SSL results to clipboard...";
    clipboard->setText(document.toJson());
}

///
/// send all...
///

void SSLEnum::m_sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model->item(index.row(), index.column()));
        /*project->addEnumSSL(ssl_to_struct(item));*/
    }
}

///
/// send selected...
///

void SSLEnum::m_sendToProject(QItemSelectionModel *selection){
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::SSL *ssl = static_cast<s3s_item::SSL*>(m_model->item(model_index.row(), model_index.column()));
        /*project->addEnumSSL(ssl_to_struct(ssl));*/
    }
}

void SSLEnum::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::CERT_ID)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

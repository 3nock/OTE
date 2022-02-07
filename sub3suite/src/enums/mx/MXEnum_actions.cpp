#include "MXEnum.h"
#include "ui_MXEnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


/* TODO:
 *     validate if selected index is ip...
 */

void MXEnum::m_clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({"    MX", "    Value"});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void MXEnum::m_removeResults(QItemSelectionModel *selectionModel){
    /* loop to delete all selected items */
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes())
    {
        QModelIndex index = proxyModel->mapToSource(proxyIndex);

        /* remove entire mx */
        if(index.parent() == m_model->invisibleRootItem()->index()){
            m_resultsSet.remove(index.data().toString());
            m_model->removeRow(index.row());
        }
        /* remove a certain row in the mx item */
        else{
            m_model->removeRow(index.row());
        }
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void MXEnum::m_saveResults(){
    /* checks... */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    if(!file.isOpen())
        return;

    QJsonArray asn_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::MX *mx = static_cast<s3s_item::MX*>(m_model->item(index.row(), index.column()));

        asn_array.append(mx_to_json(mx));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Saving MX results to File: " << file.fileName();
    file.write(document.toJson());
    file.close();
}

void MXEnum::m_saveResults(QItemSelectionModel *selection){
    /* checks... */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    if(!file.isOpen())
        return;

    QJsonArray asn_array;
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::MX *mx = static_cast<s3s_item::MX*>(m_model->item(model_index.row(), model_index.column()));

        asn_array.append(mx_to_json(mx));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Saving MX results to File: " << file.fileName();
    file.write(document.toJson());
    file.close();
}

void MXEnum::m_copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray asn_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::MX *mx = static_cast<s3s_item::MX*>(m_model->item(index.row(), index.column()));

        asn_array.append(mx_to_json(mx));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Copying MX results to clipboard...";
    clipboard->setText(document.toJson());
}

void MXEnum::m_copyResults(QItemSelectionModel *selection){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray asn_array;
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::MX *mx = static_cast<s3s_item::MX*>(m_model->item(model_index.row(), model_index.column()));

        asn_array.append(mx_to_json(mx));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Copying MX results to clipboard...";
    clipboard->setText(document.toJson());
}

///
/// send all...
///

void MXEnum::m_sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::MX *item = static_cast<s3s_item::MX*>(m_model->item(index.row(), index.column()));
        project->addEnumMX(mx_to_struct(item));
    }
}

///
/// send selected...
///

void MXEnum::m_sendToProject(QItemSelectionModel *selection){
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::MX *mx = static_cast<s3s_item::MX*>(m_model->item(model_index.row(), model_index.column()));

        project->addEnumMX(mx_to_struct(mx));
    }
}

void MXEnum::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::MX)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

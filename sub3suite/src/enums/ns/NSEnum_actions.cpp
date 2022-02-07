#include "NSEnum.h"
#include "ui_NSEnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


/* TODO:
 *     validate if selected index is ip...
 */

void NSEnum::m_clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({"    NS", "    Value"});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void NSEnum::m_removeResults(QItemSelectionModel *selectionModel){
    /* loop to delete all selected items */
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes())
    {
        QModelIndex index = proxyModel->mapToSource(proxyIndex);

        /* remove entire ns */
        if(index.parent() == m_model->invisibleRootItem()->index()){
            m_resultsSet.remove(index.data().toString());
            m_model->removeRow(index.row());
        }
        /* remove a certain row in the ns item */
        else{
            m_model->removeRow(index.row());
        }
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void NSEnum::m_saveResults(){
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
        s3s_item::NS *ns = static_cast<s3s_item::NS*>(m_model->item(index.row(), index.column()));

        asn_array.append(ns_to_json(ns));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Saving NS results to File: " << file.fileName();
    file.write(document.toJson());
    file.close();
}

void NSEnum::m_saveResults(QItemSelectionModel *selection){
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
        s3s_item::NS *ns = static_cast<s3s_item::NS*>(m_model->item(model_index.row(), model_index.column()));

        asn_array.append(ns_to_json(ns));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Saving NS results to File: " << file.fileName();
    file.write(document.toJson());
    file.close();
}

void NSEnum::m_copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray asn_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::NS *ns = static_cast<s3s_item::NS*>(m_model->item(index.row(), index.column()));

        asn_array.append(ns_to_json(ns));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Copying NS results to clipboard...";
    clipboard->setText(document.toJson());
}

void NSEnum::m_copyResults(QItemSelectionModel *selection){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray asn_array;
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::NS *ns = static_cast<s3s_item::NS*>(m_model->item(model_index.row(), model_index.column()));

        asn_array.append(ns_to_json(ns));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Copying NS results to clipboard...";
    clipboard->setText(document.toJson());
}

///
/// send all...
///

void NSEnum::m_sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::NS *item = static_cast<s3s_item::NS*>(m_model->item(index.row(), index.column()));
        project->addEnumNS(ns_to_struct(item));
    }
}

///
/// send selected...
///

void NSEnum::m_sendToProject(QItemSelectionModel *selection){
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::NS *ns = static_cast<s3s_item::NS*>(m_model->item(model_index.row(), model_index.column()));

        project->addEnumNS(ns_to_struct(ns));
    }
}

void NSEnum::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::NS)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

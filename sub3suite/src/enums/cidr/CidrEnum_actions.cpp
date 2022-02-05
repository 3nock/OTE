#include "CidrEnum.h"
#include "ui_CidrEnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


/* TODO:
 *     validate if selected index is asn...
 *     sujest to save as json
 */

void CidrEnum::m_clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({"    CIDR", "    Value"});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void CidrEnum::m_removeResults(QItemSelectionModel *selectionModel){
    /* loop to delete all selected items */
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes())
    {
        QModelIndex index = proxyModel->mapToSource(proxyIndex);

        /* remove entire cidr */
        if(index.parent() == m_model->invisibleRootItem()->index()){
            m_resultsSet.remove(index.data().toString());
            m_model->removeRow(index.row());
        }
        /* remove a certain row in the cidr item */
        else{
            m_model->removeRow(index.row());
        }
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void CidrEnum::m_saveResults(){
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
        s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->item(index.row(), index.column()));

        asn_array.append(cidr_to_json(cidr));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Saving CIDR results to File: " << file.fileName();
    file.write(document.toJson());
    file.close();
}

void CidrEnum::m_saveResults(QItemSelectionModel *selection){
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
        s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->item(model_index.row(), model_index.column()));

        asn_array.append(cidr_to_json(cidr));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Saving CIDR results to File: " << file.fileName();
    file.write(document.toJson());
    file.close();
}

void CidrEnum::m_copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray asn_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->item(index.row(), index.column()));

        asn_array.append(cidr_to_json(cidr));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Copying CIDR results to clipboard...";
    clipboard->setText(document.toJson());
}

void CidrEnum::m_copyResults(QItemSelectionModel *selection){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray asn_array;
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->item(model_index.row(), model_index.column()));

        asn_array.append(cidr_to_json(cidr));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Copying ASN results to clipboard...";
    clipboard->setText(document.toJson());
}

///
/// send all...
///

void CidrEnum::m_sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::CIDR *item = static_cast<s3s_item::CIDR*>(m_model->item(index.row(), index.column()));
        project->addEnumCIDR(cidr_to_struct(item));
    }
}

void CidrEnum::m_sendASNToEngine(ENGINE engine){
    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->item(index.row(), index.column()));

            for(int j = 0; j < cidr->asns->rowCount(); j++)
                emit sendResultsToOsint(cidr->asns->child(j, 1)->text(), RESULT_TYPE::ASN);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->item(index.row(), index.column()));

            for(int j = 0; j < cidr->asns->rowCount(); j++)
                emit sendResultsToRaw(cidr->asns->child(j, 1)->text(), RESULT_TYPE::ASN);
        }
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void CidrEnum::m_sendASNToEnum(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->item(index.row(), index.column()));

        for(int j = 0; j < cidr->asns->rowCount(); j++)
            emit sendResultsToAsnEnum(cidr->asns->child(j, 1)->text(), RESULT_TYPE::ASN);
    }
    emit changeTabToAsnEnum();
}

///
/// send selected...
///

void CidrEnum::m_sendToProject(QItemSelectionModel *selection){
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->item(model_index.row(), model_index.column()));

        project->addEnumCIDR(cidr_to_struct(cidr));
    }
}

void CidrEnum::m_sendASNToEngine(ENGINE engine, QItemSelectionModel *selection){
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            QModelIndex model_index = proxyModel->mapToSource(index);
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->item(model_index.row(), model_index.column()));

            for(int j = 0; j < cidr->asns->rowCount(); j++)
                emit sendResultsToOsint(cidr->asns->child(j, 1)->text(), RESULT_TYPE::ASN);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            QModelIndex model_index = proxyModel->mapToSource(index);
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->item(model_index.row(), model_index.column()));

            for(int j = 0; j < cidr->asns->rowCount(); j++)
                emit sendResultsToRaw(cidr->asns->child(j, 1)->text(), RESULT_TYPE::ASN);
        }
        emit changeTabToRaw();
        break;

    default:
        break;
    }
}

void CidrEnum::m_sendASNToEnum(QItemSelectionModel *selection){
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->item(model_index.row(), model_index.column()));

        for(int j = 0; j < cidr->asns->rowCount(); j++)
            emit sendResultsToAsnEnum(cidr->asns->child(j, 1)->text(), RESULT_TYPE::ASN);
    }
    emit changeTabToAsnEnum();
}

void CidrEnum::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::CIDR)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

#include "ASNEnum.h"
#include "ui_ASNEnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


/* TODO:
 *     validate if selected index is asn...
 */

void ASNEnum::m_clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({"    ASN", "    Value"});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void ASNEnum::m_removeResults(QItemSelectionModel *selectionModel){
    /* loop to delete all selected items */
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes())
    {
        QModelIndex index = proxyModel->mapToSource(proxyIndex);

        /* remove entire asn */
        if(index.parent() == m_model->invisibleRootItem()->index()){
            m_resultsSet.remove(index.data().toString());
            m_model->removeRow(index.row());
        }
        /* remove a certain row in the asn item */
        else{
            m_model->removeRow(index.row());
        }
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void ASNEnum::m_saveResults(){
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
        s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(index.row(), index.column()));

        asn_array.append(asn_to_json(asn));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Saving ASN results to File: " << file.fileName();
    file.write(document.toJson());
    file.close();
}

void ASNEnum::m_saveResults(QItemSelectionModel *selection){
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
        s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(model_index.row(), model_index.column()));

        asn_array.append(asn_to_json(asn));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Saving ASN results to File: " << file.fileName();
    file.write(document.toJson());
    file.close();
}

void ASNEnum::m_copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray asn_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(index.row(), index.column()));

        asn_array.append(asn_to_json(asn));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Copying ASN results to clipboard...";
    clipboard->setText(document.toJson());
}

void ASNEnum::m_copyResults(QItemSelectionModel *selection){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray asn_array;
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(model_index.row(), model_index.column()));

        asn_array.append(asn_to_json(asn));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "Copying ASN results to clipboard...";
    clipboard->setText(document.toJson());
}

///
/// send all...
///

void ASNEnum::m_sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::ASN *asn_item = static_cast<s3s_item::ASN*>(m_model->item(index.row(), index.column()));
        s3s_struct::ASN asn_struct = asn_to_struct(asn_item);
        project->addEnumASN(asn_struct);
    }
}

void ASNEnum::m_sendASNToEngine(ENGINE engine){
    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(index.row(), index.column()));

            for(int j = 0; j < asn->peers->rowCount(); j++)
                emit sendResultsToOsint(asn->peers->child(j, 1)->text(), RESULT_TYPE::ASN);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(index.row(), index.column()));

            for(int j = 0; j < asn->peers->rowCount(); j++)
                emit sendResultsToRaw(asn->peers->child(j, 1)->text(), RESULT_TYPE::ASN);
        }
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void ASNEnum::m_sendCIDRToEngine(ENGINE engine){
    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(index.row(), index.column()));

            for(int j = 0; j < asn->prefixes->rowCount(); j++)
                emit sendResultsToOsint(asn->prefixes->child(j, 1)->text(), RESULT_TYPE::CIDR);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(index.row(), index.column()));

            for(int j = 0; j < asn->prefixes->rowCount(); j++)
                emit sendResultsToRaw(asn->prefixes->child(j, 1)->text(), RESULT_TYPE::CIDR);
        }
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void ASNEnum::m_sendASNToEnum(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(index.row(), index.column()));

        for(int j = 0; j < asn->peers->rowCount(); j++)
            emit sendResultsToAsnEnum(asn->peers->child(j, 1)->text(), RESULT_TYPE::ASN);
    }
    emit changeTabToAsnEnum();
}

void ASNEnum::m_sendCIDRToEnum(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(index.row(), index.column()));

        for(int j = 0; j < asn->prefixes->rowCount(); j++)
            emit sendResultsToCidrEnum(asn->prefixes->child(j, 1)->text(), RESULT_TYPE::CIDR);
    }
    emit changeTabToCidrEnum();
}

///
/// send selected...
///

void ASNEnum::m_sendToProject(QItemSelectionModel *selection){
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(model_index.row(), model_index.column()));

        project->addEnumASN(asn_to_struct(asn));
    }
}

void ASNEnum::m_sendASNToEngine(ENGINE engine, QItemSelectionModel *selection){
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            QModelIndex model_index = proxyModel->mapToSource(index);
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(model_index.row(), model_index.column()));

            for(int j = 0; j < asn->peers->rowCount(); j++)
                emit sendResultsToOsint(asn->peers->child(j, 1)->text(), RESULT_TYPE::ASN);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            QModelIndex model_index = proxyModel->mapToSource(index);
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(model_index.row(), model_index.column()));

            for(int j = 0; j < asn->peers->rowCount(); j++)
                emit sendResultsToRaw(asn->peers->child(j, 1)->text(), RESULT_TYPE::ASN);
        }
        emit changeTabToRaw();
        break;

    default:
        break;
    }
}

void ASNEnum::m_sendCIDRToEngine(ENGINE engine, QItemSelectionModel *selection){
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            QModelIndex model_index = proxyModel->mapToSource(index);
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(model_index.row(), model_index.column()));

            for(int j = 0; j < asn->prefixes->rowCount(); j++)
                emit sendResultsToOsint(asn->prefixes->child(j, 1)->text(), RESULT_TYPE::CIDR);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selection->selectedIndexes()){
            QModelIndex model_index = proxyModel->mapToSource(index);
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(model_index.row(), model_index.column()));

            for(int j = 0; j < asn->prefixes->rowCount(); j++)
                emit sendResultsToRaw(asn->prefixes->child(j, 1)->text(), RESULT_TYPE::CIDR);
        }
        emit changeTabToRaw();
        break;

    default:
        break;
    }
}

void ASNEnum::m_sendASNToEnum(QItemSelectionModel *selection){
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(model_index.row(), model_index.column()));

        for(int j = 0; j < asn->peers->rowCount(); j++)
            emit sendResultsToAsnEnum(asn->peers->child(j, 1)->text(), RESULT_TYPE::ASN);
    }
    emit changeTabToAsnEnum();
}

void ASNEnum::m_sendCIDRToEnum(QItemSelectionModel *selection){
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->item(model_index.row(), model_index.column()));

        for(int j = 0; j < asn->prefixes->rowCount(); j++)
            emit sendResultsToCidrEnum(asn->prefixes->child(j, 1)->text(), RESULT_TYPE::CIDR);
    }
    emit changeTabToCidrEnum();
}

void ASNEnum::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::ASN)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

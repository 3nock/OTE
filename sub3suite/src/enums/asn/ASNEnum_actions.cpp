#include "ASNEnum.h"
#include "ui_ASNEnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


void ASNEnum::clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({tr(" ASN"), tr(" Value")});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void ASNEnum::removeResults(){
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

void ASNEnum::saveResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "ASN-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "ASN-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray asn_array;
    for(int i = 0; i < proxyModel->rowCount(); i++)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));
        asn_array.append(asn_to_json(asn));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    file.write(document.toJson());
    file.close();
}

void ASNEnum::saveSelectedResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "ASN-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "ASN-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray asn_array;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column() == 0){
            QModelIndex model_index = proxyModel->mapToSource(index);
            if(model_index.parent() == m_model->invisibleRootItem()->index()){
                s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(model_index));
                asn_array.append(asn_to_json(asn));
            }
        }else
            file.write(index.data().toString().append(NEWLINE).toUtf8());
    }

    if(!asn_array.isEmpty()){
        QJsonDocument document;
        document.setArray(asn_array);
        file.write(document.toJson());
    }
    file.close();
}

void ASNEnum::copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray asn_array;
    for(int i = 0; i < proxyModel->rowCount(); i++)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));
        asn_array.append(asn_to_json(asn));
    }

    QJsonDocument document;
    document.setArray(asn_array);

    qDebug() << "ASN-Enum: Copying ASN results to clipboard...";
    clipboard->setText(document.toJson());
}

void ASNEnum::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QJsonArray asn_array;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column() == 0){
            QModelIndex model_index = proxyModel->mapToSource(index);
            if(model_index.parent() == m_model->invisibleRootItem()->index()){
                s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(model_index));
                asn_array.append(asn_to_json(asn));
            }
        }else
            clipboardData.append(index.data().toString().append(NEWLINE));
    }

    qDebug() << "ASN-Enum: Copying ASN results to clipboard...";

    if(asn_array.isEmpty())
        clipboard->setText(clipboardData.trimmed());
    else {
        QJsonDocument document;
        document.setArray(asn_array);
        clipboard->setText(document.toJson());
    }
}

///
/// Sending Results
///

void ASNEnum::sendToProject(){
    for(int i = 0; i < proxyModel->rowCount(); i++)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i, 0));
        s3s_item::ASN *asn_item = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));
        project->addEnumASN(asn_to_struct(asn_item));
    }
}

void ASNEnum::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        QModelIndex model_index = proxyModel->mapToSource(index);
        if(model_index.parent() == m_model->invisibleRootItem()->index()){
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(model_index));
            project->addEnumASN(asn_to_struct(asn));
        }
    }
}

void ASNEnum::sendToEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i < proxyModel->rowCount(); i++)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));

            if(result_type == RESULT_TYPE::ASN){
                for(int j = 0; j < asn->peers->rowCount(); j++)
                    emit sendResultsToOsint(asn->peers->child(j, 1)->text(), result_type);
            }
            if(result_type == RESULT_TYPE::CIDR){
                for(int j = 0; j < asn->prefixes->rowCount(); j++)
                    emit sendResultsToOsint(asn->prefixes->child(j, 1)->text(), result_type);
            }
            if(result_type == RESULT_TYPE::EMAIL){
                for(int j = 0; j < asn->emailContacts->rowCount(); j++)
                    emit sendResultsToOsint(asn->emailContacts->child(j, 1)->text(), result_type);
                for(int j = 0; j < asn->abuseContacts->rowCount(); j++)
                    emit sendResultsToOsint(asn->abuseContacts->child(j, 1)->text(), result_type);
            }
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i < proxyModel->rowCount(); i++)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));

            if(result_type == RESULT_TYPE::ASN){
                for(int j = 0; j < asn->peers->rowCount(); j++)
                    emit sendResultsToRaw(asn->peers->child(j, 1)->text(), result_type);
            }
            if(result_type == RESULT_TYPE::CIDR){
                for(int j = 0; j < asn->prefixes->rowCount(); j++)
                    emit sendResultsToRaw(asn->prefixes->child(j, 1)->text(), result_type);
            }
            if(result_type == RESULT_TYPE::EMAIL){
                for(int j = 0; j < asn->emailContacts->rowCount(); j++)
                    emit sendResultsToRaw(asn->emailContacts->child(j, 1)->text(), result_type);
                for(int j = 0; j < asn->abuseContacts->rowCount(); j++)
                    emit sendResultsToRaw(asn->abuseContacts->child(j, 1)->text(), result_type);
            }
        }
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void ASNEnum::sendSelectedToEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToOsint(index.data().toString(), result_type);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToRaw(index.data().toString(), result_type);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void ASNEnum::sendToEnum(const TOOL &tool){
    switch (tool) {
    case TOOL::ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));

            for(int j = 0; j < asn->peers->rowCount(); j++)
                emit sendResultsToAsnEnum(asn->peers->child(j, 1)->text(), RESULT_TYPE::ASN);
        }
        emit changeTabToAsnEnum();
        break;
    case TOOL::CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));

            for(int j = 0; j < asn->prefixes->rowCount(); j++)
                emit sendResultsToCidrEnum(asn->prefixes->child(j, 1)->text(), RESULT_TYPE::CIDR);
        }
        emit changeTabToCidrEnum();
        break;
    case TOOL::EMAIL:
        for(int i = 0; i < proxyModel->rowCount(); i++)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));

            for(int j = 0; j < asn->emailContacts->rowCount(); j++)
                emit sendResultsToEmailEnum(asn->emailContacts->child(j, 1)->text(), RESULT_TYPE::EMAIL);
            for(int j = 0; j < asn->abuseContacts->rowCount(); j++)
                emit sendResultsToEmailEnum(asn->abuseContacts->child(j, 1)->text(), RESULT_TYPE::EMAIL);
        }
        emit changeTabToEmailEnum();
        break;
    default:
        break;
    }

}

void ASNEnum::sendSelectedToEnum(const TOOL &tool){
    switch (tool) {
    case TOOL::ASN:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToAsnEnum(index.data().toString(), RESULT_TYPE::ASN);
        emit changeTabToAsnEnum();
        break;
    case TOOL::CIDR:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToCidrEnum(index.data().toString(), RESULT_TYPE::CIDR);
        emit changeTabToCidrEnum();
        break;
    case TOOL::EMAIL:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToEmailEnum(index.data().toString(), RESULT_TYPE::EMAIL);
        emit changeTabToEmailEnum();
        break;
    default:
        break;
    }
}

///
/// Receiving Targets
///

void ASNEnum::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::ASN)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

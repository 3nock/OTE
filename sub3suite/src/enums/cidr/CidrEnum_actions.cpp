#include "CidrEnum.h"
#include "ui_CidrEnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


/* TODO:
 *     validate if selected index is cidr...
 */

void CidrEnum::clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({tr(" CIDR"), tr(" Value")});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void CidrEnum::removeResults(){
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

void CidrEnum::saveResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "CIDR-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "CIDR-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray cidr_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(index));
        cidr_array.append(cidr_to_json(cidr));
    }

    QJsonDocument document;
    document.setArray(cidr_array);

    file.write(document.toJson());
    file.close();
}

void CidrEnum::saveSelectedResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "CIDR-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "CIDR-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray cidr_array;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
        if(index.column() == 0){
            QModelIndex model_index = proxyModel->mapToSource(index);
            if(model_index.parent() == m_model->invisibleRootItem()->index()){
                s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(model_index));
                cidr_array.append(cidr_to_json(cidr));
            }
        }else
            file.write(index.data().toString().append(NEWLINE).toUtf8());
    }

    if(!cidr_array.isEmpty()){
        QJsonDocument document;
        document.setArray(cidr_array);
        file.write(document.toJson());
    }
    file.close();
}

void CidrEnum::copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray cidr_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(index));
        cidr_array.append(cidr_to_json(cidr));
    }

    QJsonDocument document;
    document.setArray(cidr_array);

    qDebug() << "CIDR-Enum: Copying CIDR results to clipboard...";
    clipboard->setText(document.toJson());
}

void CidrEnum::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QJsonArray cidr_array;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column() == 0){
            QModelIndex model_index = proxyModel->mapToSource(index);
            if(model_index.parent() == m_model->invisibleRootItem()->index()){
                s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(model_index));
                cidr_array.append(cidr_to_json(cidr));
            }
        }else
            clipboardData.append(index.data().toString().append(NEWLINE));
    }

    qDebug() << "CIDR-Enum: Copying CIDR results to clipboard...";

    if(cidr_array.isEmpty())
        clipboard->setText(clipboardData.trimmed());
    else {
        QJsonDocument document;
        document.setArray(cidr_array);
        clipboard->setText(document.toJson());
    }
}

///
/// Sending Results
///

void CidrEnum::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(index));
        project->addEnumCIDR(cidr_to_struct(cidr));
    }
}

void CidrEnum::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        QModelIndex model_index = proxyModel->mapToSource(index);
        if(model_index.parent() == m_model->invisibleRootItem()->index()){
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(model_index));
            project->addEnumCIDR(cidr_to_struct(cidr));
        }
    }
}

void CidrEnum::sendToEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(index));

            if(result_type == RESULT_TYPE::ASN){
                for(int j = 0; j < cidr->asns->rowCount(); j++)
                    emit sendResultsToOsint(cidr->asns->child(j, 1)->text(), result_type);
            }
            if(result_type == RESULT_TYPE::EMAIL){
                for(int j = 0; j < cidr->emailContacts->rowCount(); j++)
                    emit sendResultsToOsint(cidr->emailContacts->child(j, 1)->text(), result_type);
                for(int j = 0; j < cidr->abuseContacts->rowCount(); j++)
                    emit sendResultsToOsint(cidr->abuseContacts->child(j, 1)->text(), result_type);
            }
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(index));

            if(result_type == RESULT_TYPE::ASN){
                for(int j = 0; j < cidr->asns->rowCount(); j++)
                    emit sendResultsToRaw(cidr->asns->child(j, 1)->text(), result_type);
            }
            if(result_type == RESULT_TYPE::EMAIL){
                for(int j = 0; j < cidr->emailContacts->rowCount(); j++)
                    emit sendResultsToRaw(cidr->emailContacts->child(j, 1)->text(), result_type);
                for(int j = 0; j < cidr->abuseContacts->rowCount(); j++)
                    emit sendResultsToRaw(cidr->abuseContacts->child(j, 1)->text(), result_type);
            }
        }
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void CidrEnum::sendSelectedToEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
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

void CidrEnum::sendToEnum(const TOOL &tool){
    switch (tool) {
    case TOOL::ASN:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(index));

            for(int j = 0; j < cidr->asns->rowCount(); j++)
                emit sendResultsToAsnEnum(cidr->asns->child(j, 1)->text(), RESULT_TYPE::ASN);
        }
        emit changeTabToAsnEnum();
        break;
    case TOOL::EMAIL:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
        {
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(index));

            for(int j = 0; j < cidr->emailContacts->rowCount(); j++)
                emit sendResultsToEmailEnum(cidr->emailContacts->child(j, 1)->text(), RESULT_TYPE::EMAIL);
            for(int j = 0; j < cidr->abuseContacts->rowCount(); j++)
                emit sendResultsToEmailEnum(cidr->abuseContacts->child(j, 1)->text(), RESULT_TYPE::EMAIL);
        }
        emit changeTabToEmailEnum();
        break;
    default:
        break;
    }

}

void CidrEnum::sendSelectedToEnum(const TOOL &tool){
    switch (tool) {
    case TOOL::ASN:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToAsnEnum(index.data().toString(), RESULT_TYPE::ASN);
        emit changeTabToAsnEnum();
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

void CidrEnum::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::CIDR)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

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
    QSet<QString> targets;

    /* getting targets */
    switch (result_type) {
    case RESULT_TYPE::ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));

            for(int j = 0; j < asn->peers->rowCount(); j++)
                targets.insert(asn->peers->child(j, 1)->text());
        }
        break;
    case RESULT_TYPE::CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));

            for(int j = 0; j < asn->prefixes->rowCount(); j++)
                targets.insert(asn->prefixes->child(j, 1)->text());
        }
        break;
    case RESULT_TYPE::EMAIL:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));

            for(int j = 0; j < asn->emailContacts->rowCount(); j++)
                targets.insert(asn->emailContacts->child(j, 1)->text());
            for(int j = 0; j < asn->abuseContacts->rowCount(); j++)
                targets.insert(asn->abuseContacts->child(j, 1)->text());
        }
        break;
    default:
        break;
    }

    /* sending targets */
    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(targets, result_type);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(targets, result_type);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void ASNEnum::sendSelectedToEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting targets */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        if(index.column())
            targets.insert(index.data().toString());

    /* sending targets */
    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(targets, result_type);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(targets, result_type);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void ASNEnum::sendToEnum(const ENUMERATOR &tool, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting targets */
    switch (result_type) {
    case RESULT_TYPE::ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));

            for(int j = 0; j < asn->peers->rowCount(); j++)
                targets.insert(asn->peers->child(j, 1)->text());
        }
        break;
    case RESULT_TYPE::CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));

            for(int j = 0; j < asn->prefixes->rowCount(); j++)
                targets.insert(asn->prefixes->child(j, 1)->text());
        }
        break;
    case RESULT_TYPE::EMAIL:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::ASN *asn = static_cast<s3s_item::ASN*>(m_model->itemFromIndex(index));

            for(int j = 0; j < asn->emailContacts->rowCount(); j++)
                targets.insert(asn->emailContacts->child(j, 1)->text());
            for(int j = 0; j < asn->abuseContacts->rowCount(); j++)
                targets.insert(asn->abuseContacts->child(j, 1)->text());
        }
        break;
    default:
        break;
    }

    /* sending targets */
    switch (tool) {
    case ENUMERATOR::ASN:
        emit sendToAsnEnum(targets, result_type);
        emit changeTabToAsnEnum();
        break;
    case ENUMERATOR::CIDR:
        emit sendToCidrEnum(targets, result_type);
        emit changeTabToCidrEnum();
        break;
    case ENUMERATOR::EMAIL:
        emit sendToEmailEnum(targets, result_type);
        emit changeTabToEmailEnum();
        break;
    default:
        break;
    }
}

void ASNEnum::sendSelectedToEnum(const ENUMERATOR &tool, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting targets */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        if(index.column())
            targets.insert(index.data().toString());

    /* sending targets */
    switch (tool) {
    case ENUMERATOR::ASN:
        emit sendToAsnEnum(targets, result_type);
        emit changeTabToAsnEnum();
        break;
    case ENUMERATOR::CIDR:
        emit sendToCidrEnum(targets, result_type);
        emit changeTabToCidrEnum();
        break;
    case ENUMERATOR::EMAIL:
        emit sendToEmailEnum(targets, result_type);
        emit changeTabToEmailEnum();
        break;
    default:
        break;
    }
}

///
/// Receiving Targets
///

void ASNEnum::onReceiveTargets(QSet<QString> targets, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::ASN)
        ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

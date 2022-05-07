#include "CIDREnum.h"
#include "ui_CIDREnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


/* TODO:
 *     validate if selected index is cidr...
 */

void CIDREnum::clearResults(){
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

void CIDREnum::removeResults(){
    auto model_selectedIndexes = proxyModel->mapSelectionToSource(selectionModel->selection());
    QModelIndexList selectedIndexes = model_selectedIndexes.indexes();
    for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
        if(i->parent() == m_model->invisibleRootItem()->index()){
            m_resultsSet.remove(i->data().toString());
            m_model->removeRow(i->row());
        }
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void CIDREnum::saveResults(){
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

void CIDREnum::saveSelectedResults(){
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

void CIDREnum::copyResults(){
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

void CIDREnum::copySelectedResults(){
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

void CIDREnum::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(index));
        project->addEnumCIDR(cidr_to_struct(cidr));
    }
}

void CIDREnum::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        QModelIndex model_index = proxyModel->mapToSource(index);
        if(model_index.parent() == m_model->invisibleRootItem()->index()){
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(model_index));
            project->addEnumCIDR(cidr_to_struct(cidr));
        }
    }
}

void CIDREnum::sendToEngine(const TOOL &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting targets */
    switch (result_type) {
    case RESULT_TYPE::ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(index));

            for(int j = 0; j < cidr->asns->rowCount(); j++)
                targets.insert(cidr->asns->child(j, 0)->text());
        }
        break;
    case RESULT_TYPE::EMAIL:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(index));

            for(int j = 0; j < cidr->emailContacts->rowCount(); j++)
                targets.insert(cidr->emailContacts->child(j, 0)->text());
            for(int j = 0; j < cidr->abuseContacts->rowCount(); j++)
                targets.insert(cidr->abuseContacts->child(j, 0)->text());
        }
        break;
    default:
        break;
    }

    /* sending targets */
    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(targets, result_type);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(targets, result_type);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void CIDREnum::sendSelectedToEngine(const TOOL &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting targets */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        if(index.column())
            targets.insert(index.data().toString());

    /* sending targets */
    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(targets, result_type);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(targets, result_type);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void CIDREnum::sendToEnum(const ENUMERATOR &tool, const RESULT_TYPE &result_type){
    QSet<QString> targets;

    /* getting targets */
    switch (result_type) {
    case RESULT_TYPE::ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(index));

            for(int j = 0; j < cidr->asns->rowCount(); j++)
                targets.insert(cidr->asns->child(j, 0)->text());
        }
        break;
    case RESULT_TYPE::EMAIL:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
            s3s_item::CIDR *cidr = static_cast<s3s_item::CIDR*>(m_model->itemFromIndex(index));

            for(int j = 0; j < cidr->emailContacts->rowCount(); j++)
                targets.insert(cidr->emailContacts->child(j, 0)->text());
            for(int j = 0; j < cidr->abuseContacts->rowCount(); j++)
                targets.insert(cidr->abuseContacts->child(j, 0)->text());
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
    case ENUMERATOR::EMAIL:
        emit sendToEmailEnum(targets, result_type);
        emit changeTabToEmailEnum();
        break;
    default:
        break;
    }
}

void CIDREnum::sendSelectedToEnum(const ENUMERATOR &tool, const RESULT_TYPE &result_type){
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

void CIDREnum::onReceiveTargets(QSet<QString> targets, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::CIDR)
        ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

#include "NSEnum.h"
#include "ui_NSEnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


void NSEnum::clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({tr(" NS"), tr(" Value")});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void NSEnum::removeResults(){
    /* loop to delete all selected items */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        QModelIndex model_index = proxyModel->mapToSource(index);

        /* remove entire ns */
        if(model_index.parent() == m_model->invisibleRootItem()->index()){
            m_resultsSet.remove(model_index.data().toString());
            m_model->removeRow(model_index.row());
        }
        /* remove a certain row in the ns item */
        else{
            m_model->removeRow(model_index.row());
        }
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void NSEnum::saveResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "NS-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "NS-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray ns_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::NS *ns = static_cast<s3s_item::NS*>(m_model->itemFromIndex(index));
        ns_array.append(ns_to_json(ns));
    }

    QJsonDocument document;
    document.setArray(ns_array);
    file.write(document.toJson());
    file.close();
}

void NSEnum::saveSelectedResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "NS-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "NS-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray ns_array;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column())
            file.write(index.data().toString().append(NEWLINE).toUtf8());
        else {
            QModelIndex model_index = proxyModel->mapToSource(index);
            if(model_index.parent() == m_model->invisibleRootItem()->index()){
                s3s_item::NS *ns = static_cast<s3s_item::NS*>(m_model->itemFromIndex(model_index));
                ns_array.append(ns_to_json(ns));
            }
        }

    }

    if(!ns_array.isEmpty()){
        QJsonDocument document;
        document.setArray(ns_array);
        file.write(document.toJson());
    }
    file.close();
}

void NSEnum::copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray ns_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::NS *ns = static_cast<s3s_item::NS*>(m_model->itemFromIndex(index));
        ns_array.append(ns_to_json(ns));
    }

    qDebug() << "Copying Email results to clipboard...";

    QJsonDocument document;
    document.setArray(ns_array);
    clipboard->setText(document.toJson());
}

void NSEnum::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QJsonArray ns_array;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column())
            clipboardData.append(index.data().toString().append(NEWLINE));
        else {
            QModelIndex model_index = proxyModel->mapToSource(index);
            if(model_index.parent() == m_model->invisibleRootItem()->index()){
                s3s_item::NS *ns = static_cast<s3s_item::NS*>(m_model->itemFromIndex(model_index));
                ns_array.append(ns_to_json(ns));
            }
        }

    }

    qDebug() << "[NS-Enum] Copying NS results to clipboard...";

    if(ns_array.isEmpty())
        clipboard->setText(clipboardData.trimmed());
    else {
        QJsonDocument document;
        document.setArray(ns_array);
        clipboard->setText(document.toJson());
    }
}

///
/// sending results
///

void NSEnum::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::NS *item = static_cast<s3s_item::NS*>(m_model->itemFromIndex(index));
        project->addEnumNS(ns_to_struct(item));
    }
}

void NSEnum::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        QModelIndex model_index = proxyModel->mapToSource(index);
        if(model_index.parent() == m_model->invisibleRootItem()->index()){
            s3s_item::NS *ns = static_cast<s3s_item::NS*>(m_model->itemFromIndex(model_index));
            project->addEnumNS(ns_to_struct(ns));
        }
    }
}

void NSEnum::sendToEngine(const ENGINE &engine){
    switch (engine) {
    case ENGINE::OSINT:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(m_model->itemFromIndex(model_index));
            for(int j = 0; j < item->domains->rowCount(); j++)
                emit sendResultsToOsint(item->domains->child(j, 0)->text(), RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(m_model->itemFromIndex(model_index));
            for(int j = 0; j < item->domains->rowCount(); j++)
                emit sendResultsToRaw(item->domains->child(j, 0)->text(), RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(m_model->itemFromIndex(model_index));
            for(int j = 0; j < item->domains->rowCount(); j++)
                emit sendResultsToBrute(item->domains->child(j, 0)->text(), RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(m_model->itemFromIndex(model_index));
            for(int j = 0; j < item->domains->rowCount(); j++)
                emit sendResultsToActive(item->domains->child(j, 0)->text(), RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(m_model->itemFromIndex(model_index));
            for(int j = 0; j < item->domains->rowCount(); j++)
                emit sendResultsToDns(item->domains->child(j, 0)->text(), RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToDns();
        break;
    case ENGINE::SSL:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(m_model->itemFromIndex(model_index));
            for(int j = 0; j < item->domains->rowCount(); j++)
                emit sendResultsToSsl(item->domains->child(j, 0)->text(), RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToSSL();
        break;
    case ENGINE::URL:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(m_model->itemFromIndex(model_index));
            for(int j = 0; j < item->domains->rowCount(); j++)
                emit sendResultsToUrl(item->domains->child(j, 0)->text(), RESULT_TYPE::SUBDOMAIN);
        }
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

void NSEnum::sendSelectedToEngine(const ENGINE &engine){
    switch (engine) {
    case ENGINE::OSINT:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToOsint(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToRaw(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToBrute(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToActive(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToDns(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
        emit changeTabToDns();
        break;
    case ENGINE::SSL:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToSsl(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
        emit changeTabToSSL();
        break;
    case ENGINE::URL:
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            emit sendResultsToUrl(index.data().toString(), RESULT_TYPE::SUBDOMAIN);
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

///
/// receiving targets
///

void NSEnum::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::NS)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

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

void NSEnum::sendToEngine(const TOOL &engine){
    QSet<QString> targets;

    /* getting targets */
    for(int i = 0; i != proxyModel->rowCount(); ++i){
        QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
        s3s_item::NS *item = static_cast<s3s_item::NS*>(m_model->itemFromIndex(model_index));

        for(int j = 0; j < item->domains->rowCount(); j++)
            targets.insert(item->domains->child(j, 0)->text());
    }

    /* sending the targets */
    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToRaw();
        break;
    case TOOL::BRUTE:
        emit sendToBrute(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToBrute();
        break;
    case TOOL::HOST:
        emit sendToHost(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToHost();
        break;
    case TOOL::DNS:
        emit sendToDns(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToDns();
        break;
    case TOOL::SSL:
        emit sendToSsl(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void NSEnum::sendSelectedToEngine(const TOOL &engine){
    QSet<QString> targets;

    /* getting targets */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        if(index.column())
            targets.insert(index.data().toString());

    /* sending the targets */
    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToRaw();
        break;
    case TOOL::BRUTE:
        emit sendToBrute(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToBrute();
        break;
    case TOOL::HOST:
        emit sendToHost(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToHost();
        break;
    case TOOL::DNS:
        emit sendToDns(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToDns();
        break;
    case TOOL::SSL:
        emit sendToSsl(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

///
/// receiving targets
///

void NSEnum::onReceiveTargets(QSet<QString> targets, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::NS)
        ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

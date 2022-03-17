#include "MXEnum.h"
#include "ui_MXEnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


void MXEnum::clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({tr(" MX"), tr(" Value")});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void MXEnum::removeResults(){
    /* loop to delete all selected items */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        QModelIndex model_index = proxyModel->mapToSource(index);

        /* remove entire mx */
        if(model_index.parent() == m_model->invisibleRootItem()->index()){
            m_resultsSet.remove(model_index.data().toString());
            m_model->removeRow(model_index.row());
        }
        /* remove a certain row in the mx item */
        else{
            m_model->removeRow(model_index.row());
        }
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void MXEnum::saveResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "MX-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "MX-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray mx_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::MX *mx = static_cast<s3s_item::MX*>(m_model->itemFromIndex(index));
        mx_array.append(mx_to_json(mx));
    }

    QJsonDocument document;
    document.setArray(mx_array);
    file.write(document.toJson());
    file.close();
}

void MXEnum::saveSelectedResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "MX-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "MX-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray mx_array;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column())
            file.write(index.data().toString().append(NEWLINE).toUtf8());
        else {
            QModelIndex model_index = proxyModel->mapToSource(index);
            if(model_index.parent() == m_model->invisibleRootItem()->index()){
                s3s_item::MX *mx = static_cast<s3s_item::MX*>(m_model->itemFromIndex(model_index));
                mx_array.append(mx_to_json(mx));
            }
        }

    }

    if(!mx_array.isEmpty()){
        QJsonDocument document;
        document.setArray(mx_array);
        file.write(document.toJson());
    }
    file.close();
}

void MXEnum::copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray mx_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::MX *mx = static_cast<s3s_item::MX*>(m_model->itemFromIndex(index));
        mx_array.append(mx_to_json(mx));
    }

    qDebug() << "Copying Email results to clipboard...";

    QJsonDocument document;
    document.setArray(mx_array);
    clipboard->setText(document.toJson());
}

void MXEnum::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QJsonArray mx_array;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column())
            clipboardData.append(index.data().toString().append(NEWLINE));
        else {
            QModelIndex model_index = proxyModel->mapToSource(index);
            if(model_index.parent() == m_model->invisibleRootItem()->index()){
                s3s_item::MX *mx = static_cast<s3s_item::MX*>(m_model->itemFromIndex(model_index));
                mx_array.append(mx_to_json(mx));
            }
        }

    }

    qDebug() << "[MX-Enum] Copying MX results to clipboard...";

    if(mx_array.isEmpty())
        clipboard->setText(clipboardData.trimmed());
    else {
        QJsonDocument document;
        document.setArray(mx_array);
        clipboard->setText(document.toJson());
    }
}

///
/// sending results
///

void MXEnum::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::MX *item = static_cast<s3s_item::MX*>(m_model->itemFromIndex(index));
        project->addEnumMX(mx_to_struct(item));
    }
}

void MXEnum::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        QModelIndex model_index = proxyModel->mapToSource(index);
        if(model_index.parent() == m_model->invisibleRootItem()->index()){
            s3s_item::MX *mx = static_cast<s3s_item::MX*>(m_model->itemFromIndex(model_index));
            project->addEnumMX(mx_to_struct(mx));
        }
    }
}

void MXEnum::sendToEngine(const ENGINE &engine){
    QSet<QString> targets;

    /* getting targets */
    for(int i = 0; i != proxyModel->rowCount(); ++i){
        QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
        s3s_item::MX *item = static_cast<s3s_item::MX*>(m_model->itemFromIndex(model_index));

        for(int j = 0; j < item->domains->rowCount(); j++)
            targets.insert(item->domains->child(j, 1)->text());
    }

    /* sending the targets */
    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        emit sendToBrute(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        emit sendToActive(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        emit sendToDns(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToDns();
        break;
    case ENGINE::SSL:
        emit sendToSsl(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void MXEnum::sendSelectedToEngine(const ENGINE &engine){
    QSet<QString> targets;

    /* getting targets */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        if(index.column())
            targets.insert(index.data().toString());

    /* sending the targets */
    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        emit sendToBrute(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        emit sendToActive(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        emit sendToDns(targets, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToDns();
        break;
    case ENGINE::SSL:
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

void MXEnum::onReceiveTargets(QSet<QString> targets, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::MX)
        ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

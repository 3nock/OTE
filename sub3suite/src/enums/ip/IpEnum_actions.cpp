#include "IpEnum.h"
#include "ui_IpEnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


void IpEnum::clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({tr(" IP"), tr(" Value")});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void IpEnum::removeResults(){
    /* loop to delete all selected items */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        QModelIndex model_index = proxyModel->mapToSource(index);

        /* remove entire ip */
        if(model_index.parent() == m_model->invisibleRootItem()->index()){
            m_resultsSet.remove(model_index.data().toString());
            m_model->removeRow(model_index.row());
        }
        /* remove a certain row in the ip item */
        else{
            m_model->removeRow(model_index.row());
        }
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void IpEnum::saveResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "IP-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "IP-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray ip_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::IP *ip = static_cast<s3s_item::IP*>(m_model->itemFromIndex(index));
        ip_array.append(ip_to_json(ip));
    }

    QJsonDocument document;
    document.setArray(ip_array);
    file.write(document.toJson());
    file.close();
}

void IpEnum::saveSelectedResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "IP-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "IP-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray ip_array;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column())
            file.write(index.data().toString().append(NEWLINE).toUtf8());
        else {
            QModelIndex model_index = proxyModel->mapToSource(index);
            if(model_index.parent() == m_model->invisibleRootItem()->index()){
                s3s_item::IP *ip = static_cast<s3s_item::IP*>(m_model->itemFromIndex(model_index));
                ip_array.append(ip_to_json(ip));
            }
        }

    }

    if(!ip_array.isEmpty()){
        QJsonDocument document;
        document.setArray(ip_array);
        file.write(document.toJson());
    }
    file.close();
}

void IpEnum::copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray ip_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::IP *ip = static_cast<s3s_item::IP*>(m_model->itemFromIndex(index));
        ip_array.append(ip_to_json(ip));
    }

    qDebug() << "Copying IP results to clipboard...";

    QJsonDocument document;
    document.setArray(ip_array);
    clipboard->setText(document.toJson());
}

void IpEnum::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QJsonArray ip_array;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column())
            clipboardData.append(index.data().toString().append(NEWLINE));
        else {
            QModelIndex model_index = proxyModel->mapToSource(index);
            if(model_index.parent() == m_model->invisibleRootItem()->index()){
                s3s_item::IP *ip = static_cast<s3s_item::IP*>(m_model->itemFromIndex(model_index));
                ip_array.append(ip_to_json(ip));
            }
        }

    }

    qDebug() << "[IP-Enum] Copying IP results to clipboard...";

    if(ip_array.isEmpty())
        clipboard->setText(clipboardData.trimmed());
    else {
        QJsonDocument document;
        document.setArray(ip_array);
        clipboard->setText(document.toJson());
    }
}

///
/// sending results
///

void IpEnum::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::IP *item = static_cast<s3s_item::IP*>(m_model->itemFromIndex(index));
        project->addEnumIP(ip_to_struct(item));
    }
}

void IpEnum::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        QModelIndex model_index = proxyModel->mapToSource(index);
        if(model_index.parent() == m_model->invisibleRootItem()->index()){
            s3s_item::IP *ip = static_cast<s3s_item::IP*>(m_model->itemFromIndex(model_index));
            project->addEnumIP(ip_to_struct(ip));
        }
    }
}

///
/// receiving targets
///

void IpEnum::onReceiveTargets(QSet<QString> targets, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::IP)
        ui->targets->add(targets);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

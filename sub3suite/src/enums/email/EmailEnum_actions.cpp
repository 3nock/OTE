#include "EmailEnum.h"
#include "ui_EmailEnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


void EmailEnum::clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({tr(" Email"), tr(" Value")});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void EmailEnum::removeResults(){
    /* loop to delete all selected items */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        QModelIndex model_index = proxyModel->mapToSource(index);

        /* remove entire email */
        if(model_index.parent() == m_model->invisibleRootItem()->index()){
            m_resultsSet.remove(model_index.data().toString());
            m_model->removeRow(model_index.row());
        }
        /* remove a certain row in the email item */
        else{
            m_model->removeRow(model_index.row());
        }
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void EmailEnum::saveResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "Email-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "Email-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray email_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::Email *email = static_cast<s3s_item::Email*>(m_model->itemFromIndex(index));
        email_array.append(email_to_json(email));
    }

    QJsonDocument document;
    document.setArray(email_array);
    file.write(document.toJson());
    file.close();
}

void EmailEnum::saveSelectedResults(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qDebug() << "Email-Enum: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qDebug() << "Email-Enum: Failed to open " << filename << " For saving Results";
        return;
    }

    QJsonArray email_array;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column())
            file.write(index.data().toString().append(NEWLINE).toUtf8());
        else {
            QModelIndex model_index = proxyModel->mapToSource(index);
            if(model_index.parent() == m_model->invisibleRootItem()->index()){
                s3s_item::Email *email = static_cast<s3s_item::Email*>(m_model->itemFromIndex(model_index));
                email_array.append(email_to_json(email));
            }
        }

    }

    if(!email_array.isEmpty()){
        QJsonDocument document;
        document.setArray(email_array);
        file.write(document.toJson());
    }
    file.close();
}

void EmailEnum::copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray email_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::Email *email = static_cast<s3s_item::Email*>(m_model->itemFromIndex(index));
        email_array.append(email_to_json(email));
    }

    qDebug() << "Copying Email results to clipboard...";

    QJsonDocument document;
    document.setArray(email_array);
    clipboard->setText(document.toJson());
}

void EmailEnum::copySelectedResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QJsonArray email_array;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        if(index.column())
            clipboardData.append(index.data().toString().append(NEWLINE));
        else {
            QModelIndex model_index = proxyModel->mapToSource(index);
            if(model_index.parent() == m_model->invisibleRootItem()->index()){
                s3s_item::Email *email = static_cast<s3s_item::Email*>(m_model->itemFromIndex(model_index));
                email_array.append(email_to_json(email));
            }
        }

    }

    qDebug() << "[Email-Enum] Copying Emails results to clipboard...";

    if(email_array.isEmpty())
        clipboard->setText(clipboardData.trimmed());
    else {
        QJsonDocument document;
        document.setArray(email_array);
        clipboard->setText(document.toJson());
    }
}

///
/// sending results
///

void EmailEnum::sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::Email *item = static_cast<s3s_item::Email*>(m_model->itemFromIndex(index));
        project->addEnumEmail(email_to_struct(item));
    }
}

void EmailEnum::sendSelectedToProject(){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        QModelIndex model_index = proxyModel->mapToSource(index);
        if(model_index.parent() == m_model->invisibleRootItem()->index()){
            s3s_item::Email *email = static_cast<s3s_item::Email*>(m_model->itemFromIndex(model_index));
            project->addEnumEmail(email_to_struct(email));
        }
    }
}

///
/// receiving targets
///

void EmailEnum::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::EMAIL)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

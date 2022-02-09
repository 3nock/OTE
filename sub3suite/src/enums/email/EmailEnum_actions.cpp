#include "EmailEnum.h"
#include "ui_EmailEnum.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QClipboard>


/* TODO:
 *     validate if selected index is ip...
 */

void EmailEnum::m_clearResults(){
    /* clear the results... */
    m_model->clear();
    ui->labelResultsCount->clear();
    m_model->setHorizontalHeaderLabels({"    Email", "    Value"});
    m_resultsSet.clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void EmailEnum::m_removeResults(QItemSelectionModel *selectionModel){
    /* loop to delete all selected items */
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes())
    {
        QModelIndex index = proxyModel->mapToSource(proxyIndex);

        /* remove entire email */
        if(index.parent() == m_model->invisibleRootItem()->index()){
            m_resultsSet.remove(index.data().toString());
            m_model->removeRow(index.row());
        }
        /* remove a certain row in the email item */
        else{
            m_model->removeRow(index.row());
        }
    }
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void EmailEnum::m_saveResults(){
    /* checks... */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    if(!file.isOpen())
        return;

    QJsonArray email_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::Email *email = static_cast<s3s_item::Email*>(m_model->item(index.row(), index.column()));

        email_array.append(email_to_json(email));
    }

    QJsonDocument document;
    document.setArray(email_array);

    qDebug() << "Saving Email results to File: " << file.fileName();
    file.write(document.toJson());
    file.close();
}

void EmailEnum::m_saveResults(QItemSelectionModel *selection){
    /* checks... */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    if(!file.isOpen())
        return;

    QJsonArray email_array;
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::Email *email = static_cast<s3s_item::Email*>(m_model->item(model_index.row(), model_index.column()));

        email_array.append(email_to_json(email));
    }

    QJsonDocument document;
    document.setArray(email_array);

    qDebug() << "Saving Email results to File: " << file.fileName();
    file.write(document.toJson());
    file.close();
}

void EmailEnum::m_copyResults(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray email_array;
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::Email *email = static_cast<s3s_item::Email*>(m_model->item(index.row(), index.column()));

        email_array.append(email_to_json(email));
    }

    QJsonDocument document;
    document.setArray(email_array);

    qDebug() << "Copying Email results to clipboard...";
    clipboard->setText(document.toJson());
}

void EmailEnum::m_copyResults(QItemSelectionModel *selection){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QJsonArray email_array;
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::Email *email = static_cast<s3s_item::Email*>(m_model->item(model_index.row(), model_index.column()));

        email_array.append(email_to_json(email));
    }

    QJsonDocument document;
    document.setArray(email_array);

    qDebug() << "Copying Email results to clipboard...";
    clipboard->setText(document.toJson());
}

///
/// send all...
///

void EmailEnum::m_sendToProject(){
    for(int i = 0; i != proxyModel->rowCount(); ++i)
    {
        QModelIndex index = proxyModel->mapToSource(proxyModel->index(i ,0));
        s3s_item::Email *item = static_cast<s3s_item::Email*>(m_model->item(index.row(), index.column()));
        project->addEnumEmail(email_to_struct(item));
    }
}

///
/// send selected...
///

void EmailEnum::m_sendToProject(QItemSelectionModel *selection){
    foreach(const QModelIndex &index, selection->selectedIndexes()){
        QModelIndex model_index = proxyModel->mapToSource(index);
        s3s_item::Email *email = static_cast<s3s_item::Email*>(m_model->item(model_index.row(), model_index.column()));

        project->addEnumEmail(email_to_struct(email));
    }
}

void EmailEnum::onReceiveTargets(QString target, RESULT_TYPE resultType){
    if(resultType == RESULT_TYPE::EMAIL)
        ui->targets->add(target);

    /* set multiple targets checkbox checked */
    ui->checkBoxMultipleTargets->setChecked(true);
}

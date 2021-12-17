#include "Cert.h"
#include "ui_Cert.h"

#include <QClipboard>


void Cert::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->onClearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->onRemoveResults(selectionModel);});
    connect(&a_ExpandResults, &QAction::triggered, this, [=](){this->onExpandResults();});
    connect(&a_CollapseResults, &QAction::triggered, this, [=](){this->onCollapseResults();});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->onOpenInBrowser(selectionModel);});
    /* ... */
    connect(&a_SendHostToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::CERT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToOsint();});
    connect(&a_SendCertToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::CERT, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::CERT_ID); emit changeTabToOsint();});
    connect(&a_SendHostToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::CERT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToRaw();});
    connect(&a_SendCertToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::CERT, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::CERT_ID); emit changeTabToRaw();});
    connect(&a_SendHostToBrute, &QAction::triggered, this, [=](){emit sendResultsToBrute(ENGINE::CERT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToBrute();});
    connect(&a_SendHostToActive, &QAction::triggered, this, [=](){emit sendResultsToActive(ENGINE::CERT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToActive();});
    connect(&a_SendHostToDns, &QAction::triggered, this, [=](){emit sendResultsToDns(ENGINE::CERT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToDns();});
    connect(&a_SendHostToCert, &QAction::triggered, this, [=](){emit sendResultsToCert(ENGINE::CERT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToCert();});
    connect(&a_SendHostToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(ENGINE::CERT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN);});
    connect(&a_SendCertToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(ENGINE::CERT, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::CERT_ID);});
    connect(&a_SendHostToDomainTool, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(ENGINE::CERT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN);});
    /* ... */
    connect(&a_SendSelectedHostToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToOsint();});
    connect(&a_SendSelectedCertToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::CERT_ID); emit changeTabToOsint();});
    connect(&a_SendSelectedHostToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToRaw();});
    connect(&a_SendSelectedCertToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::CERT_ID); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToBrute, &QAction::triggered, this, [=](){emit sendResultsToBrute(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToBrute();});
    connect(&a_SendSelectedHostToActive, &QAction::triggered, this, [=](){emit sendResultsToActive(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToActive();});
    connect(&a_SendSelectedHostToDns, &QAction::triggered, this, [=](){emit sendResultsToDns(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToDns();});
    connect(&a_SendSelectedHostToCert, &QAction::triggered, this, [=](){emit sendResultsToCert(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToCert();});
    connect(&a_SendSelectedHostToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN);});
    connect(&a_SendSelectedCertToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(selectionModel, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::CERT_ID);});
    connect(&a_SendSelectedHostToDomainTool, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN);});
    /* ... */
    connect(&a_Save, &QAction::triggered, this, [=](){this->onSaveResults(selectionModel);});
    connect(&a_SaveSubdomain, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::SUBDOMAIN);});
    connect(&a_SaveCertId, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::CERT_ID);});
    connect(&a_SaveCertInfo, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::CERT_INFO);});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->onCopyResults(selectionModel);});
    connect(&a_CopySubdomain, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::SUBDOMAIN);});
    connect(&a_CopyCertId, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::CERT_ID);});
    connect(&a_CopyCertInfo, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::CERT_ID);});
}

void Cert::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    /* creating the context menu... */
    QMenu *mainMenu = new QMenu(this);
    QMenu *saveMenu = new QMenu(this);
    QMenu *copyMenu = new QMenu(this);
    QMenu *sendToToolMenu = new QMenu(this);
    QMenu *sendToEngineMenu = new QMenu(this);
    saveMenu->setTitle("Save");
    copyMenu->setTitle("Copy");
    sendToToolMenu->setTitle("Send To Tool");
    sendToEngineMenu->setTitle("Send To Engine");
    mainMenu->setAttribute(Qt::WA_DeleteOnClose, true);

    /* adding actions */
    if(ui->comboBoxOutput->currentIndex() == 0){
        saveMenu->addAction(&a_SaveSubdomain);
        copyMenu->addAction(&a_CopySubdomain);
        sendToEngineMenu->addAction(&a_SendHostToOsint);
        sendToEngineMenu->addAction(&a_SendHostToRaw);
        sendToEngineMenu->addAction(&a_SendHostToBrute);
        sendToEngineMenu->addAction(&a_SendHostToActive);
        sendToEngineMenu->addAction(&a_SendHostToDns);
        sendToEngineMenu->addAction(&a_SendHostToCert);
        sendToToolMenu->addAction(&a_SendHostToCertTool);
        sendToToolMenu->addAction(&a_SendHostToDomainTool);
    }
    if(ui->comboBoxOutput->currentIndex() == 1){
        saveMenu->addAction(&a_SaveCertId);
        copyMenu->addAction(&a_CopyCertId);
        sendToEngineMenu->addAction(&a_SendCertToOsint);
        sendToEngineMenu->addAction(&a_SendCertToRaw);
        sendToToolMenu->addAction(&a_SendCertToCertTool);
    }
    if(ui->comboBoxOutput->currentIndex() == 2){
        saveMenu->addAction(&a_SaveCertInfo);
        copyMenu->addAction(&a_CopyCertInfo);
    }
    /* adding to mainMenu */
    mainMenu->addAction(&a_ClearResults);
    mainMenu->addSeparator();
    mainMenu->addMenu(saveMenu);
    mainMenu->addMenu(copyMenu);
    mainMenu->addSeparator();
    mainMenu->addMenu(sendToEngineMenu);
    mainMenu->addMenu(sendToToolMenu);

    /* showing the context menu... */
    mainMenu->exec(pos);
}

void Cert::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->treeViewResults->selectionModel()->isSelected(ui->treeViewResults->currentIndex())){
        return;
    }

    /* getting the selected items... */
    selectionModel = ui->treeViewResults->selectionModel();

    /* creating the context menu... */
    QMenu *mainMenu = new QMenu(this);
    QMenu *saveMenu = new QMenu(this);
    QMenu *copyMenu = new QMenu(this);
    QMenu *sendToToolMenu = new QMenu(this);
    QMenu *sendToEngineMenu = new QMenu(this);
    saveMenu->setTitle("Save");
    copyMenu->setTitle("Copy");
    sendToToolMenu->setTitle("Send To Tool");
    sendToEngineMenu->setTitle("Send To Engine");
    mainMenu->setAttribute(Qt::WA_DeleteOnClose, true);

    /* adding actions */
    if(ui->comboBoxOutput->currentIndex() == 0){
        saveMenu->addAction(&a_Save);
        copyMenu->addAction(&a_Copy);
        sendToEngineMenu->addAction(&a_SendSelectedHostToOsint);
        sendToEngineMenu->addAction(&a_SendSelectedHostToRaw);
        sendToEngineMenu->addAction(&a_SendSelectedHostToBrute);
        sendToEngineMenu->addAction(&a_SendSelectedHostToActive);
        sendToEngineMenu->addAction(&a_SendSelectedHostToDns);
        sendToEngineMenu->addAction(&a_SendSelectedHostToCert);
        sendToToolMenu->addAction(&a_SendSelectedHostToCertTool);
        sendToToolMenu->addAction(&a_SendSelectedHostToDomainTool);
    }
    if(ui->comboBoxOutput->currentIndex() == 1){
        saveMenu->addAction(&a_Save);
        copyMenu->addAction(&a_Copy);
        sendToEngineMenu->addAction(&a_SendSelectedCertToOsint);
        sendToEngineMenu->addAction(&a_SendSelectedCertToRaw);
        sendToToolMenu->addAction(&a_SendSelectedCertToCertTool);
    }
    if(ui->comboBoxOutput->currentIndex() == 2){
        saveMenu->addAction(&a_Save);
        copyMenu->addAction(&a_Copy);
    }

    /* adding to mainMenu */
    mainMenu->addAction(&a_RemoveResults);
    mainMenu->addAction(&a_OpenInBrowser);
    mainMenu->addSeparator();
    mainMenu->addMenu(saveMenu);
    mainMenu->addMenu(copyMenu);
    mainMenu->addSeparator();
    mainMenu->addMenu(sendToEngineMenu);
    mainMenu->addMenu(sendToToolMenu);

    /* showing the context menu... */
    mainMenu->exec(QCursor::pos());
}

void Cert::onClearResults(){
    ui->labelResultsCount->clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();

    switch (ui->comboBoxOutput->currentIndex()) {
    case OUTPUT_SUBDOMAIN:
        result->cert->subdomain->clear();
        ui->lineEditFilter->clear();
        break;
    case OUTPUT_SSLCERT:
        result->cert->sslCert->clear();
        ui->lineEditFilter->clear();
        break;
    case OUTPUT_CERTINFO:
        result->cert->certInfo->clear();
        ui->lineEditFilter->clear();
        break;
    }
}

void Cert::onExpandResults(){

}

void Cert::onCollapseResults(){

}

void Cert::onOpenInBrowser(QItemSelectionModel *){

}

void Cert::onRemoveResults(QItemSelectionModel *selectionModel){
    Q_UNUSED(selectionModel);
}

void Cert::onSaveResults(RESULT_TYPE resultType){
    ///
    /// checks...
    ///
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        return;
    }
    ///
    /// variable declarations...
    ///
    QSet<QString> itemSet;
    QString item;
    ///
    /// choice of item to save...
    ///
    switch(resultType){
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case RESULT_TYPE::IP:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;
    case RESULT_TYPE::SUBDOMAINIP:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString()+":"+result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                file.write(item.toUtf8());
            }
        }
        break;

    default:
        break;
    }
    file.close();
}

void Cert::onSaveResults(QItemSelectionModel *selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty()){
        return;
    }
    QSet<QString> itemSet;
    QString data;
    QString item;

    /* ... */
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                data.append(item.append(NEWLINE));
            }
        }
        file.write(data.toUtf8());
        file.close();
    }
}


void Cert::onCopyResults(RESULT_TYPE resultType){
    ///
    /// variable declaration...
    ///
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QSet<QString> itemSet;
    QString item;
    ///
    /// type of item to save...
    ///
    switch(resultType){
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case RESULT_TYPE::IP:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    case RESULT_TYPE::SUBDOMAINIP:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString()+"|"+result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            if(!itemSet.contains(item)){
                itemSet.insert(item);
                clipboardData.append(item);
            }
        }
        break;
    default:
        break;
    }
    clipboard->setText(clipboardData);
}

void Cert::onCopyResults(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QSet<QString> itemSet;
    QString data;
    QString item;
    /* ... */
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        if(!itemSet.contains(item)){
            itemSet.insert(item);
            data.append(item.append(NEWLINE));
        }
    }
    clipboard->setText(data);
}

void Cert::onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void Cert::onReceiveTargets(QItemSelectionModel *, RESULT_TYPE, RESULT_MODEL_TYPE){

}

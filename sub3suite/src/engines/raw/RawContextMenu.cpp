#include "Raw.h"
#include "ui_Raw.h"

#include <QClipboard>


void Raw::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->onClearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->onRemoveResults(selectionModel);});
    connect(&a_ExpandResults, &QAction::triggered, this, [=](){this->onExpandResults();});
    connect(&a_CollapseResults, &QAction::triggered, this, [=](){this->onCollapseResults();});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->onOpenInBrowser(selectionModel);});
    /* ... */
    connect(&a_SendIpToIp, &QAction::triggered, this, [=](){emit sendResultsToIp(ENGINE::RAW, RESULT_TYPE::IP, RESULT_MODEL_TYPE::RAW); emit changeTabToIp();});
    connect(&a_SendIpToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::RAW, RESULT_TYPE::IP, RESULT_MODEL_TYPE::RAW); emit changeTabToOsint();});
    connect(&a_SendIpToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::RAW, RESULT_TYPE::IP, RESULT_MODEL_TYPE::RAW); emit changeTabToRaw();});
    connect(&a_SendHostToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW); emit changeTabToOsint();});
    connect(&a_SendHostToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW); emit changeTabToRaw();});
    connect(&a_SendHostToBrute, &QAction::triggered, this, [=](){emit sendResultsToBrute(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW); emit changeTabToBrute();});
    connect(&a_SendHostToActive, &QAction::triggered, this, [=](){emit sendResultsToActive(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW); emit changeTabToActive();});
    connect(&a_SendHostToDns, &QAction::triggered, this, [=](){emit sendResultsToDns(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW); emit changeTabToDns();});
    connect(&a_SendHostToCert, &QAction::triggered, this, [=](){emit sendResultsToCert(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW); emit changeTabToCert();});
    connect(&a_SendEmailToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::RAW, RESULT_TYPE::EMAIL, RESULT_MODEL_TYPE::RAW); emit changeTabToOsint();});
    connect(&a_SendAsnToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::RAW, RESULT_TYPE::ASN, RESULT_MODEL_TYPE::RAW); emit changeTabToOsint();});
    connect(&a_SendCidrToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::RAW, RESULT_TYPE::CIDR, RESULT_MODEL_TYPE::RAW); emit changeTabToOsint();});
    connect(&a_SendCertToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::RAW, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::RAW); emit changeTabToOsint();});
    connect(&a_SendIpToIpTool, &QAction::triggered, this, [=](){emit sendResultsToIpTool(ENGINE::RAW, RESULT_TYPE::IP, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendHostToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendHostToDomainTool, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(ENGINE::RAW, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendAsnToAsnTool, &QAction::triggered, this, [=](){emit sendResultsToASNTool(ENGINE::RAW, RESULT_TYPE::ASN, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendEmailToEmailTool, &QAction::triggered, this, [=](){emit sendResultsToEmailTool(ENGINE::RAW, RESULT_TYPE::EMAIL, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendCidrToCidrTool, &QAction::triggered, this, [=](){emit sendResultsToCidrTool(ENGINE::RAW, RESULT_TYPE::CIDR, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendCertToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(ENGINE::RAW, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendNSToNSTool, &QAction::triggered, this, [=](){emit sendResultsToNSTool(ENGINE::RAW, RESULT_TYPE::NS, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendMXToMXTool, &QAction::triggered, this, [=](){emit sendResultsToMXTool(ENGINE::RAW, RESULT_TYPE::MX, RESULT_MODEL_TYPE::RAW);});
    /* ... */
    connect(&a_SendSelectedIpToIp, &QAction::triggered, this, [=](){emit sendResultsToIp(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::RAW); emit changeTabToIp();});
    connect(&a_SendSelectedIpToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::RAW); emit changeTabToOsint();});
    connect(&a_SendSelectedIpToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::RAW); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW); emit changeTabToOsint();});
    connect(&a_SendSelectedHostToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToBrute, &QAction::triggered, this, [=](){emit sendResultsToBrute(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW); emit changeTabToBrute();});
    connect(&a_SendSelectedHostToActive, &QAction::triggered, this, [=](){emit sendResultsToActive(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW); emit changeTabToActive();});
    connect(&a_SendSelectedHostToDns, &QAction::triggered, this, [=](){emit sendResultsToDns(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW); emit changeTabToDns();});
    connect(&a_SendSelectedHostToCert, &QAction::triggered, this, [=](){emit sendResultsToCert(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW); emit changeTabToCert();});
    connect(&a_SendSelectedEmailToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::EMAIL, RESULT_MODEL_TYPE::RAW); emit changeTabToOsint();});
    connect(&a_SendSelectedAsnToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::ASN, RESULT_MODEL_TYPE::RAW); emit changeTabToOsint();});
    connect(&a_SendSelectedCidrToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::CIDR, RESULT_MODEL_TYPE::RAW); emit changeTabToOsint();});
    connect(&a_SendSelectedCertToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::RAW); emit changeTabToOsint();});
    connect(&a_SendSelectedIpToIpTool, &QAction::triggered, this, [=](){emit sendResultsToIpTool(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendSelectedHostToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendSelectedHostToDomainTool, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendSelectedAsnToAsnTool, &QAction::triggered, this, [=](){emit sendResultsToASNTool(selectionModel, RESULT_TYPE::ASN, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendSelectedEmailToEmailTool, &QAction::triggered, this, [=](){emit sendResultsToEmailTool(selectionModel, RESULT_TYPE::EMAIL, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendSelectedCidrToCidrTool, &QAction::triggered, this, [=](){emit sendResultsToCidrTool(selectionModel, RESULT_TYPE::CIDR, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendSelectedCertToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(selectionModel, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendSelectedNSToNSTool, &QAction::triggered, this, [=](){emit sendResultsToNSTool(selectionModel, RESULT_TYPE::NS, RESULT_MODEL_TYPE::RAW);});
    connect(&a_SendSelectedMXToMXTool, &QAction::triggered, this, [=](){emit sendResultsToMXTool(selectionModel, RESULT_TYPE::MX, RESULT_MODEL_TYPE::RAW);});
    /* ... */
    connect(&a_Save, &QAction::triggered, this, [=](){this->onSaveResults(selectionModel);});
    connect(&a_SaveJson, &QAction::triggered, this, [=](){this->onSaveResultsJson();});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->onCopyResults(selectionModel);});
    connect(&a_CopyJson, &QAction::triggered, this, [=](){this->onCopyResultsJson();});
}

void Raw::on_buttonActionJson_clicked(){
    /* check if there are results available else dont show the context menu */


    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonActionJson->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    /* creating the context menu... */
    QMenu *mainMenu = new QMenu(this);
    mainMenu->setAttribute(Qt::WA_DeleteOnClose, true);

    mainMenu->addAction(&a_SaveJson);
    mainMenu->addAction(&a_CopyJson);

    /* showing the context menu... */
    mainMenu->exec(pos);
}

void Raw::on_buttonActionTree_clicked(){
    /* check if there are results available else dont show the context menu */


    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonActionTree->mapToGlobal(QPoint(0,0));
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
    saveMenu->addAction(&a_Save);
    copyMenu->addAction(&a_Copy);
    sendToEngineMenu->addAction(&a_SendIpToIp);
    sendToEngineMenu->addAction(&a_SendIpToOsint);
    sendToEngineMenu->addAction(&a_SendIpToRaw);
    sendToEngineMenu->addAction(&a_SendHostToOsint);
    sendToEngineMenu->addAction(&a_SendHostToRaw);
    sendToEngineMenu->addAction(&a_SendHostToBrute);
    sendToEngineMenu->addAction(&a_SendHostToActive);
    sendToEngineMenu->addAction(&a_SendHostToDns);
    sendToEngineMenu->addAction(&a_SendHostToCert);
    sendToEngineMenu->addAction(&a_SendEmailToOsint);
    sendToEngineMenu->addAction(&a_SendAsnToOsint);
    sendToEngineMenu->addAction(&a_SendCidrToOsint);
    sendToEngineMenu->addAction(&a_SendCertToOsint);
    sendToToolMenu->addAction(&a_SendIpToIpTool);
    sendToToolMenu->addAction(&a_SendHostToCertTool);
    sendToToolMenu->addAction(&a_SendHostToDomainTool);
    sendToToolMenu->addAction(&a_SendNSToNSTool);
    sendToToolMenu->addAction(&a_SendMXToMXTool);
    /* adding to mainMenu */
    mainMenu->addAction(&a_ClearResults);
    mainMenu->addAction(&a_ExpandResults);
    mainMenu->addAction(&a_CollapseResults);
    mainMenu->addSeparator();
    mainMenu->addMenu(saveMenu);
    mainMenu->addMenu(copyMenu);
    mainMenu->addSeparator();
    mainMenu->addMenu(sendToEngineMenu);
    mainMenu->addMenu(sendToToolMenu);

    /* showing the context menu... */
    mainMenu->exec(pos);
}

void Raw::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
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
    saveMenu->addAction(&a_Save);
    copyMenu->addAction(&a_Copy);
    sendToEngineMenu->addAction(&a_SendSelectedIpToIp);
    sendToEngineMenu->addAction(&a_SendSelectedIpToOsint);
    sendToEngineMenu->addAction(&a_SendSelectedIpToRaw);
    sendToEngineMenu->addAction(&a_SendSelectedHostToOsint);
    sendToEngineMenu->addAction(&a_SendSelectedHostToRaw);
    sendToEngineMenu->addAction(&a_SendSelectedHostToBrute);
    sendToEngineMenu->addAction(&a_SendSelectedHostToActive);
    sendToEngineMenu->addAction(&a_SendSelectedHostToDns);
    sendToEngineMenu->addAction(&a_SendSelectedHostToCert);
    sendToEngineMenu->addAction(&a_SendSelectedEmailToOsint);
    sendToEngineMenu->addAction(&a_SendSelectedAsnToOsint);
    sendToEngineMenu->addAction(&a_SendSelectedCidrToOsint);
    sendToEngineMenu->addAction(&a_SendSelectedCertToOsint);
    sendToToolMenu->addAction(&a_SendSelectedIpToIpTool);
    sendToToolMenu->addAction(&a_SendSelectedHostToCertTool);
    sendToToolMenu->addAction(&a_SendSelectedHostToDomainTool);
    sendToToolMenu->addAction(&a_SendSelectedNSToNSTool);
    sendToToolMenu->addAction(&a_SendSelectedMXToMXTool);
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

void Raw::onClearResults(){
    /* clear the results... */
    result->active->subdomainIp->clear();
    ui->labelResultsCount->clear();
    result->active->subdomainIp->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Raw::onExpandResults(){

}

void Raw::onCollapseResults(){

}

void Raw::onSaveResultsJson(){

}

void Raw::onOpenInBrowser(QItemSelectionModel *){

}

void Raw::onRemoveResults(QItemSelectionModel *selectionModel){
    Q_UNUSED(selectionModel);
}

void Raw::onSaveResults(RESULT_TYPE resultType){
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

void Raw::onSaveResults(QItemSelectionModel *selectionModel){
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

void Raw::onCopyResultsJson(){

}


void Raw::onCopyResults(RESULT_TYPE resultType){
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

void Raw::onCopyResults(QItemSelectionModel *selectionModel){
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

void Raw::onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void Raw::onReceiveTargets(QItemSelectionModel *, RESULT_TYPE, RESULT_MODEL_TYPE){

}

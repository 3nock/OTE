#include "Brute.h"
#include "ui_Brute.h"

#include <QClipboard>


void Brute::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->onClearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->onRemoveResults(selectionModel);});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->onOpenInBrowser(selectionModel);});
    /* ... */
    connect(&a_SendIpToIpFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToIp(ENGINE::BRUTE, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToIp();});
    connect(&a_SendIpToIpFromTld, &QAction::triggered, this, [=](){emit sendResultsToIp(ENGINE::BRUTE, RESULT_TYPE::IP, RESULT_MODEL_TYPE::TLD); emit changeTabToIp();});
    connect(&a_SendIpToOsintFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::BRUTE, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendIpToOsintFromTld, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::BRUTE, RESULT_TYPE::IP, RESULT_MODEL_TYPE::TLD); emit changeTabToOsint();});
    connect(&a_SendIpToRawFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::BRUTE, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendIpToRawFromTld, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::BRUTE, RESULT_TYPE::IP, RESULT_MODEL_TYPE::TLD); emit changeTabToRaw();});
    connect(&a_SendHostToOsintFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendHostToOsintFromTld, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD); emit changeTabToOsint();});
    connect(&a_SendHostToRawFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendHostToRawFromTld, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD); emit changeTabToRaw();});
    connect(&a_SendHostToBruteFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToBrute(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToBrute();});
    connect(&a_SendHostToBruteFromTld, &QAction::triggered, this, [=](){emit sendResultsToBrute(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD); emit changeTabToBrute();});
    connect(&a_SendHostToActiveFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToActive(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToActive();});
    connect(&a_SendHostToActiveFromTld, &QAction::triggered, this, [=](){emit sendResultsToActive(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD); emit changeTabToActive();});
    connect(&a_SendHostToDnsFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToDns(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToDns();});
    connect(&a_SendHostToDnsFromTld, &QAction::triggered, this, [=](){emit sendResultsToDns(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD); emit changeTabToDns();});
    connect(&a_SendHostToCertFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToCert(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToCert();});
    connect(&a_SendHostToCertFromTld, &QAction::triggered, this, [=](){emit sendResultsToCert(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD); emit changeTabToCert();});
    connect(&a_SendIpToIpToolFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToIpTool(ENGINE::BRUTE, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendIpToIpToolFromTld, &QAction::triggered, this, [=](){emit sendResultsToIpTool(ENGINE::BRUTE, RESULT_TYPE::IP, RESULT_MODEL_TYPE::TLD);});
    connect(&a_SendHostToCertToolFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToCertTool(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendHostToCertToolFromTld, &QAction::triggered, this, [=](){emit sendResultsToCertTool(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD);});
    connect(&a_SendHostToDomainToolFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendHostToDomainToolFromTld, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(ENGINE::BRUTE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD);});
    /* ... */
    connect(&a_SendSelectedIpToIpFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToIp(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToIp();});
    connect(&a_SendSelectedIpToIpFromTld, &QAction::triggered, this, [=](){emit sendResultsToIp(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::TLD); emit changeTabToIp();});
    connect(&a_SendSelectedIpToOsintFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendSelectedIpToOsintFromTld, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::TLD); emit changeTabToOsint();});
    connect(&a_SendSelectedIpToRawFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendSelectedIpToRawFromTld, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::TLD); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToOsintFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendSelectedHostToOsintFromTld, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD); emit changeTabToOsint();});
    connect(&a_SendSelectedHostToRawFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToRawFromTld, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToBruteFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToBrute(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToBrute();});
    connect(&a_SendSelectedHostToBruteFromTld, &QAction::triggered, this, [=](){emit sendResultsToBrute(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD); emit changeTabToBrute();});
    connect(&a_SendSelectedHostToActiveFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToActive(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToActive();});
    connect(&a_SendSelectedHostToActiveFromTld, &QAction::triggered, this, [=](){emit sendResultsToActive(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD); emit changeTabToActive();});
    connect(&a_SendSelectedHostToDnsFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToDns(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToDns();});
    connect(&a_SendSelectedHostToDnsFromTld, &QAction::triggered, this, [=](){emit sendResultsToDns(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD); emit changeTabToDns();});
    connect(&a_SendSelectedHostToCertFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToCert(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToCert();});
    connect(&a_SendSelectedHostToCertFromTld, &QAction::triggered, this, [=](){emit sendResultsToCert(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD); emit changeTabToCert();});
    connect(&a_SendSelectedIpToIpToolFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToIpTool(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendSelectedIpToIpToolFromTld, &QAction::triggered, this, [=](){emit sendResultsToIpTool(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::TLD);});
    connect(&a_SendSelectedHostToCertToolFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToCertTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendSelectedHostToCertToolFromTld, &QAction::triggered, this, [=](){emit sendResultsToCertTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD);});
    connect(&a_SendSelectedHostToDomainToolFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendSelectedHostToDomainToolFromTld, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD);});
    /* ... */
    connect(&a_SaveFromSubdomain, &QAction::triggered, this, [=](){this->onSaveResults(selectionModel);});
    connect(&a_SaveFromTld, &QAction::triggered, this, [=](){this->onSaveResults(selectionModel);});
    connect(&a_SaveSubdomainIpFromSubdomain, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::SUBDOMAINIP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SaveSubdomainIpFromTld, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::SUBDOMAINIP, RESULT_MODEL_TYPE::TLD);});
    connect(&a_SaveSubdomainFromSubdomain, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SaveSubdomainFromTld, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD);});
    connect(&a_SaveIpFromSubdomain, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SaveIpFromTld, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::IP, RESULT_MODEL_TYPE::TLD);});
    connect(&a_CopyFromSubdomain, &QAction::triggered, this, [=](){this->onCopyResults(selectionModel);});
    connect(&a_CopyFromTld, &QAction::triggered, this, [=](){this->onCopyResults(selectionModel);});
    connect(&a_CopySubdomainIpFromSubdomain, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::SUBDOMAINIP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_CopySubdomainIpFromTld, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::SUBDOMAINIP, RESULT_MODEL_TYPE::TLD);});
    connect(&a_CopySubdomainFromSubdomain, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_CopySubdomainFromTld, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::TLD);});
    connect(&a_CopyIpFromSubdomain, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_CopyIpFromTld, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::IP, RESULT_MODEL_TYPE::TLD);});
}

void Brute::on_buttonAction_clicked(){
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
    if(ui->comboBoxBruteType->currentIndex() == 0){
        saveMenu->addAction(&a_SaveSubdomainIpFromSubdomain);
        saveMenu->addAction(&a_SaveSubdomainFromSubdomain);
        saveMenu->addAction(&a_SaveIpFromSubdomain);
        copyMenu->addAction(&a_CopySubdomainIpFromSubdomain);
        copyMenu->addAction(&a_CopySubdomainFromSubdomain);
        copyMenu->addAction(&a_CopyIpFromSubdomain);
        sendToEngineMenu->addAction(&a_SendIpToIpFromSubdomain);
        sendToEngineMenu->addAction(&a_SendIpToOsintFromSubdomain);
        sendToEngineMenu->addAction(&a_SendIpToRawFromSubdomain);
        sendToEngineMenu->addSeparator();
        sendToEngineMenu->addAction(&a_SendHostToOsintFromSubdomain);
        sendToEngineMenu->addAction(&a_SendHostToRawFromSubdomain);
        sendToEngineMenu->addAction(&a_SendHostToBruteFromSubdomain);
        sendToEngineMenu->addAction(&a_SendHostToActiveFromSubdomain);
        sendToEngineMenu->addAction(&a_SendHostToDnsFromSubdomain);
        sendToEngineMenu->addAction(&a_SendHostToCertFromSubdomain);
        sendToToolMenu->addAction(&a_SendIpToIpToolFromSubdomain);
        sendToToolMenu->addAction(&a_SendHostToCertToolFromSubdomain);
        sendToToolMenu->addAction(&a_SendHostToDomainToolFromSubdomain);
    }
    if(ui->comboBoxBruteType->currentIndex() == 1){
        saveMenu->addAction(&a_SaveSubdomainIpFromTld);
        saveMenu->addAction(&a_SaveSubdomainFromTld);
        saveMenu->addAction(&a_SaveIpFromTld);
        copyMenu->addAction(&a_CopySubdomainIpFromTld);
        copyMenu->addAction(&a_CopySubdomainFromTld);
        copyMenu->addAction(&a_CopyIpFromTld);
        sendToEngineMenu->addAction(&a_SendIpToIpFromTld);
        sendToEngineMenu->addAction(&a_SendIpToOsintFromTld);
        sendToEngineMenu->addAction(&a_SendIpToRawFromTld);
        sendToEngineMenu->addSeparator();
        sendToEngineMenu->addAction(&a_SendHostToOsintFromTld);
        sendToEngineMenu->addAction(&a_SendHostToRawFromTld);
        sendToEngineMenu->addAction(&a_SendHostToBruteFromTld);
        sendToEngineMenu->addAction(&a_SendHostToActiveFromTld);
        sendToEngineMenu->addAction(&a_SendHostToDnsFromTld);
        sendToEngineMenu->addAction(&a_SendHostToCertFromTld);
        sendToToolMenu->addAction(&a_SendIpToIpToolFromTld);
        sendToToolMenu->addAction(&a_SendHostToCertToolFromTld);
        sendToToolMenu->addAction(&a_SendHostToDomainToolFromTld);
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

void Brute::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex())){
        return;
    }

    /* getting the selected items... */
    selectionModel = ui->tableViewResults->selectionModel();

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
    if(ui->comboBoxBruteType->currentIndex() == 0){
        saveMenu->addAction(&a_SaveFromSubdomain);
        copyMenu->addAction(&a_CopyFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedIpToIpFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedIpToOsintFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedIpToRawFromSubdomain);
        sendToEngineMenu->addSeparator();
        sendToEngineMenu->addAction(&a_SendSelectedHostToOsintFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedHostToRawFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedHostToBruteFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedHostToActiveFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedHostToDnsFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedHostToCertFromSubdomain);
        sendToToolMenu->addAction(&a_SendSelectedIpToIpToolFromSubdomain);
        sendToToolMenu->addAction(&a_SendSelectedHostToCertToolFromSubdomain);
        sendToToolMenu->addAction(&a_SendSelectedHostToDomainToolFromSubdomain);
    }
    if(ui->comboBoxBruteType->currentIndex() == 1){
        saveMenu->addAction(&a_SaveFromTld);
        copyMenu->addAction(&a_CopyFromTld);
        sendToEngineMenu->addAction(&a_SendSelectedIpToIpFromTld);
        sendToEngineMenu->addAction(&a_SendSelectedIpToOsintFromTld);
        sendToEngineMenu->addAction(&a_SendSelectedIpToRawFromTld);
        sendToEngineMenu->addSeparator();
        sendToEngineMenu->addAction(&a_SendSelectedHostToOsintFromTld);
        sendToEngineMenu->addAction(&a_SendSelectedHostToRawFromTld);
        sendToEngineMenu->addAction(&a_SendSelectedHostToBruteFromTld);
        sendToEngineMenu->addAction(&a_SendSelectedHostToActiveFromTld);
        sendToEngineMenu->addAction(&a_SendSelectedHostToDnsFromTld);
        sendToEngineMenu->addAction(&a_SendSelectedHostToCertFromTld);
        sendToToolMenu->addAction(&a_SendSelectedIpToIpToolFromTld);
        sendToToolMenu->addAction(&a_SendSelectedHostToCertToolFromTld);
        sendToToolMenu->addAction(&a_SendSelectedHostToDomainToolFromTld);
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

void Brute::onOpenInBrowser(QItemSelectionModel *){

}

void Brute::onRemoveResults(QItemSelectionModel *selectionModel){
    Q_UNUSED(selectionModel);
}

void Brute::onSaveResults(RESULT_TYPE resultType, RESULT_MODEL_TYPE modelType){
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

void Brute::onSaveResults(QItemSelectionModel *selectionModel){
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


void Brute::onCopyResults(RESULT_TYPE resultType, RESULT_MODEL_TYPE modelType){
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

void Brute::onCopyResults(QItemSelectionModel *selectionModel){
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

void Brute::onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void Brute::onReceiveTargets(QItemSelectionModel *, RESULT_TYPE, RESULT_MODEL_TYPE){

}

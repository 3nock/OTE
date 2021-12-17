#include "Active.h"
#include "ui_Active.h"

#include <QClipboard>
#include <QFileDialog>
#include <QDesktopServices>


void Active::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->onClearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->onRemoveResults(selectionModel);});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->onOpenInBrowser(selectionModel);});
    /* ... */
    connect(&a_SendIpToIp, &QAction::triggered, this, [=](){emit sendResultsToIp(ENGINE::ACTIVE, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToIp();});
    connect(&a_SendIpToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::ACTIVE, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendIpToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::ACTIVE, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendHostToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendHostToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendHostToBrute, &QAction::triggered, this, [=](){emit sendResultsToBrute(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToBrute();});
    connect(&a_SendHostToActive, &QAction::triggered, this, [=](){emit sendResultsToActive(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToActive();});
    connect(&a_SendHostToDns, &QAction::triggered, this, [=](){emit sendResultsToDns(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToDns();});
    connect(&a_SendHostToCert, &QAction::triggered, this, [=](){emit sendResultsToCert(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToCert();});
    connect(&a_SendIpToIpTool, &QAction::triggered, this, [=](){emit sendResultsToIpTool(ENGINE::ACTIVE, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendHostToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendHostToDomainTool, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(ENGINE::ACTIVE, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    /* ... */
    connect(&a_SendSelectedIpToIp, &QAction::triggered, this, [=](){emit sendResultsToIp(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToIp();});
    connect(&a_SendSelectedIpToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendSelectedIpToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToOsint, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendSelectedHostToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToBrute, &QAction::triggered, this, [=](){emit sendResultsToBrute(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToBrute();});
    connect(&a_SendSelectedHostToActive, &QAction::triggered, this, [=](){emit sendResultsToActive(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToActive();});
    connect(&a_SendSelectedHostToDns, &QAction::triggered, this, [=](){emit sendResultsToDns(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToDns();});
    connect(&a_SendSelectedHostToCert, &QAction::triggered, this, [=](){emit sendResultsToCert(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToCert();});
    connect(&a_SendSelectedIpToIpTool, &QAction::triggered, this, [=](){emit sendResultsToIpTool(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendSelectedHostToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendSelectedHostToDomainTool, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    /* ... */
    connect(&a_Save, &QAction::triggered, this, [=](){this->onSaveResults(selectionModel);});
    connect(&a_SaveSubdomainIp, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::SUBDOMAINIP);});
    connect(&a_SaveSubdomain, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::SUBDOMAIN);});
    connect(&a_SaveIp, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::IP);});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->onCopyResults(selectionModel);});
    connect(&a_CopySubdomainIp, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::SUBDOMAINIP);});
    connect(&a_CopySubdomain, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::SUBDOMAIN);});
    connect(&a_CopyIp, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::IP);});
}

void Active::on_buttonAction_clicked(){
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
    saveMenu->addAction(&a_SaveSubdomainIp);
    saveMenu->addAction(&a_SaveSubdomain);
    saveMenu->addAction(&a_SaveIp);
    copyMenu->addAction(&a_CopySubdomainIp);
    copyMenu->addAction(&a_CopySubdomain);
    copyMenu->addAction(&a_CopyIp);
    sendToEngineMenu->addAction(&a_SendIpToIp);
    sendToEngineMenu->addAction(&a_SendIpToOsint);
    sendToEngineMenu->addAction(&a_SendIpToRaw);
    sendToEngineMenu->addSeparator();
    sendToEngineMenu->addAction(&a_SendHostToOsint);
    sendToEngineMenu->addAction(&a_SendHostToRaw);
    sendToEngineMenu->addAction(&a_SendHostToBrute);
    sendToEngineMenu->addAction(&a_SendHostToActive);
    sendToEngineMenu->addAction(&a_SendHostToDns);
    sendToEngineMenu->addAction(&a_SendHostToCert);
    sendToToolMenu->addAction(&a_SendIpToIpTool);
    sendToToolMenu->addAction(&a_SendHostToCertTool);
    sendToToolMenu->addAction(&a_SendHostToDomainTool);
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

void Active::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex())){
        return;
    }

    /* getting the selected items... */
    selectionModel = ui->tableViewResults->selectionModel();

    /* creating the context menu... */
    QMenu *mainMenu = new QMenu(this);
    QMenu *sendToToolMenu = new QMenu(this);
    QMenu *sendToEngineMenu = new QMenu(this);
    sendToToolMenu->setTitle("Send To Tool");
    sendToEngineMenu->setTitle("Send To Engine");
    mainMenu->setAttribute(Qt::WA_DeleteOnClose, true);

    /* adding actions */
    sendToEngineMenu->addAction(&a_SendSelectedIpToIp);
    sendToEngineMenu->addAction(&a_SendSelectedIpToOsint);
    sendToEngineMenu->addAction(&a_SendSelectedIpToRaw);
    sendToEngineMenu->addSeparator();
    sendToEngineMenu->addAction(&a_SendSelectedHostToOsint);
    sendToEngineMenu->addAction(&a_SendSelectedHostToRaw);
    sendToEngineMenu->addAction(&a_SendSelectedHostToBrute);
    sendToEngineMenu->addAction(&a_SendSelectedHostToActive);
    sendToEngineMenu->addAction(&a_SendSelectedHostToDns);
    sendToEngineMenu->addAction(&a_SendSelectedHostToCert);
    sendToToolMenu->addAction(&a_SendSelectedIpToIpTool);
    sendToToolMenu->addAction(&a_SendSelectedHostToCertTool);
    sendToToolMenu->addAction(&a_SendSelectedHostToDomainTool);
    /* adding to mainMenu */
    mainMenu->addAction(&a_RemoveResults);
    mainMenu->addAction(&a_OpenInBrowser);
    mainMenu->addSeparator();
    mainMenu->addAction(&a_Save);
    mainMenu->addAction(&a_Copy);
    mainMenu->addSeparator();
    mainMenu->addMenu(sendToEngineMenu);
    mainMenu->addMenu(sendToToolMenu);

    /* showing the context menu... */
    mainMenu->exec(QCursor::pos());
}

void Active::onClearResults(){
    /* clear the results... */
    result->active->subdomainIp->clear();
    ui->labelResultsCount->clear();
    result->active->subdomainIp->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Active::onOpenInBrowser(QItemSelectionModel *selectionModel){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
}

void Active::onRemoveResults(QItemSelectionModel *selectionModel){
    foreach(const QModelIndex &proxyIndex, selectionModel->selectedIndexes()){
        auto index = result->active->subdomainIpProxy->mapToSource(proxyIndex);
        result->active->subdomainIp->removeRow(index.row());
    }

    ui->labelResultsCount->setNum(result->active->subdomainIpProxy->rowCount());
}

void Active::onSaveResults(RESULT_TYPE resultType){
    /* checks... */
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen())
        return;

    /* writing to file */
    QString item;
    switch(resultType){
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    case RESULT_TYPE::IP:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    case RESULT_TYPE::SUBDOMAINIP:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString()+":"+result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            file.write(item.toUtf8());
        }
        break;

    default:
        break;
    }

    file.close();
}

void Active::onSaveResults(QItemSelectionModel *selectionModel){
    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty())
        return;

    /* saving to file */
    QString data;
    QString item;
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            item = index.data().toString();
            data.append(item.append(NEWLINE));
        }
        file.write(data.toUtf8());
        file.close();
    }
}


void Active::onCopyResults(RESULT_TYPE resultType){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString clipboardData;
    QString item;

    /* copying result type */
    switch(resultType){
    case RESULT_TYPE::SUBDOMAIN:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case RESULT_TYPE::IP:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    case RESULT_TYPE::SUBDOMAINIP:
        for(int i = 0; i != result->active->subdomainIpProxy->rowCount(); ++i)
        {
            item = result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 0)).toString()+"|"+result->active->subdomainIpProxy->data(result->active->subdomainIpProxy->index(i, 1)).toString().append(NEWLINE);
            clipboardData.append(item);
        }
        break;

    default:
        break;
    }

    clipboard->setText(clipboardData.trimmed());
}

void Active::onCopyResults(QItemSelectionModel *selectionModel){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;
    QString item;

    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        data.append(item.append(NEWLINE));
    }

    clipboard->setText(data.trimmed());
}

void Active::onReceiveTargets(ENGINE engine, RESULT_TYPE resultType, RESULT_MODEL_TYPE resultModelType){
    Q_UNUSED(resultType);

    QSortFilterProxyModel *model = nullptr;

    switch (resultModelType) {
    case RESULT_MODEL_TYPE::SUBDOMAINIP:
        switch(engine){
            case ENGINE::OSINT:
                model = result->osint->subdomainIpProxy;
                break;
            case ENGINE::BRUTE:
                model = result->brute->subdomainIpProxy;
                break;
            case ENGINE::ACTIVE:
                model = result->active->subdomainIpProxy;
                break;
            case ENGINE::IP:
                model = result->ip->subdomainIpProxy;
                break;
            default:
                return;
        }
        for(int i = 0; i < model->rowCount(); i++)
            ui->targets->add(model->data(model->index(i, 0)).toString());
        break;

    case RESULT_MODEL_TYPE::SUBDOMAIN:
        switch(engine){
            case ENGINE::OSINT:
                model = result->osint->subdomainProxy;
                break;
            case ENGINE::CERT:
                model = result->cert->subdomainProxy;
                break;
            default:
                return;
        }
        for(int i = 0; i < model->rowCount(); i++)
            ui->targets->add(model->data(model->index(i, 0)).toString());
        break;

    case RESULT_MODEL_TYPE::TLD:
        model = result->brute->tldProxy;
        for(int i = 0; i < model->rowCount(); i++)
            ui->targets->add(model->data(model->index(i, 0)).toString());
        break;

    case RESULT_MODEL_TYPE::DNS:
    {
        model = result->records->dnsProxy;

        QModelIndexList domains;
        for(int i = 0; i < model->rowCount(); ++i)
            domains << model->index( i, 0 );

        for ( int i = 0; i < domains.size(); ++i )
        {
            for ( int j = 0; j < model->rowCount( domains[i] ); ++j )
            {
                QString type = domains[i].child(j, 0).data().toString();
                if(type == "CNAME" || type == "NS" || type == "MX")
                {
                    for(int k = 0; k < model->rowCount(domains[i].child(j, 0)); k++)
                    {
                        QString domain = domains[i].child(j, 0).child(k, 0).data().toString();
                        ui->targets->add(domain);
                    }
                }
            }
        }
        break;
    }

    default:
        break;
    }
}

void Active::onReceiveTargets(QItemSelectionModel *selectionModel, RESULT_TYPE, RESULT_MODEL_TYPE){
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        ui->targets->add(index.data().toString());
}

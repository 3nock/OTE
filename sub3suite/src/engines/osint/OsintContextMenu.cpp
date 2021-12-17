#include "Osint.h"
#include "ui_Osint.h"

#include <QClipboard>


void Osint::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->onClearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->onRemoveResults(selectionModel);});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->onOpenInBrowser(selectionModel);});
    /* ... */
    connect(&a_SendIpToIpFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToIp(ENGINE::OSINT, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToIp();});
    connect(&a_SendIpToIpFromIp, &QAction::triggered, this, [=](){emit sendResultsToIp(ENGINE::OSINT, RESULT_TYPE::IP, RESULT_MODEL_TYPE::IP); emit changeTabToIp();});
    connect(&a_SendIpToOsintFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::OSINT, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendIpToOsintFromIp, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::OSINT, RESULT_TYPE::IP, RESULT_MODEL_TYPE::IP); emit changeTabToOsint();});
    connect(&a_SendIpToRawFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::OSINT, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendIpToRawFromIp, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::OSINT, RESULT_TYPE::IP, RESULT_MODEL_TYPE::IP); emit changeTabToRaw();});
    connect(&a_SendHostToOsintFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendHostToOsintFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToOsint(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToOsint();});
    connect(&a_SendHostToRawFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendHostToRawFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToRaw();});
    connect(&a_SendHostToBruteFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToBrute(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToBrute();});
    connect(&a_SendHostToBruteFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToBrute(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToBrute();});
    connect(&a_SendHostToActiveFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToActive(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToActive();});
    connect(&a_SendHostToActiveFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToActive(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToActive();});
    connect(&a_SendHostToDnsFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToDns(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToDns();});
    connect(&a_SendHostToDnsFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToDns(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToDns();});
    connect(&a_SendHostToCertFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToCert(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToCert();});
    connect(&a_SendHostToCertFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToCert(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToCert();});
    connect(&a_SendEmailToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::OSINT, RESULT_TYPE::EMAIL, RESULT_MODEL_TYPE::EMAIL); emit changeTabToRaw();});
    connect(&a_SendUrlToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::OSINT, RESULT_TYPE::URL, RESULT_MODEL_TYPE::URL); emit changeTabToRaw();});
    connect(&a_SendAsnToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::OSINT, RESULT_TYPE::ASN, RESULT_MODEL_TYPE::ASN); emit changeTabToRaw();});
    connect(&a_SendCidrToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::OSINT, RESULT_TYPE::CIDR, RESULT_MODEL_TYPE::CIDR); emit changeTabToRaw();});
    connect(&a_SendCertToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(ENGINE::OSINT, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::CERT_ID); emit changeTabToRaw();});
    connect(&a_SendIpToIpToolFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToIpTool(ENGINE::OSINT, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendIpToIpToolFromIp, &QAction::triggered, this, [=](){emit sendResultsToIpTool(ENGINE::OSINT, RESULT_TYPE::IP, RESULT_MODEL_TYPE::IP);});
    connect(&a_SendHostToCertToolFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToCertTool(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendHostToCertToolFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToCertTool(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN);});
    connect(&a_SendHostToDomainToolFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendHostToDomainToolFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(ENGINE::OSINT, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN);});
    connect(&a_SendAsnToAsnTool, &QAction::triggered, this, [=](){emit sendResultsToASNTool(ENGINE::OSINT, RESULT_TYPE::ASN, RESULT_MODEL_TYPE::ASN);});
    connect(&a_SendEmailToEmailTool, &QAction::triggered, this, [=](){emit sendResultsToEmailTool(ENGINE::OSINT, RESULT_TYPE::EMAIL, RESULT_MODEL_TYPE::EMAIL);});
    connect(&a_SendCidrToCidrTool, &QAction::triggered, this, [=](){emit sendResultsToCidrTool(ENGINE::OSINT, RESULT_TYPE::CIDR, RESULT_MODEL_TYPE::CIDR);});
    connect(&a_SendCertToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(ENGINE::OSINT, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::CERT_ID);});
    /* ... */
    connect(&a_SendSelectedIpToIpFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToIp(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToIp();});
    connect(&a_SendSelectedIpToIpFromIp, &QAction::triggered, this, [=](){emit sendResultsToIp(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::IP); emit changeTabToIp();});
    connect(&a_SendSelectedIpToOsintFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendSelectedIpToOsintFromIp, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::IP); emit changeTabToOsint();});
    connect(&a_SendSelectedIpToRawFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendSelectedIpToRawFromIp, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::IP); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToOsintFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToOsint();});
    connect(&a_SendSelectedHostToOsintFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToOsint(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToOsint();});
    connect(&a_SendSelectedHostToRawFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToRawFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToRaw();});
    connect(&a_SendSelectedHostToBruteFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToBrute(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToBrute();});
    connect(&a_SendSelectedHostToBruteFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToBrute(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToBrute();});
    connect(&a_SendSelectedHostToActiveFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToActive(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToActive();});
    connect(&a_SendSelectedHostToActiveFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToActive(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToActive();});
    connect(&a_SendSelectedHostToDnsFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToDns(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToDns();});
    connect(&a_SendSelectedHostToDnsFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToDns(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToDns();});
    connect(&a_SendSelectedHostToCertFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToCert(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP); emit changeTabToCert();});
    connect(&a_SendSelectedHostToCertFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToCert(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN); emit changeTabToCert();});
    connect(&a_SendSelectedEmailToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::EMAIL, RESULT_MODEL_TYPE::EMAIL); emit changeTabToRaw();});
    connect(&a_SendSelectedUrlToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::URL, RESULT_MODEL_TYPE::URL); emit changeTabToRaw();});
    connect(&a_SendSelectedAsnToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::ASN, RESULT_MODEL_TYPE::ASN); emit changeTabToRaw();});
    connect(&a_SendSelectedCidrToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::CIDR, RESULT_MODEL_TYPE::CIDR); emit changeTabToRaw();});
    connect(&a_SendSelectedCertToRaw, &QAction::triggered, this, [=](){emit sendResultsToRaw(selectionModel, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::CERT_ID); emit changeTabToRaw();});
    connect(&a_SendSelectedIpToIpToolFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToIpTool(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendSelectedIpToIpToolFromIp, &QAction::triggered, this, [=](){emit sendResultsToIpTool(selectionModel, RESULT_TYPE::IP, RESULT_MODEL_TYPE::IP);});
    connect(&a_SendSelectedHostToCertToolFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToCertTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendSelectedHostToCertToolFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToCertTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN);});
    connect(&a_SendSelectedHostToDomainToolFromSubdomainIp, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SendSelectedHostToDomainToolFromSubdomain, &QAction::triggered, this, [=](){emit sendResultsToDomainTool(selectionModel, RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN);});
    connect(&a_SendSelectedAsnToAsnTool, &QAction::triggered, this, [=](){emit sendResultsToASNTool(selectionModel, RESULT_TYPE::ASN, RESULT_MODEL_TYPE::ASN);});
    connect(&a_SendSelectedEmailToEmailTool, &QAction::triggered, this, [=](){emit sendResultsToEmailTool(selectionModel, RESULT_TYPE::EMAIL, RESULT_MODEL_TYPE::EMAIL);});
    connect(&a_SendSelectedCidrToCidrTool, &QAction::triggered, this, [=](){emit sendResultsToCidrTool(selectionModel, RESULT_TYPE::CIDR, RESULT_MODEL_TYPE::CIDR);});
    connect(&a_SendSelectedCertToCertTool, &QAction::triggered, this, [=](){emit sendResultsToCertTool(selectionModel, RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::CERT_ID);});
    /* ... */
    connect(&a_Save, &QAction::triggered, this, [=](){this->onSaveResults(selectionModel);});
    connect(&a_SaveSubdomainIp, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::SUBDOMAINIP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SaveSubdomainFromSubdomainIp, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SaveSubdomainFromSubdomain, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN);});
    connect(&a_SaveIpFromSubdomainIp, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_SaveIpFromIp, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::IP, RESULT_MODEL_TYPE::IP);});
    connect(&a_SaveEmail, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::EMAIL, RESULT_MODEL_TYPE::EMAIL);});
    connect(&a_SaveUrl, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::URL, RESULT_MODEL_TYPE::URL);});
    connect(&a_SaveAsn, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::ASN, RESULT_MODEL_TYPE::ASN);});
    connect(&a_SaveCidr, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::CIDR, RESULT_MODEL_TYPE::CIDR);});
    connect(&a_SaveCert, &QAction::triggered, this, [=](){this->onSaveResults(RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::CERT_ID);});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->onCopyResults(selectionModel);});
    connect(&a_CopySubdomainIp, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::SUBDOMAINIP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_CopySubdomainFromSubdomainIp, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_CopySubdomainFromSubdomain, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::SUBDOMAIN, RESULT_MODEL_TYPE::SUBDOMAIN);});
    connect(&a_CopyIpFromSubdomainIp, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::IP, RESULT_MODEL_TYPE::SUBDOMAINIP);});
    connect(&a_CopyIpFromIp, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::IP, RESULT_MODEL_TYPE::IP);});
    connect(&a_CopyEmail, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::EMAIL, RESULT_MODEL_TYPE::EMAIL);});
    connect(&a_CopyUrl, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::URL, RESULT_MODEL_TYPE::URL);});
    connect(&a_CopyAsn, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::ASN, RESULT_MODEL_TYPE::ASN);});
    connect(&a_CopyCidr, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::CIDR, RESULT_MODEL_TYPE::CIDR);});
    connect(&a_CopyCert, &QAction::triggered, this, [=](){this->onCopyResults(RESULT_TYPE::CERT_ID, RESULT_MODEL_TYPE::CERT_ID);});
}

void Osint::on_buttonAction_clicked(){
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
        saveMenu->addAction(&a_SaveSubdomainFromSubdomain);
        copyMenu->addAction(&a_CopySubdomainFromSubdomain);
        sendToEngineMenu->addAction(&a_SendHostToOsintFromSubdomain);
        sendToEngineMenu->addAction(&a_SendHostToRawFromSubdomain);
        sendToEngineMenu->addAction(&a_SendHostToBruteFromSubdomain);
        sendToEngineMenu->addAction(&a_SendHostToActiveFromSubdomain);
        sendToEngineMenu->addAction(&a_SendHostToDnsFromSubdomain);
        sendToEngineMenu->addAction(&a_SendHostToCertFromSubdomain);
        sendToToolMenu->addAction(&a_SendHostToCertToolFromSubdomain);
        sendToToolMenu->addAction(&a_SendHostToDomainToolFromSubdomain);
    }
    if(ui->comboBoxOutput->currentIndex() == 1){
        saveMenu->addAction(&a_SaveSubdomainIp);
        saveMenu->addAction(&a_SaveSubdomainFromSubdomainIp);
        saveMenu->addAction(&a_SaveIpFromSubdomainIp);
        copyMenu->addAction(&a_CopySubdomainIp);
        copyMenu->addAction(&a_CopySubdomainFromSubdomainIp);
        copyMenu->addAction(&a_CopyIpFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendIpToIpFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendIpToOsintFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendIpToRawFromSubdomainIp);
        sendToEngineMenu->addSeparator();
        sendToEngineMenu->addAction(&a_SendHostToOsintFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendHostToRawFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendHostToBruteFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendHostToActiveFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendHostToDnsFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendHostToCertFromSubdomainIp);
        sendToToolMenu->addAction(&a_SendIpToIpToolFromSubdomainIp);
        sendToToolMenu->addAction(&a_SendHostToCertToolFromSubdomainIp);
        sendToToolMenu->addAction(&a_SendHostToDomainToolFromSubdomainIp);
    }
    if(ui->comboBoxOutput->currentIndex() == 2){
        saveMenu->addAction(&a_SaveIpFromIp);
        copyMenu->addAction(&a_CopyIpFromIp);
        sendToEngineMenu->addAction(&a_SendIpToIpFromIp);
        sendToEngineMenu->addAction(&a_SendIpToOsintFromIp);
        sendToEngineMenu->addAction(&a_SendIpToRawFromIp);
        sendToToolMenu->addAction(&a_SendIpToIpToolFromIp);
    }
    if(ui->comboBoxOutput->currentIndex() == 3){
        saveMenu->addAction(&a_SaveEmail);
        copyMenu->addAction(&a_CopyEmail);
        //sendToEngineMenu->addAction(&a_SendEmailToOsint);
        sendToEngineMenu->addAction(&a_SendEmailToRaw);
        sendToToolMenu->addAction(&a_SendEmailToEmailTool);
    }
    if(ui->comboBoxOutput->currentIndex() == 4){
        saveMenu->addAction(&a_SaveUrl);
        copyMenu->addAction(&a_CopyUrl);
        sendToEngineMenu->addAction(&a_SendUrlToRaw);
    }
    if(ui->comboBoxOutput->currentIndex() == 5){
        saveMenu->addAction(&a_SaveAsn);
        copyMenu->addAction(&a_CopyAsn);
        //sendToEngineMenu->addAction(&a_SendAsnToOsint);
        sendToEngineMenu->addAction(&a_SendAsnToRaw);
        sendToToolMenu->addAction(&a_SendAsnToAsnTool);
    }
    if(ui->comboBoxOutput->currentIndex() == 6){
        saveMenu->addAction(&a_SaveCert);
        copyMenu->addAction(&a_CopyCert);
        //sendToEngineMenu->addAction(&a_SendCertToOsint);
        sendToEngineMenu->addAction(&a_SendCertToRaw);
        sendToToolMenu->addAction(&a_SendCertToCertTool);
    }
    if(ui->comboBoxOutput->currentIndex() == 7){
        saveMenu->addAction(&a_SaveCidr);
        copyMenu->addAction(&a_CopyCidr);
        //sendToEngineMenu->addAction(&a_SendCidrToOsint);
        sendToEngineMenu->addAction(&a_SendCidrToRaw);
        sendToToolMenu->addAction(&a_SendCidrToCidrTool);
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

void Osint::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
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
    saveMenu->addAction(&a_Save);
    copyMenu->addAction(&a_Copy);

    if(ui->comboBoxOutput->currentIndex() == 0){
        sendToEngineMenu->addAction(&a_SendSelectedHostToOsintFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedHostToRawFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedHostToBruteFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedHostToActiveFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedHostToDnsFromSubdomain);
        sendToEngineMenu->addAction(&a_SendSelectedHostToCertFromSubdomain);
        sendToToolMenu->addAction(&a_SendSelectedHostToCertToolFromSubdomain);
        sendToToolMenu->addAction(&a_SendSelectedHostToDomainToolFromSubdomain);
    }
    if(ui->comboBoxOutput->currentIndex() == 1){
        sendToEngineMenu->addAction(&a_SendSelectedIpToIpFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendSelectedIpToOsintFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendSelectedIpToRawFromSubdomainIp);
        sendToEngineMenu->addSeparator();
        sendToEngineMenu->addAction(&a_SendSelectedHostToOsintFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendSelectedHostToRawFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendSelectedHostToBruteFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendSelectedHostToActiveFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendSelectedHostToDnsFromSubdomainIp);
        sendToEngineMenu->addAction(&a_SendSelectedHostToCertFromSubdomainIp);
        sendToToolMenu->addAction(&a_SendSelectedIpToIpToolFromSubdomainIp);
        sendToToolMenu->addAction(&a_SendSelectedHostToCertToolFromSubdomainIp);
        sendToToolMenu->addAction(&a_SendSelectedHostToDomainToolFromSubdomainIp);
    }
    if(ui->comboBoxOutput->currentIndex() == 2){
        sendToEngineMenu->addAction(&a_SendSelectedIpToIpFromIp);
        sendToEngineMenu->addAction(&a_SendSelectedIpToOsintFromIp);
        sendToEngineMenu->addAction(&a_SendSelectedIpToRawFromIp);
        sendToToolMenu->addAction(&a_SendSelectedIpToIpToolFromIp);
    }
    if(ui->comboBoxOutput->currentIndex() == 3){
        //sendToEngineMenu->addAction(&a_SendSelectedEmailToOsint);
        sendToEngineMenu->addAction(&a_SendSelectedEmailToRaw);
        sendToToolMenu->addAction(&a_SendSelectedEmailToEmailTool);
    }
    if(ui->comboBoxOutput->currentIndex() == 4){
        sendToEngineMenu->addAction(&a_SendSelectedUrlToRaw);
    }
    if(ui->comboBoxOutput->currentIndex() == 5){
        //sendToEngineMenu->addAction(&a_SendSelectedAsnToOsint);
        sendToEngineMenu->addAction(&a_SendSelectedAsnToRaw);
        sendToToolMenu->addAction(&a_SendSelectedAsnToAsnTool);
    }
    if(ui->comboBoxOutput->currentIndex() == 6){
        //sendToEngineMenu->addAction(&a_SendSelectedCertToOsint);
        sendToEngineMenu->addAction(&a_SendSelectedCertToRaw);
        sendToToolMenu->addAction(&a_SendSelectedCertToCertTool);
    }
    if(ui->comboBoxOutput->currentIndex() == 7){
        //sendToEngineMenu->addAction(&a_SendSelectedCidrToOsint);
        sendToEngineMenu->addAction(&a_SendSelectedCidrToRaw);
        sendToToolMenu->addAction(&a_SendSelectedCidrToCidrTool);
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

void Osint::onOpenInBrowser(QItemSelectionModel *){

}

void Osint::onRemoveResults(QItemSelectionModel *selectionModel){
    Q_UNUSED(selectionModel);
}

void Osint::onSaveResults(RESULT_TYPE resultType, RESULT_MODEL_TYPE){
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

void Osint::onSaveResults(QItemSelectionModel *selectionModel){
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


void Osint::onCopyResults(RESULT_TYPE resultType, RESULT_MODEL_TYPE){
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

void Osint::onCopyResults(QItemSelectionModel *selectionModel){
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

void Osint::onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void Osint::onReceiveTargets(QItemSelectionModel *, RESULT_TYPE, RESULT_MODEL_TYPE){

}

/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Raw.h"
#include "ui_Raw.h"

#include <QClipboard>
#include <QDesktopServices>


void Raw::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->m_clearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->m_removeResults(selectionModel);});
    connect(&a_ExpandResults, &QAction::triggered, this, [=](){this->m_expandResults();});
    connect(&a_CollapseResults, &QAction::triggered, this, [=](){this->m_collapseResults();});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->m_openInBrowser(selectionModel);});
    /* ... */
    connect(&a_SendAllIpToIp, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::IP);});
    connect(&a_SendAllIpToOsint, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::OSINT);});
    connect(&a_SendAllIpToRaw, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::RAW);});
    connect(&a_SendAllHostToOsint, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::OSINT);});
    connect(&a_SendAllHostToRaw, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::RAW);});
    connect(&a_SendAllHostToBrute, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::BRUTE);});
    connect(&a_SendAllHostToActive, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::ACTIVE);});
    connect(&a_SendAllHostToDns, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::DNS);});
    connect(&a_SendAllHostToCert, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::CERT);});
    connect(&a_SendAllEmailToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW);});
    connect(&a_SendAllUrlToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW);});
    connect(&a_SendAllAsnToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW);});
    connect(&a_SendAllCidrToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW);});
    connect(&a_SendAllCertToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW);});
    connect(&a_SendAllEmailToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT);});
    connect(&a_SendAllAsnToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT);});
    connect(&a_SendAllCidrToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT);});
    connect(&a_SendAllCertToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT);});
    connect(&a_SendAllIpToIpTool, &QAction::triggered, this, [=](){this->m_sendIpToTool(TOOL::IP);});
    connect(&a_SendAllHostToCertTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::DOMAINTOOL);});
    connect(&a_SendAllHostToDomainTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::CERT);});
    connect(&a_SendAllAsnToAsnTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::ASN);});
    connect(&a_SendAllEmailToEmailTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::EMAIL);});
    connect(&a_SendAllCidrToCidrTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::CIDR);});
    connect(&a_SendAllCertToCertTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::CERT);});
    /* ... */
    connect(&a_SendSelectedIpToIp, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::IP, selectionModel);});
    connect(&a_SendSelectedIpToOsint, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedIpToRaw, &QAction::triggered, this, [=](){this->m_sendIpToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedHostToOsint, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedHostToRaw, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedHostToBrute, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::BRUTE, selectionModel);});
    connect(&a_SendSelectedHostToActive, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::ACTIVE, selectionModel);});
    connect(&a_SendSelectedHostToDns, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::DNS, selectionModel);});
    connect(&a_SendSelectedHostToCert, &QAction::triggered, this, [=](){this->m_sendSubdomainToEngine(ENGINE::CERT, selectionModel);});
    connect(&a_SendSelectedEmailToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedUrlToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedAsnToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedCidrToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedCertToRaw, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedEmailToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedAsnToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedCidrToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedCertToOsint, &QAction::triggered, this, [=](){this->m_sendToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedIpToIpTool, &QAction::triggered, this, [=](){this->m_sendIpToTool(TOOL::IP, selectionModel);});
    connect(&a_SendSelectedHostToCertTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::DOMAINTOOL, selectionModel);});
    connect(&a_SendSelectedHostToDomainTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::CERT, selectionModel);});
    connect(&a_SendSelectedAsnToAsnTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::ASN, selectionModel);});
    connect(&a_SendSelectedEmailToEmailTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::EMAIL, selectionModel);});
    connect(&a_SendSelectedCidrToCidrTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::CIDR, selectionModel);});
    connect(&a_SendSelectedCertToCertTool, &QAction::triggered, this, [=](){this->m_sendToTool(TOOL::CERT, selectionModel);});
    /* ... */
    connect(&a_Save, &QAction::triggered, this, [=](){this->m_saveResults(selectionModel);});
    connect(&a_SaveJson, &QAction::triggered, this, [=](){this->m_saveResultsJson();});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->m_copyResults(selectionModel);});
    connect(&a_CopyJson, &QAction::triggered, this, [=](){this->m_copyResultsJson();});
}

void Raw::on_buttonActionJson_clicked(){
    /*
     nothing yet
     */
}

void Raw::on_buttonActionTree_clicked(){
    /*
     nothing yet
     */
}

void Raw::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /*
     nothing yet
     */
}

void Raw::m_clearResults(){
    /* clear the results... */
    ui->plainTextEditResults->clear();
    m_model->clear();
    m_model->setHorizontalHeaderLabels({"Key", "Value"});
    ui->labelResultsCount->clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void Raw::m_expandResults(){
    ui->treeViewResults->expandAll();
}

void Raw::m_collapseResults(){
    ui->treeViewResults->collapseAll();
}

void Raw::m_saveResultsJson(){

}

void Raw::m_openInBrowser(QItemSelectionModel *selectionModel){
    Q_UNUSED(selectionModel);
}

void Raw::m_removeResults(QItemSelectionModel *selectionModel){
    Q_UNUSED(selectionModel);
}

void Raw::m_saveResults(RESULT_TYPE resultType){
    Q_UNUSED(resultType);
}

void Raw::m_saveResults(QItemSelectionModel *selectionModel){
    Q_UNUSED(selectionModel);
}

void Raw::m_copyResultsJson(){

}


void Raw::m_copyResults(RESULT_TYPE resultType){
    Q_UNUSED(resultType);
}

void Raw::m_copyResults(QItemSelectionModel *selectionModel){
    Q_UNUSED(selectionModel);
}

void Raw::m_sendToEngine(ENGINE){

}

void Raw::m_sendSubdomainToEngine(ENGINE){

}

void Raw::m_sendIpToEngine(ENGINE){

}

void Raw::m_sendSubdomainToTool(TOOL){

}

void Raw::m_sendIpToTool(TOOL){

}

void Raw::m_sendToTool(TOOL){

}

void Raw::m_sendToEngine(ENGINE, QItemSelectionModel *){

}

void Raw::m_sendSubdomainToEngine(ENGINE, QItemSelectionModel *){

}

void Raw::m_sendIpToEngine(ENGINE, QItemSelectionModel *){

}

void Raw::m_sendSubdomainToTool(TOOL, QItemSelectionModel *){

}

void Raw::m_sendIpToTool(TOOL, QItemSelectionModel *){

}

void Raw::m_sendToTool(TOOL, QItemSelectionModel *){

}

void Raw::onReceiveTargets(QString target, RESULT_TYPE resultType){
    Q_UNUSED(target);
    Q_UNUSED(resultType);
}

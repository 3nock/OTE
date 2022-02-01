/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Raw.h"
#include "ui_Raw.h"

#include <QClipboard>
#include <QDesktopServices>


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

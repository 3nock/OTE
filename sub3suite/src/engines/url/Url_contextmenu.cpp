/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Url.h"
#include "ui_Url.h"


void Url::m_initActions(){
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->m_clearResults();});
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->m_removeResults(selectionModel);});
    connect(&a_OpenInBrowser, &QAction::triggered, this, [=](){this->m_openInBrowser(selectionModel);});
    connect(&a_SendAllToProject, &QAction::triggered, this, [=](){this->m_sendToProject();});
    connect(&a_SendSelectedToProject, &QAction::triggered, this, [=](){this->m_sendToProject(selectionModel);});
    connect(&a_Save, &QAction::triggered, this, [=](){this->m_saveResults(selectionModel);});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->m_copyResults(selectionModel);});

}

void Url::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(m_resultProxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+63, pos.y());

    /* creating the context menu... */
    QMenu menu(this);
    /* adding to mainMenu */
    menu.addAction(&a_ClearResults);
    menu.addSeparator();
    menu.addAction(&a_Save);
    menu.addAction(&a_Copy);
    menu.addSeparator();
    menu.addAction(&a_SendAllToProject);

    /* showing the context menu... */
    menu.exec(pos);
}

void Url::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex()))
        return;

    /* getting the selected items... */
    selectionModel = ui->tableViewResults->selectionModel();

    /* creating the context menu... */
    QMenu menu(this);

    /* adding to mainMenu */
    menu.addAction(&a_RemoveResults);
    menu.addAction(&a_OpenInBrowser);
    menu.addSeparator();
    menu.addAction(&a_Save);
    menu.addAction(&a_Copy);
    menu.addSeparator();
    menu.addAction(&a_SendSelectedToProject);

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}

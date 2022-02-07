/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "NSEnum.h"
#include "ui_NSEnum.h"

#include <QMenu>


void NSEnum::initActions(){
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->m_removeResults(selectionModel);});
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->m_clearResults();});
    connect(&a_ExpandResults, &QAction::triggered, this, [=](){ui->treeResults->expandAll();});
    connect(&a_CollapseResults, &QAction::triggered, this, [=](){ui->treeResults->collapseAll();});
    connect(&a_Save, &QAction::triggered, this, [=](){this->m_saveResults();});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->m_copyResults();});
    /* for all */
    connect(&a_SendAllToProject, &QAction::triggered, this, [=](){this->m_sendToProject();});
    /* for selected */
    connect(&a_SendSelectedToProject, &QAction::triggered, this, [=](){this->m_sendToProject(selectionModel);});
}

void NSEnum::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(proxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+63, pos.y());

    /* creating the context menu... */
    QMenu menu(this);
    /* adding to mainMenu */
    menu.addAction(&a_ClearResults);
    menu.addAction(&a_ExpandResults);
    menu.addAction(&a_CollapseResults);
    menu.addSeparator();
    menu.addAction(&a_Save);
    menu.addAction(&a_Copy);
    menu.addSeparator();
    menu.addAction(&a_SendAllToProject);

    /* showing the context menu... */
    menu.exec(pos);
}

void NSEnum::on_treeResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->treeResults->selectionModel()->isSelected(ui->treeResults->currentIndex()))
        return;

    /* getting the selected items... */
    selectionModel = ui->treeResults->selectionModel();

    /* creating the context menu... */
    QMenu menu(this);

    /* adding to mainMenu */
    menu.addAction(&a_RemoveResults);
    menu.addSeparator();
    menu.addAction(&a_Save);
    menu.addAction(&a_Copy);
    menu.addSeparator();
    menu.addAction(&a_SendSelectedToProject);

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}

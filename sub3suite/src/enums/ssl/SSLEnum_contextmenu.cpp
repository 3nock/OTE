/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "SSLEnum.h"
#include "ui_SSLEnum.h"

#include <QMenu>


void SSLEnum::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(proxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    /* creating the context menu... */
    QMenu menu(this);

    /* adding actions */
    menu.addAction(tr("Clear"), this, [=](){this->clearResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addAction(tr("Exapand"), this, [=](){ui->treeViewResults->expandAll();})->setIcon(QIcon(":/img/res/icons/expand.png"));
    menu.addAction(tr("Collapse"), this, [=](){ui->treeViewResults->collapseAll();})->setIcon(QIcon(":/img/res/icons/collapse.png"));
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveResults();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->copyResults();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));

    /* showing the context menu... */
    menu.exec(pos);
}

void SSLEnum::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->treeViewResults->selectionModel()->isSelected(ui->treeViewResults->currentIndex()))
        return;

    /* getting the selected items... */
    selectionModel = ui->treeViewResults->selectionModel();

    /* creating the context menu... */
    QMenu menu(this);

    /* adding actions */
    menu.addAction(tr("Remove"), this, [=](){this->clearResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveSelectedResults();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->copySelectedResults();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendSelectedToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}

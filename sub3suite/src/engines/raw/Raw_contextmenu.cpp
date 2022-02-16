/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Raw.h"
#include "ui_Raw.h"


void Raw::on_buttonActionJson_clicked(){
    /* check if there are results available else dont show the context menu */
    if(proxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonActionJson->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    /* creating the context menu... */
    QMenu menu(this);

    /* adding actions */
    menu.addAction(tr("Clear"), this, [=](){this->clearResults_txt();});
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveResults_txt();});
    menu.addAction(tr("Copy"), this, [=](){this->copyResults_txt();});

    /* showing the context menu... */
    menu.exec(pos);
}

void Raw::on_buttonActionTree_clicked(){
    /* check if there are results available else dont show the context menu */
    if(proxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonActionTree->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+65, pos.y());

    /* creating the context menu... */
    QMenu menu(this);

    /* adding actions */
    menu.addAction(tr("Clear"), this, [=](){this->clearResults();});
    menu.addAction(tr("Expand"), this, [=](){ui->treeViewResults->expandAll();});
    menu.addAction(tr("Collapse"), this, [=](){ui->treeViewResults->collapseAll();});
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveResults();});
    menu.addAction(tr("Copy"), this, [=](){this->copyResults();});
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();});
    menu.addAction(tr("Send To Engine"), this, [=](){this->sendToEngine();});
    menu.addAction(tr("Send To Enum"), this, [=](){this->sendToEnum();});

    /* showing the context menu */
    menu.exec(pos);
}

void Raw::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->treeViewResults->selectionModel()->isSelected(ui->treeViewResults->currentIndex()))
        return;

    /* getting the selected items... */
     selectionModel = ui->treeViewResults->selectionModel();

    /* creating the context menu... */
    QMenu menu(this);

    /* adding actions */
    menu.addAction(tr("Remove"), this, [=](){this->removeResults();});
    menu.addAction(tr("Open In Browser"), this, [=](){this->openInBrowser();});
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveSelectedResults();});
    menu.addAction(tr("Copy"), this, [=](){this->copySelectedResults();});
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendSelectedToProject();});
    menu.addAction(tr("Send To Engine"), this, [=](){this->sendSelectedToEngine();});
    menu.addAction(tr("Send T Enum"), this, [=](){this->sendSelectedToEnum();});

    /* showing the context menu */
    menu.exec(QCursor::pos());
}

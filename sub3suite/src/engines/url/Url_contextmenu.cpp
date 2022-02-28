/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Url.h"
#include "ui_Url.h"


void Url::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(proxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+63, pos.y());

    /* extract menu */
    QMenu extractMenu(this);
    extractMenu.setTitle(tr("Extract"));
    extractMenu.setIcon(QIcon(":/img/res/icons/extract.png"));
    extractMenu.addAction(tr("Domain"), this, [=](){this->extract();});

    QMenu menu(this);
    menu.addAction(tr("Clear"), this, [=](){this->clearResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addMenu(&extractMenu);
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveResults();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->copyResults();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
    menu.addSeparator();
    menu.addAction(tr("Send URL to URL"), this, [=](){this->sendToEngine(ENGINE::URL);})->setIcon(QIcon(":/img/res/icons/url.png"));
    menu.addAction(tr("Send URL to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW);})->setIcon(QIcon(":/img/res/icons/url.png"));

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

    /* extract menu */
    QMenu extractMenu(this);
    extractMenu.setTitle(tr("Extract"));
    extractMenu.setIcon(QIcon(":/img/res/icons/extract.png"));
    extractMenu.addAction(tr("Domain"), this, [=](){this->extractSelected();});

    QMenu menu(this);
    menu.addAction(tr("Remove"), this, [=](){this->saveSelectedResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addAction(tr("Open in Browser"), this, [=](){this->openInBrowser();})->setIcon(QIcon(":/img/res/icons/browser.png"));
    menu.addMenu(&extractMenu);
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveSelectedResults();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->copySelectedResults();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendSelectedToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
    menu.addSeparator();
    menu.addAction(tr("Send URL to URL"), this, [=](){this->sendSelectedToEngine(ENGINE::URL);})->setIcon(QIcon(":/img/res/icons/url.png"));
    menu.addAction(tr("Send URL to RAW"), this, [=](){this->sendSelectedToEngine(ENGINE::RAW);})->setIcon(QIcon(":/img/res/icons/url.png"));

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}

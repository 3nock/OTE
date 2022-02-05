#include "CidrEnum.h"
#include "ui_CidrEnum.h"

#include <QMenu>


void CidrEnum::initActions(){
    connect(&a_RemoveResults, &QAction::triggered, this, [=](){this->m_removeResults(selectionModel);});
    connect(&a_ClearResults, &QAction::triggered, this, [=](){this->m_clearResults();});
    connect(&a_ExpandResults, &QAction::triggered, this, [=](){ui->treeResults->expandAll();});
    connect(&a_CollapseResults, &QAction::triggered, this, [=](){ui->treeResults->collapseAll();});
    connect(&a_Save, &QAction::triggered, this, [=](){this->m_saveResults();});
    connect(&a_Copy, &QAction::triggered, this, [=](){this->m_copyResults();});
    /* for all */
    connect(&a_SendAllToProject, &QAction::triggered, this, [=](){this->m_sendToProject();});
    connect(&a_SendAllASNToOsint, &QAction::triggered, this, [=](){this->m_sendASNToEngine(ENGINE::OSINT);});
    connect(&a_SendAllASNToRaw, &QAction::triggered, this, [=](){this->m_sendASNToEngine(ENGINE::RAW);});
    connect(&a_SendAllASNToASNEnum, &QAction::triggered, this, [=](){this->m_sendASNToEnum();});
    /* for selected */
    connect(&a_SendSelectedToProject, &QAction::triggered, this, [=](){this->m_sendToProject(selectionModel);});
    connect(&a_SendSelectedASNToOsint, &QAction::triggered, this, [=](){this->m_sendASNToEngine(ENGINE::OSINT, selectionModel);});
    connect(&a_SendSelectedASNToRaw, &QAction::triggered, this, [=](){this->m_sendASNToEngine(ENGINE::RAW, selectionModel);});
    connect(&a_SendSelectedASNToASNEnum, &QAction::triggered, this, [=](){this->m_sendASNToEnum(selectionModel);});
}

void CidrEnum::on_buttonAction_clicked(){
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
    menu.addSeparator();
    menu.addAction(&a_SendAllASNToOsint);
    menu.addAction(&a_SendAllASNToRaw);
    menu.addSeparator();
    menu.addAction(&a_SendAllASNToASNEnum);

    /* showing the context menu... */
    menu.exec(pos);
}

void CidrEnum::on_treeResults_customContextMenuRequested(const QPoint &pos){
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
    menu.addSeparator();
    menu.addAction(&a_SendSelectedASNToOsint);
    menu.addAction(&a_SendSelectedASNToRaw);
    menu.addSeparator();
    menu.addAction(&a_SendSelectedASNToASNEnum);

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}

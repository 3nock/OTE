#include "CidrEnum.h"
#include "ui_CidrEnum.h"

#include <QMenu>


void CidrEnum::on_buttonAction_clicked(){
    /* check if there are results available else dont show the context menu */
    if(proxyModel->rowCount() < 1)
        return;

    /* getting the position of the action button to place the context menu and
       showing the context menu right by the side of the action button... */
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+60, pos.y());

    /* creating the context menu... */
    QMenu menu(this);
    menu.addAction(tr("Clear"), this, [=](){this->clearResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addAction(tr("Expand"), this, [=](){ui->treeViewResults->expandAll();})->setIcon(QIcon(":/img/res/icons/expand.png"));
    menu.addAction(tr("Collapse"), this, [=](){ui->treeViewResults->collapseAll();})->setIcon(QIcon(":/img/res/icons/collapse.png"));
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveResults();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->copyResults();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    menu.addSeparator();
    menu.addAction(tr("Send To Project"), this, [=](){this->sendToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
    menu.addSeparator();
    menu.addAction(tr("Send ASN to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
    menu.addAction(tr("Send ASN to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
    menu.addAction(tr("Send Email to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
    menu.addAction(tr("Send Email to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
    menu.addSeparator();
    menu.addAction(tr("Send ASN to ASN-Enum"), this, [=](){this->sendToEnum(TOOL::ASN, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
    menu.addAction(tr("Send Email to Email-Enum"), this, [=](){this->sendToEnum(TOOL::EMAIL, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));

    /* showing the context menu... */
    menu.exec(pos);
}

void CidrEnum::on_treeViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->treeViewResults->selectionModel()->isSelected(ui->treeViewResults->currentIndex()))
        return;

    /* getting the selected items... */
    selectionModel = ui->treeViewResults->selectionModel();

    /* creating the context menu... */
    QMenu menu(this);
    menu.addAction(tr("Remove"), this, [=](){this->clearResults();})->setIcon(QIcon(":/img/res/icons/delete.png"));
    menu.addSeparator();
    menu.addAction(tr("Save"), this, [=](){this->saveSelectedResults();})->setIcon(QIcon(":/img/res/icons/save.png"));
    menu.addAction(tr("Copy"), this, [=](){this->copySelectedResults();})->setIcon(QIcon(":/img/res/icons/copy.png"));
    menu.addSeparator();
    if(selectionModel->columnIntersectsSelection(0, selectionModel->currentIndex().parent()))
        menu.addAction(tr("Send To Project"), this, [=](){this->sendSelectedToProject();})->setIcon(QIcon(":/img/res/icons/project.png"));
    if(selectionModel->columnIntersectsSelection(1, selectionModel->currentIndex().parent())){
        menu.addAction(tr("Send ASN to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
        menu.addAction(tr("Send ASN to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
        menu.addAction(tr("Send Email to OSINT"), this, [=](){this->sendToEngine(ENGINE::OSINT, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
        menu.addAction(tr("Send Email to RAW"), this, [=](){this->sendToEngine(ENGINE::RAW, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
        menu.addSeparator();
        menu.addAction(tr("Send ASN to ASN-Enum"), this, [=](){this->sendToEnum(TOOL::ASN, RESULT_TYPE::ASN);})->setIcon(QIcon(":/img/res/icons/asn.png"));
        menu.addAction(tr("Send Email to Email-Enum"), this, [=](){this->sendToEnum(TOOL::EMAIL, RESULT_TYPE::EMAIL);})->setIcon(QIcon(":/img/res/icons/email.png"));
    }

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}

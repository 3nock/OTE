#include "Base.h"


BaseClass::BaseClass(ENGINE engineName, ResultsModel *resultsModel, QWidget *parent) :
    ContextMenu(engineName, resultsModel, parent),
    scanStatus(new ScanStatus),
    scanConfig(new ScanConfig)
{
}
BaseClass::~BaseClass(){
    delete scanStatus;
    delete scanConfig;
}

void BaseClass::logs(QString log){
    Q_UNUSED(log);
}

void BaseClass::onReceiveTargets(ENGINE engineName, CHOICE choice){
    ///
    /// varible declaration...
    ///
    QStandardItemModel *model = nullptr;
    QString item;
    ///
    /// engine targets are from...
    ///
    switch(engineName){
    case ENGINE::BRUTE:
        model = resultsModel->brute;
        break;
    case ENGINE::ACTIVE:
        model = resultsModel->active;
        break;
    case ENGINE::LEVEL:
        model = resultsModel->level;
        break;
    case ENGINE::IP:
        model = resultsModel->ip;
        break;
    case ENGINE::RECORDS:
        model = resultsModel->dnsrecords;
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(choice == CHOICE::susbdomains && (model->item(i)->child(j)->text() == "CNAME" || model->item(i)->child(j)->text() == "MX" || model->item(i)->child(j)->text() == "NS"))
                {
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                        m_targets->add(model->item(i)->child(j)->child(k)->text());
                }
                if(choice == CHOICE::ipaddress && (model->item(i)->child(j)->text() == "A" || model->item(i)->child(j)->text() == "AAAA"))
                {
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                        m_targets->add(model->item(i)->child(j)->child(k)->text());
                }
            }
        }
        ///
        /// for srv records dont have ip-address...
        ///
        if(choice == CHOICE::ipaddress)
            return;
        model = resultsModel->dnsrecords;
        for(int i = 0; i < model->rowCount(); i++)
            m_targets->add(model->item(i, 1)->text());
        return;
    default:
        break;
    }
    ///
    /// for any other engine...
    ///
    for(int i = 0; i < model->rowCount(); i++)
    {
        if(choice == CHOICE::susbdomains)
            item = model->item(i, 0)->text();
        if(choice == CHOICE::ipaddress)
            item = model->item(i, 1)->text();
        m_targets->add(item);
    }
}

void BaseClass::onReceiveTargets(QItemSelectionModel *selectionModel){
    QString item;
    ///
    /// iterate and open each selected and append on the target's listwidget...
    ///
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        m_targets->add(index.data().toString());
}

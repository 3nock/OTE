#include "AbstractEngine.h"
#include <QDesktopServices>

/*
 * use DNSRECORDS & SRVRECORDS for engineNames
 */
void AbstractEngine::onReceiveTargets(ENGINE engineName, CHOICE choice){
    QSortFilterProxyModel *model = nullptr;
    QString item;
    ///
    /// engine targets are from...
    ///
    switch(engineName){
    case ENGINE::OSINT:
        model = resultsModel->proxy->osint;
        break;
    case ENGINE::BRUTE:
        model = resultsModel->proxy->brute;
        break;
    case ENGINE::ACTIVE:
        model = resultsModel->proxy->active;
        break;
    case ENGINE::IP:
        model = resultsModel->proxy->ip;
        break;
    case ENGINE::RECORDS:
        /*
        model = resultsModel->proxy->dnsrecords;
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(choice == CHOICE::susbdomains && (model->item(i)->child(j)->text() == "CNAME" || model->item(i)->child(j)->text() == "MX" || model->item(i)->child(j)->text() == "NS"))
                {
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                        targets->add(model->item(i)->child(j)->child(k)->text());
                }
                if(choice == CHOICE::ipaddress && (model->item(i)->child(j)->text() == "A" || model->item(i)->child(j)->text() == "AAAA"))
                {
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                        targets->add(model->item(i)->child(j)->child(k)->text());
                }
            }
        }
        */
        ///
        /// for srv records dont have ip-address...
        ///
        if(choice == CHOICE::ipaddress)
            return;
        model = resultsModel->proxy->srvrecords;
        for(int i = 0; i < model->rowCount(); i++)
            targets->add(model->data(model->index(i, 1)).toString());
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
            item = model->data(model->index(i, 0)).toString();
        if(choice == CHOICE::ipaddress){
            item = model->data(model->index(i, 1)).toString();
        }
        targets->add(item);
    }
}

void AbstractEngine::onReceiveTargets(QItemSelectionModel *selectionModel){
    QString item;
    ///
    /// iterate and open each selected and append on the target's listwidget...
    ///
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
        targets->add(index.data().toString());
}

void AbstractEngine::openInBrowser(QItemSelectionModel *selectionModel){
    QString item;
    foreach(const QModelIndex &index, selectionModel->selectedIndexes())
    {
        item = index.data().toString();
        if(item != "A" && item != "AAAA" && item != "NS" && item != "MX" && item != "CNAME" && item != "TXT")
            QDesktopServices::openUrl(QUrl("https://"+item, QUrl::TolerantMode));
    }
}

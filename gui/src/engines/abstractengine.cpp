#include "AbstractEngine.h"
#include <QDesktopServices>

/*
 * use DNSRECORDS & SRVRECORDS for engineNames
 */
void AbstractEngine::onReceiveTargets(ENGINE engineName, CHOICE choice){
    QStandardItemModel *model = nullptr;
    QString item;
    ///
    /// engine targets are from...
    ///
    switch(engineName){
    case ENGINE::OSINT:
        model = resultsModel->osint;
        break;
    case ENGINE::BRUTE:
        model = resultsModel->brute;
        break;
    case ENGINE::ACTIVE:
        model = resultsModel->active;
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
                        targets->add(model->item(i)->child(j)->child(k)->text());
                }
                if(choice == CHOICE::ipaddress && (model->item(i)->child(j)->text() == "A" || model->item(i)->child(j)->text() == "AAAA"))
                {
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                        targets->add(model->item(i)->child(j)->child(k)->text());
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
            targets->add(model->item(i, 1)->text());
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

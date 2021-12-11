#include "AbstractEngine.h"
#include <QDesktopServices>

/*
 * use DNSRECORDS & SRVRECORDS for engineNames
 */
void AbstractEngine::onReceiveTargets(ENGINE engineName, CHOICE choice, PROXYMODEL_TYPE proxyModelType){
    QSortFilterProxyModel *model = nullptr;
    QString item;


    switch (proxyModelType) {

    case subdomainIpProxy:
        switch(engineName){
            case ENGINE::OSINT:
                model = result->osint->subdomainIpProxy;
                break;
            case ENGINE::BRUTE:
                model = result->brute->subdomainIpProxy;
                break;
            case ENGINE::ACTIVE:
                model = result->active->subdomainIpProxy;
                break;
            case ENGINE::IP:
                model = result->ip->subdomainIpProxy;
                break;
            default:
                return;
        }
        for(int i = 0; i < model->rowCount(); i++)
        {
            if(choice == CHOICE::subdomain)
                item = model->data(model->index(i, 0)).toString();
            if(choice == CHOICE::ip){
                item = model->data(model->index(i, 1)).toString();
            }
            targets->add(item);
        }
        return;

    case subdomainProxy:
        switch(engineName){
            case ENGINE::OSINT:
                model = result->osint->subdomainProxy;
                break;
            case ENGINE::CERT:
                model = result->cert->subdomainProxy;
                break;
            case ENGINE::BRUTE:
                model = result->brute->subdomainIpProxy;
                break;
            case ENGINE::ACTIVE:
                model = result->active->subdomainIpProxy;
                break;
            case ENGINE::IP:
                model = result->ip->subdomainIpProxy;
                break;
            default:
                return;
        }
        for(int i = 0; i < model->rowCount(); i++)
        {
            item = model->data(model->index(i, 0)).toString();
            targets->add(item);
        }
        return;

    case ipProxy:
        switch(engineName){
            case ENGINE::OSINT:
                model = result->osint->ipProxy;
                ///
                /// osint ips are located at first column...
                ///
                for(int i = 0; i < model->rowCount(); i++)
                {
                    item = model->data(model->index(i, 0)).toString();
                    targets->add(item);
                }
                return;
            case ENGINE::BRUTE:
                model = result->brute->subdomainIpProxy;
                break;
            case ENGINE::ACTIVE:
                model = result->active->subdomainIpProxy;
                break;
            case ENGINE::IP:
                model = result->ip->subdomainIpProxy;
                break;
            default:
                break;
        }
        ///
        /// all other models ip is located at second column...
        ///
        for(int i = 0; i < model->rowCount(); i++)
        {
            item = model->data(model->index(i, 1)).toString();
            targets->add(item);
        }
        break;

    case srvProxy:
        model = result->records->srvProxy;
        for(int i = 0; i < model->rowCount(); i++)
        {
            item = model->data(model->index(i, 1)).toString();
            targets->add(item);
        }
        return;

    case sslCertProxy:
        model = result->cert->sslCertProxy;
        for(int i = 0; i < model->rowCount(); i++)
        {
            item = model->data(model->index(i, 0)).toString();
            targets->add(item);
        }
        return;

    case dnsProxy:
        /*
        model = resultsModel->proxy->dnsrecords;
        for(int i = 0; i < model->rowCount(); i++)
        {
            for(int j = 0; j < model->item(i)->rowCount(); j++)
            {
                if(choice == CHOICE::subdomain && (model->item(i)->child(j)->text() == "CNAME" || model->item(i)->child(j)->text() == "MX" || model->item(i)->child(j)->text() == "NS"))
                {
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                        targets->add(model->item(i)->child(j)->child(k)->text());
                }
                if(choice == CHOICE::ip && (model->item(i)->child(j)->text() == "A" || model->item(i)->child(j)->text() == "AAAA"))
                {
                    for(int k = 0; k < model->item(i)->child(j)->rowCount(); k++)
                        targets->add(model->item(i)->child(j)->child(k)->text());
                }
            }
        }
        */
        return;

    default:
        break;
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

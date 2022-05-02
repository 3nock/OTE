#include "Project.h"
#include "ui_Project.h"

#include <QDesktopServices>
#include <QFileDialog>
#include <QClipboard>

/*
 * TODO:
 *      refactor remove action, for items
 *      when you remove one an entire s3s_item is gone hence modelindexes isn't down by one
 */

void Project::action_clear(){
    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeHost:
        model->activeHost->clear();
        model->map_activeHost.clear();
        break;
    case ExplorerType::activeWildcard:
        model->activeWildcard->clear();
        model->map_activeWildcard.clear();
        break;
    case ExplorerType::activeDNS:
        model->activeDNS->clear();
        model->map_activeDNS.clear();
        break;
    case ExplorerType::activeDNS_A:
        model->activeA->clear();
        break;
    case ExplorerType::activeDNS_AAAA:
        model->activeAAAA->clear();
        break;
    case ExplorerType::activeDNS_NS:
        model->activeNS->clear();
        break;
    case ExplorerType::activeDNS_MX:
        model->activeMX->clear();
        break;
    case ExplorerType::activeDNS_TXT:
        model->activeTXT->clear();
        break;
    case ExplorerType::activeDNS_CNAME:
        model->activeCNAME->clear();
        break;
    case ExplorerType::activeSSL:
        model->activeSSL->clear();
        model->map_activeSSL.clear();
        break;
    case ExplorerType::activeSSL_sha1:
        model->activeSSL_sha1->clear();
        break;
    case ExplorerType::activeSSL_sha256:
        model->activeSSL_sha256->clear();
        break;
    case ExplorerType::activeSSL_altNames:
        model->activeSSL_altNames->clear();
        break;
    case ExplorerType::activeURL:
        model->activeURL->clear();
        model->map_activeURL.clear();
        break;
    case ExplorerType::passive_subdomainIp:
        model->passiveSubdomainIp->clear();
        break;
    case ExplorerType::passive_subdomain:
        model->passiveSubdomain->clear();
        break;
    case ExplorerType::passive_A:
        model->passiveA->clear();
        break;
    case ExplorerType::passive_AAAA:
        model->passiveAAAA->clear();
        break;
    case ExplorerType::passive_NS:
        model->passiveNS->clear();
        break;
    case ExplorerType::passive_MX:
        model->passiveMX->clear();
        break;
    case ExplorerType::passive_TXT:
        model->passiveTXT->clear();
        break;
    case ExplorerType::passive_CNAME:
        model->passiveCNAME->clear();
        break;
    case ExplorerType::passive_Email:
        model->passiveEmail->clear();
        break;
    case ExplorerType::passive_URL:
        model->passiveURL->clear();
        break;
    case ExplorerType::passive_ASN:
        model->passiveASN->clear();
        break;
    case ExplorerType::passive_SSL:
        model->passiveSSL->clear();
        break;
    case ExplorerType::enum_IP:
        model->enumIP->clear();
        model->map_enumIP.clear();
        break;
    case ExplorerType::enum_ASN:
        model->enumASN->clear();
        model->map_enumASN.clear();
        break;
    case ExplorerType::enum_CIDR:
        model->enumCIDR->clear();
        model->map_enumCIDR.clear();
        break;
    case ExplorerType::enum_NS:
        model->enumNS->clear();
        model->map_enumNS.clear();
        break;
    case ExplorerType::enum_MX:
        model->enumMX->clear();
        model->map_enumMX.clear();
        break;
    case ExplorerType::enum_Email:
        model->enumEmail->clear();
        model->map_enumEmail.clear();
        break;
    case ExplorerType::enum_SSL:
        model->enumSSL->clear();
        model->map_enumSSL.clear();
        break;
    case ExplorerType::raw:
        model->raw->clear();
        break;
    }

    model->setHeaderLabels();
    ui->labelCount->setNum(proxyModel->rowCount());
    model->modified = true;
}

void Project::action_remove_duplicates(){
    QStandardItemModel *choosen_model;
    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeDNS_A:
        choosen_model = model->activeA;
        break;
    case ExplorerType::activeDNS_AAAA:
        choosen_model = model->activeAAAA;
        break;
    case ExplorerType::activeDNS_NS:
        choosen_model = model->activeNS;
        break;
    case ExplorerType::activeDNS_MX:
        choosen_model = model->activeMX;
        break;
    case ExplorerType::activeDNS_TXT:
        choosen_model = model->activeTXT;
        break;
    case ExplorerType::activeDNS_CNAME:
        choosen_model = model->activeCNAME;
        break;
    case ExplorerType::activeSSL_sha1:
        choosen_model = model->activeSSL_sha1;
        break;
    case ExplorerType::activeSSL_sha256:
        choosen_model = model->activeSSL_sha256;
        break;
    case ExplorerType::activeSSL_altNames:
        choosen_model = model->activeSSL_altNames;
        break;
    case ExplorerType::passive_subdomainIp:
        choosen_model = model->passiveSubdomainIp;
        break;
    case ExplorerType::passive_subdomain:
        choosen_model = model->passiveSubdomain;
        break;
    case ExplorerType::passive_A:
        choosen_model = model->passiveA;
        break;
    case ExplorerType::passive_AAAA:
        choosen_model = model->passiveAAAA;
        break;
    case ExplorerType::passive_NS:
        choosen_model = model->passiveNS;
        break;
    case ExplorerType::passive_MX:
        choosen_model = model->passiveMX;
        break;
    case ExplorerType::passive_TXT:
        choosen_model = model->passiveTXT;
        break;
    case ExplorerType::passive_CNAME:
        choosen_model = model->passiveCNAME;
        break;
    case ExplorerType::passive_Email:
        choosen_model = model->passiveEmail;
        break;
    case ExplorerType::passive_URL:
        choosen_model = model->passiveURL;
        break;
    case ExplorerType::passive_ASN:
        choosen_model = model->passiveASN;
        break;
    case ExplorerType::passive_SSL:
        choosen_model = model->passiveSSL;
        break;
    default:
        return;
    }

    QSet<QString> set;
    for(int i = 0; i < choosen_model->rowCount(); i++){
        if(set.contains(choosen_model->item(i,0)->text())){
            choosen_model->removeRow(i);
            i--;
        }
        else
            set.insert(choosen_model->item(i,0)->text());
    }
    ui->labelCount->setNum(proxyModel->rowCount());
    model->modified = true;
}

void Project::action_save(const RESULT_TYPE &result_type){
    if(proxyModel->rowCount() == 0){
        qWarning() << "PROJECT:  Nothing to save";
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qWarning() << "PROJECT: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qWarning() << "PROJECT: Failed to open " << filename << " For saving Results";
        return;
    }

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeDNS_A:
    case ExplorerType::activeDNS_AAAA:
    case ExplorerType::activeDNS_NS:
    case ExplorerType::activeDNS_MX:
    case ExplorerType::activeDNS_TXT:
    case ExplorerType::activeDNS_CNAME:
    case ExplorerType::activeSSL_sha1:
    case ExplorerType::activeSSL_sha256:
    case ExplorerType::activeSSL_altNames:
    case ExplorerType::passive_subdomain:
    case ExplorerType::passive_A:
    case ExplorerType::passive_AAAA:
    case ExplorerType::passive_NS:
    case ExplorerType::passive_MX:
    case ExplorerType::passive_TXT:
    case ExplorerType::passive_CNAME:
    case ExplorerType::passive_Email:
    case ExplorerType::passive_URL:
    case ExplorerType::passive_ASN:
    case ExplorerType::passive_SSL:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            file.write(proxyModel->index(i, 0).data().toString().append(NEWLINE).toUtf8());
        break;

    case ExplorerType::passive_subdomainIp:
        switch (result_type) {
        case RESULT_TYPE::CSV:
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QString host(proxyModel->index(i, 0).data().toString());
                QString ip(proxyModel->index(i, 1).data().toString());

                if(!ip.isEmpty())
                    host.append(",").append(ip);

                file.write(host.append(NEWLINE).toUtf8());
            }
            break;
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                file.write(proxyModel->index(i, 0).data().toString().append(NEWLINE).toUtf8());
            break;
        case RESULT_TYPE::IP:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                file.write(proxyModel->index(i, 1).data().toString().append(NEWLINE).toUtf8());
            break;
        default:
            break;
        }
        break;
    case ExplorerType::activeDNS_SRV:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QString name(proxyModel->index(i, 0).data().toString());
            QString target(proxyModel->index(i, 1).data().toString());
            QString port(proxyModel->index(i, 2).data().toString());

            if(!target.isEmpty())
                name.append(",").append(target);
            if(!port.isEmpty())
                name.append(",").append(port);

            file.write(name.append(NEWLINE).toUtf8());
        }
        break;
    case ExplorerType::activeHost:
        switch (result_type) {
        case RESULT_TYPE::JSON:
        {
            QJsonArray array;
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
                s3s_item::HOST *item = static_cast<s3s_item::HOST*>(model->activeHost->itemFromIndex(model_index));
                array.append(host_to_json(item));
            }
            QJsonDocument document;
            document.setArray(array);
            file.write(document.toJson());
        }
            break;
        case RESULT_TYPE::CSV:
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QString host(proxyModel->index(i, 0).data().toString());
                QString ipv4(proxyModel->index(i, 1).data().toString());
                QString ipv6(proxyModel->index(i, 2).data().toString());
                QString port(proxyModel->index(i, 3).data().toString());

                if(!ipv4.isEmpty())
                    host.append(",").append(ipv4);
                if(!ipv6.isEmpty())
                    host.append(",").append(ipv6);
                if(!port.isEmpty())
                    host.append(",").append(port);

                file.write(host.append(NEWLINE).toUtf8());
            }
            break;
        case RESULT_TYPE::IP:
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QString ipv4(proxyModel->index(i, 1).data().toString());
                QString ipv6(proxyModel->index(i, 2).data().toString());

                if(!ipv4.isEmpty())
                    file.write(ipv4.append(NEWLINE).toUtf8());
                if(!ipv6.isEmpty())
                    file.write(ipv6.append(NEWLINE).toUtf8());
            }
            break;
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                file.write(proxyModel->index(i, 0).data().toString().append(NEWLINE).toUtf8());
            break;
        case RESULT_TYPE::IPV4:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                file.write(proxyModel->index(i, 1).data().toString().append(NEWLINE).toUtf8());
            break;
        case RESULT_TYPE::IPV6:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                file.write(proxyModel->index(i, 2).data().toString().append(NEWLINE).toUtf8());
            break;
        default:
            break;
        }
        break;
    case ExplorerType::activeWildcard:
        switch (result_type) {
        case RESULT_TYPE::JSON:
        {
            QJsonArray array;
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
                s3s_item::Wildcard *item = static_cast<s3s_item::Wildcard*>(model->activeWildcard->itemFromIndex(model_index));
                array.append(wildcard_to_json(item));
            }
            QJsonDocument document;
            document.setArray(array);
            file.write(document.toJson());
        }
            break;
        case RESULT_TYPE::CSV:
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QString host(proxyModel->index(i, 0).data().toString());
                QString ipv4(proxyModel->index(i, 1).data().toString());
                QString ipv6(proxyModel->index(i, 2).data().toString());

                if(!ipv4.isEmpty())
                    host.append(",").append(ipv4);
                if(!ipv6.isEmpty())
                    host.append(",").append(ipv6);

                file.write(host.append(NEWLINE).toUtf8());
            }
            break;
        case RESULT_TYPE::IP:
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QString ipv4(proxyModel->index(i, 1).data().toString());
                QString ipv6(proxyModel->index(i, 2).data().toString());

                if(!ipv4.isEmpty())
                    file.write(ipv4.append(NEWLINE).toUtf8());
                if(!ipv6.isEmpty())
                    file.write(ipv6.append(NEWLINE).toUtf8());
            }
            break;
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                file.write(proxyModel->index(i, 0).data().toString().append(NEWLINE).toUtf8());
            break;
        case RESULT_TYPE::IPV4:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                file.write(proxyModel->index(i, 1).data().toString().append(NEWLINE).toUtf8());
            break;
        case RESULT_TYPE::IPV6:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                file.write(proxyModel->index(i, 2).data().toString().append(NEWLINE).toUtf8());
            break;
        default:
            break;
        }
        break;
    case ExplorerType::activeDNS:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));
            array.append(dns_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    case ExplorerType::activeSSL:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(model->activeSSL->itemFromIndex(model_index));
            array.append(ssl_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    case ExplorerType::activeURL:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::URL *item = static_cast<s3s_item::URL*>(model->activeURL->itemFromIndex(model_index));
            array.append(url_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    case ExplorerType::enum_IP:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::IP *item = static_cast<s3s_item::IP*>(model->enumIP->itemFromIndex(model_index));
            array.append(ip_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    case ExplorerType::enum_ASN:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));
            array.append(asn_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    case ExplorerType::enum_CIDR:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::CIDR *item = static_cast<s3s_item::CIDR*>(model->enumCIDR->itemFromIndex(model_index));
            array.append(cidr_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    case ExplorerType::enum_NS:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(model->enumNS->itemFromIndex(model_index));
            array.append(ns_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    case ExplorerType::enum_MX:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::MX *item = static_cast<s3s_item::MX*>(model->enumMX->itemFromIndex(model_index));
            array.append(mx_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    case ExplorerType::enum_Email:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::Email *item = static_cast<s3s_item::Email*>(model->enumEmail->itemFromIndex(model_index));
            array.append(email_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    case ExplorerType::enum_SSL:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(model->enumSSL->itemFromIndex(model_index));
            array.append(ssl_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    case ExplorerType::raw:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::RAW *item = static_cast<s3s_item::RAW*>(model->raw->itemFromIndex(model_index));
            array.append(raw_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        file.write(document.toJson());
    }
        break;
    }

    file.close();
}

void Project::action_copy(const RESULT_TYPE &result_type){
    if(proxyModel->rowCount() == 0){
        qWarning() << "PROJECT:  Nothing to copy";
        return;
    }

    QString clipboardData;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeDNS_A:
    case ExplorerType::activeDNS_AAAA:
    case ExplorerType::activeDNS_NS:
    case ExplorerType::activeDNS_MX:
    case ExplorerType::activeDNS_TXT:
    case ExplorerType::activeDNS_CNAME:
    case ExplorerType::activeSSL_sha1:
    case ExplorerType::activeSSL_sha256:
    case ExplorerType::activeSSL_altNames:
    case ExplorerType::passive_subdomain:
    case ExplorerType::passive_A:
    case ExplorerType::passive_AAAA:
    case ExplorerType::passive_NS:
    case ExplorerType::passive_MX:
    case ExplorerType::passive_TXT:
    case ExplorerType::passive_CNAME:
    case ExplorerType::passive_Email:
    case ExplorerType::passive_URL:
    case ExplorerType::passive_ASN:
    case ExplorerType::passive_SSL:
        for(int i = 0; i != proxyModel->rowCount(); ++i)
            clipboardData.append(proxyModel->index(i, 0).data().toString().append(NEWLINE));
        break;

    case ExplorerType::passive_subdomainIp:
        switch (result_type) {
        case RESULT_TYPE::CSV:
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QString host(proxyModel->index(i, 0).data().toString());
                QString ip(proxyModel->index(i, 1).data().toString());

                if(!ip.isEmpty())
                    host.append(",").append(ip);

                clipboardData.append(host.append(NEWLINE));
            }
            break;
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                clipboardData.append(proxyModel->index(i, 0).data().toString().append(NEWLINE));
            break;
        case RESULT_TYPE::IP:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                clipboardData.append(proxyModel->index(i, 1).data().toString().append(NEWLINE));
            break;
        default:
            break;
        }
        break;
    case ExplorerType::activeDNS_SRV:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QString name(proxyModel->index(i, 0).data().toString());
            QString target(proxyModel->index(i, 1).data().toString());
            QString port(proxyModel->index(i, 2).data().toString());

            if(!target.isEmpty())
                name.append(",").append(target);
            if(!port.isEmpty())
                name.append(",").append(port);

            clipboardData.append(name.append(NEWLINE));
        }
        break;
    case ExplorerType::activeHost:
        switch (result_type) {
        case RESULT_TYPE::JSON:
        {
            QJsonArray array;
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
                s3s_item::HOST *item = static_cast<s3s_item::HOST*>(model->activeHost->itemFromIndex(model_index));
                array.append(host_to_json(item));
            }
            QJsonDocument document;
            document.setArray(array);
            clipboardData.append(document.toJson());
        }
            break;
        case RESULT_TYPE::CSV:
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QString host(proxyModel->index(i, 0).data().toString());
                QString ipv4(proxyModel->index(i, 1).data().toString());
                QString ipv6(proxyModel->index(i, 2).data().toString());
                QString port(proxyModel->index(i, 3).data().toString());

                if(!ipv4.isEmpty())
                    host.append(",").append(ipv4);
                if(!ipv6.isEmpty())
                    host.append(",").append(ipv6);
                if(!port.isEmpty())
                    host.append(",").append(port);

                clipboardData.append(host.append(NEWLINE));
            }
            break;
        case RESULT_TYPE::IP:
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QString ipv4(proxyModel->index(i, 1).data().toString());
                QString ipv6(proxyModel->index(i, 2).data().toString());

                if(!ipv4.isEmpty())
                    clipboardData.append(ipv4.append(NEWLINE));
                if(!ipv6.isEmpty())
                    clipboardData.append(ipv6.append(NEWLINE));
            }
            break;
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                clipboardData.append(proxyModel->index(i, 0).data().toString().append(NEWLINE));
            break;
        case RESULT_TYPE::IPV4:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                clipboardData.append(proxyModel->index(i, 1).data().toString().append(NEWLINE));
            break;
        case RESULT_TYPE::IPV6:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                clipboardData.append(proxyModel->index(i, 2).data().toString().append(NEWLINE));
            break;
        default:
            break;
        }
        break;
    case ExplorerType::activeWildcard:
        switch (result_type) {
        case RESULT_TYPE::JSON:
        {
            QJsonArray array;
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
                s3s_item::Wildcard *item = static_cast<s3s_item::Wildcard*>(model->activeWildcard->itemFromIndex(model_index));
                array.append(wildcard_to_json(item));
            }
            QJsonDocument document;
            document.setArray(array);
            clipboardData.append(document.toJson());
        }
            break;
        case RESULT_TYPE::CSV:
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QString host(proxyModel->index(i, 0).data().toString());
                QString ipv4(proxyModel->index(i, 1).data().toString());
                QString ipv6(proxyModel->index(i, 2).data().toString());

                if(!ipv4.isEmpty())
                    host.append(",").append(ipv4);
                if(!ipv6.isEmpty())
                    host.append(",").append(ipv6);

                clipboardData.append(host.append(NEWLINE));
            }
            break;
        case RESULT_TYPE::IP:
            for(int i = 0; i != proxyModel->rowCount(); ++i){
                QString ipv4(proxyModel->index(i, 1).data().toString());
                QString ipv6(proxyModel->index(i, 2).data().toString());

                if(!ipv4.isEmpty())
                    clipboardData.append(ipv4.append(NEWLINE));
                if(!ipv6.isEmpty())
                    clipboardData.append(ipv6.append(NEWLINE));
            }
            break;
        case RESULT_TYPE::SUBDOMAIN:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                clipboardData.append(proxyModel->index(i, 0).data().toString().append(NEWLINE));
            break;
        case RESULT_TYPE::IPV4:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                clipboardData.append(proxyModel->index(i, 1).data().toString().append(NEWLINE));
            break;
        case RESULT_TYPE::IPV6:
            for(int i = 0; i != proxyModel->rowCount(); ++i)
                clipboardData.append(proxyModel->index(i, 2).data().toString().append(NEWLINE));
            break;
        default:
            break;
        }
        break;
    case ExplorerType::activeDNS:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));
            array.append(dns_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        clipboardData.append(document.toJson());
    }
        break;
    case ExplorerType::activeSSL:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(model->activeSSL->itemFromIndex(model_index));
            array.append(ssl_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        clipboardData.append(document.toJson());
    }
        break;
    case ExplorerType::activeURL:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::URL *item = static_cast<s3s_item::URL*>(model->activeURL->itemFromIndex(model_index));
            array.append(url_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        clipboardData.append(document.toJson());
    }
        break;
    case ExplorerType::enum_IP:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::IP *item = static_cast<s3s_item::IP*>(model->enumIP->itemFromIndex(model_index));
            array.append(ip_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        clipboardData.append(document.toJson());
    }
        break;
    case ExplorerType::enum_ASN:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));
            array.append(asn_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        clipboardData.append(document.toJson());
    }
        break;
    case ExplorerType::enum_CIDR:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::CIDR *item = static_cast<s3s_item::CIDR*>(model->enumCIDR->itemFromIndex(model_index));
            array.append(cidr_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        clipboardData.append(document.toJson());
    }
        break;
    case ExplorerType::enum_NS:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(model->enumNS->itemFromIndex(model_index));
            array.append(ns_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        clipboardData.append(document.toJson());
    }
        break;
    case ExplorerType::enum_MX:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::MX *item = static_cast<s3s_item::MX*>(model->enumMX->itemFromIndex(model_index));
            array.append(mx_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        clipboardData.append(document.toJson());
    }
        break;
    case ExplorerType::enum_Email:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::Email *item = static_cast<s3s_item::Email*>(model->enumEmail->itemFromIndex(model_index));
            array.append(email_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        clipboardData.append(document.toJson());
    }
        break;
    case ExplorerType::enum_SSL:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(model->enumSSL->itemFromIndex(model_index));
            array.append(ssl_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        clipboardData.append(document.toJson());
    }
        break;
    case ExplorerType::raw:
    {
        QJsonArray array;
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::RAW *item = static_cast<s3s_item::RAW*>(model->raw->itemFromIndex(model_index));
            array.append(raw_to_json(item));
        }
        QJsonDocument document;
        document.setArray(array);
        clipboardData.append(document.toJson());
    }
        break;
    }

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(clipboardData.trimmed());
}

///
/// Send...
///

void Project::action_sendToEngine(const TOOL &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;
    foreach(const QModelIndex &index, m_selectionModel->selectedIndexes())
        targets.insert(index.data().toString());

    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(targets, result_type);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(targets, result_type);
        emit changeTabToRaw();
        break;
    case TOOL::BRUTE:
        emit sendToBrute(targets, result_type);
        emit changeTabToBrute();
        break;
    case TOOL::HOST:
        emit sendToHost(targets, result_type);
        emit changeTabToHost();
        break;
    case TOOL::IP:
        emit sendToIP(targets, result_type);
        emit changeTabToIP();
        break;
    case TOOL::DNS:
        emit sendToDns(targets, result_type);
        emit changeTabToDns();
        break;
    case TOOL::SSL:
        emit sendToSsl(targets, result_type);
        emit changeTabToSSL();
        break;
    case TOOL::URL:
        emit sendToUrl(targets, result_type);
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

void Project::action_sendToEnum(const ENUMERATOR &tool, const RESULT_TYPE &result_type){
    QSet<QString> targets;
    foreach(const QModelIndex &index, m_selectionModel->selectedIndexes())
        targets.insert(index.data().toString());

    switch (tool) {
    case ENUMERATOR::IP:
        emit sendToIpEnum(targets, result_type);
        emit changeTabToIpEnum();
        break;
    case ENUMERATOR::ASN:
        emit sendToAsnEnum(targets, result_type);
        emit changeTabToAsnEnum();
        break;
    case ENUMERATOR::CIDR:
        emit sendToCidrEnum(targets, result_type);
        emit changeTabToCidrEnum();
        break;
    case ENUMERATOR::NS:
        emit sendToNSEnum(targets, result_type);
        emit changeTabToNSEnum();
        break;
    case ENUMERATOR::MX:
        emit sendToMXEnum(targets, result_type);
        emit changeTabToMXEnum();
        break;
    case ENUMERATOR::EMAIL:
        emit sendToEmailEnum(targets, result_type);
        emit changeTabToEmailEnum();
        break;
    case ENUMERATOR::SSL:
        emit sendToSSLEnum(targets, result_type);
        emit changeTabToSSLEnum();
        break;
    }
}

void Project::action_send_host(const TOOL &engine){
    QSet<QString> hostnames;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeHost:
    case ExplorerType::activeDNS_NS:
    case ExplorerType::activeDNS_MX:
    case ExplorerType::activeDNS_CNAME:
    case ExplorerType::activeSSL_altNames:
    case ExplorerType::passive_subdomainIp:
    case ExplorerType::passive_subdomain:
    case ExplorerType::passive_NS:
    case ExplorerType::passive_MX:
    case ExplorerType::passive_CNAME:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            hostnames.insert(proxyModel->index(i,0).data().toString());
        break;
    case ExplorerType::activeDNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            /* itselft first */
            hostnames.insert(proxyModel->index(i,0).data().toString());

            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                hostnames.insert(item->CNAME->child(j, 1)->text());
            for(int j = 0; j < item->NS->rowCount(); j++)
                hostnames.insert(item->NS->child(j, 1)->text());
            for(int j = 0; j < item->MX->rowCount(); j++)
                hostnames.insert(item->MX->child(j, 1)->text());
            for(int j = 0; j < item->SRV->rowCount(); j++)
                hostnames.insert(item->SRV->child(j, 1)->text());
        }
        break;
    case ExplorerType::activeSSL:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(model->activeSSL->itemFromIndex(model_index));

            for(int j = 0; j < item->subjectAltNames->rowCount(); j++)
                hostnames.insert(item->subjectAltNames->child(j, 1)->text());
        }
        break;
    case ExplorerType::enum_MX:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::MX *item = static_cast<s3s_item::MX*>(model->enumMX->itemFromIndex(model_index));

            for(int j = 0; j < item->domains->rowCount(); j++)
                hostnames.insert(item->domains->child(j, 1)->text());
        }
        break;
    case ExplorerType::enum_NS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(model->enumNS->itemFromIndex(model_index));

            for(int j = 0; j < item->domains->rowCount(); j++)
                hostnames.insert(item->domains->child(j, 1)->text());
        }
        break;
    case ExplorerType::enum_SSL:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(model->enumSSL->itemFromIndex(model_index));

            for(int j = 0; j < item->subjectAltNames->rowCount(); j++)
                hostnames.insert(item->subjectAltNames->child(j, 1)->text());
        }
        break;
    }

    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(hostnames, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(hostnames, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToRaw();
        break;
    case TOOL::BRUTE:
        emit sendToBrute(hostnames, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToBrute();
        break;
    case TOOL::HOST:
        emit sendToHost(hostnames, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToHost();
        break;
    case TOOL::DNS:
        emit sendToDns(hostnames, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToDns();
        break;
    case TOOL::SSL:
        emit sendToSsl(hostnames, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Project::action_send_ip(const TOOL &engine){
    QSet<QString> ip;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeDNS_A:
    case ExplorerType::activeDNS_AAAA:
    case ExplorerType::passive_A:
    case ExplorerType::passive_AAAA:
    case ExplorerType::enum_IP:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            ip.insert(proxyModel->index(i, 0).data().toString());
        break;
    case ExplorerType::activeHost:
    case ExplorerType::activeWildcard:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            ip.insert(proxyModel->index(i, 1).data().toString());
            ip.insert(proxyModel->index(i, 2).data().toString());
        }
        break;
    case ExplorerType::passive_subdomainIp:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            ip.insert(proxyModel->index(i, 1).data().toString());
        break;
    case ExplorerType::activeDNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->A->rowCount(); j++)
                ip.insert(item->A->child(j, 1)->text());
            for(int j = 0; j < item->AAAA->rowCount(); j++)
                ip.insert(item->AAAA->child(j, 1)->text());
        }
        break;
    }

    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(ip, RESULT_TYPE::IP);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(ip, RESULT_TYPE::IP);
        emit changeTabToRaw();
        break;
    case TOOL::IP:
        emit sendToIP(ip, RESULT_TYPE::IP);
        emit changeTabToIP();
        break;
    default:
        break;
    }
}

void Project::action_send_url(const TOOL &engine){
    QSet<QString> url;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeURL:
    case ExplorerType::passive_URL:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            url.insert(proxyModel->index(i, 0).data().toString());
        break;
    }

    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(url, RESULT_TYPE::URL);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(url, RESULT_TYPE::URL);
        emit changeTabToRaw();
        break;
    case TOOL::URL:
        emit sendToUrl(url, RESULT_TYPE::URL);
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

void Project::action_send_email(const TOOL &engine){
    QSet<QString> emails;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::enum_Email:
    case ExplorerType::passive_Email:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            emails.insert(proxyModel->index(i, 0).data().toString());
        break;
    case ExplorerType::enum_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));

            for(int j = 0; j < item->abuseContacts->rowCount(); j++)
                emails.insert(item->abuseContacts->child(j, 1)->text());
            for(int j = 0; j < item->emailContacts->rowCount(); j++)
                emails.insert(item->emailContacts->child(j, 1)->text());
        }
        break;
    case ExplorerType::enum_CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::CIDR *item = static_cast<s3s_item::CIDR*>(model->enumCIDR->itemFromIndex(model_index));

            for(int j = 0; j < item->abuseContacts->rowCount(); j++)
                emails.insert(item->abuseContacts->child(j, 1)->text());
            for(int j = 0; j < item->emailContacts->rowCount(); j++)
                emails.insert(item->emailContacts->child(j, 1)->text());
        }
        break;
    }

    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(emails, RESULT_TYPE::EMAIL);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(emails, RESULT_TYPE::EMAIL);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Project::action_send_asn(const TOOL &engine){
    QSet<QString> asn;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            asn.insert(proxyModel->index(i, 0).data().toString());
        break;
    case ExplorerType::enum_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            /* itsefl first */
            asn.insert(proxyModel->index(i, 0).data().toString());

            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));

            for(int j = 0; j < item->peers->rowCount(); j++)
                asn.insert(item->peers->child(j, 1)->text());
        }
        break;
    case ExplorerType::enum_CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::CIDR *item = static_cast<s3s_item::CIDR*>(model->enumCIDR->itemFromIndex(model_index));

            for(int j = 0; j < item->asns->rowCount(); j++)
                asn.insert(item->asns->child(j, 1)->text());
        }
        break;
    }

    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(asn, RESULT_TYPE::ASN);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(asn, RESULT_TYPE::ASN);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Project::action_send_cidr(const TOOL &engine){
    QSet<QString> cidr;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_CIDR:
    case ExplorerType::enum_CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            cidr.insert(proxyModel->index(i, 0).data().toString());
        break;
    case ExplorerType::enum_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));

            for(int j = 0; j < item->prefixes->rowCount(); j++)
                cidr.insert(item->prefixes->child(j, 1)->text());
        }
        break;
    }

    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(cidr, RESULT_TYPE::CIDR);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(cidr, RESULT_TYPE::CIDR);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Project::action_send_ssl(const TOOL &engine){
    QSet<QString> ssl;
    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_SSL:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            ssl.insert(proxyModel->index(i, 0).data().toString());
        break;
    }

    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(ssl, RESULT_TYPE::CERT_ID);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(ssl, RESULT_TYPE::CERT_ID);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Project::action_send_ns(const TOOL &engine){
    QSet<QString> ns;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::enum_NS:
    case ExplorerType::passive_NS:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            ns.insert(proxyModel->index(i, 0).data().toString());
        break;
    case ExplorerType::activeDNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->NS->rowCount(); j++)
                ns.insert(item->NS->child(j, 1)->text());
        }
        break;
    }

    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(ns, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(ns, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToRaw();
        break;
    case TOOL::BRUTE:
        emit sendToBrute(ns, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToBrute();
        break;
    case TOOL::HOST:
        emit sendToHost(ns, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToHost();
        break;
    case TOOL::DNS:
        emit sendToDns(ns, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToDns();
        break;
    case TOOL::SSL:
        emit sendToSsl(ns, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Project::action_send_mx(const TOOL &engine){
    QSet<QString> mx;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::enum_MX:
    case ExplorerType::passive_MX:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            mx.insert(proxyModel->index(i, 0).data().toString());
        break;
    case ExplorerType::activeDNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->MX->rowCount(); j++)
                mx.insert(item->MX->child(j, 1)->text());
        }
        break;
    }

    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(mx, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(mx, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToRaw();
        break;
    case TOOL::BRUTE:
        emit sendToBrute(mx, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToBrute();
        break;
    case TOOL::HOST:
        emit sendToHost(mx, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToHost();
        break;
    case TOOL::DNS:
        emit sendToDns(mx, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToDns();
        break;
    case TOOL::SSL:
        emit sendToSsl(mx, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Project::action_send_ip(){
    QSet<QString> ip;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeDNS_A:
    case ExplorerType::activeDNS_AAAA:
    case ExplorerType::passive_A:
    case ExplorerType::passive_AAAA:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            ip.insert(proxyModel->index(i, 0).data().toString());
        break;
    case ExplorerType::activeHost:
    case ExplorerType::activeWildcard:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            ip.insert(proxyModel->index(i, 1).data().toString());
            ip.insert(proxyModel->index(i, 2).data().toString());
        }
        break;
    case ExplorerType::passive_subdomainIp:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            ip.insert(proxyModel->index(i, 1).data().toString());
        break;
    case ExplorerType::activeDNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->A->rowCount(); j++)
                ip.insert(item->A->child(j, 1)->text());
            for(int j = 0; j < item->AAAA->rowCount(); j++)
                ip.insert(item->AAAA->child(j, 1)->text());
        }
        break;
    }

    emit sendToIpEnum(ip, RESULT_TYPE::IP);
    emit changeTabToIpEnum();
}

void Project::action_send_email(){
    QSet<QString> emails;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::enum_Email:
    case ExplorerType::passive_Email:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            emails.insert(proxyModel->index(i, 0).data().toString());
        break;
    case ExplorerType::enum_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));

            for(int j = 0; j < item->abuseContacts->rowCount(); j++)
                emails.insert(item->abuseContacts->child(j, 1)->text());
            for(int j = 0; j < item->emailContacts->rowCount(); j++)
                emails.insert(item->emailContacts->child(j, 1)->text());
        }
        break;
    case ExplorerType::enum_CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::CIDR *item = static_cast<s3s_item::CIDR*>(model->enumCIDR->itemFromIndex(model_index));

            for(int j = 0; j < item->abuseContacts->rowCount(); j++)
                emails.insert(item->abuseContacts->child(j, 1)->text());
            for(int j = 0; j < item->emailContacts->rowCount(); j++)
                emails.insert(item->emailContacts->child(j, 1)->text());
        }
        break;
    }

    emit sendToEmailEnum(emails, RESULT_TYPE::EMAIL);
    emit changeTabToEmailEnum();
}

void Project::action_send_asn(){
    QSet<QString> asn;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            asn.insert(proxyModel->index(i, 0).data().toString());
        break;
    case ExplorerType::enum_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            /* itsefl first */
            asn.insert(proxyModel->index(i, 0).data().toString());

            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));

            for(int j = 0; j < item->peers->rowCount(); j++)
                asn.insert(item->peers->child(j, 1)->text());
        }
        break;
    case ExplorerType::enum_CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::CIDR *item = static_cast<s3s_item::CIDR*>(model->enumCIDR->itemFromIndex(model_index));

            for(int j = 0; j < item->asns->rowCount(); j++)
                asn.insert(item->asns->child(j, 1)->text());
        }
        break;
    }

    emit sendToAsnEnum(asn, RESULT_TYPE::ASN);
    emit changeTabToAsnEnum();
}

void Project::action_send_cidr(){
    QSet<QString> cidr;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_CIDR:
    case ExplorerType::enum_CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            cidr.insert(proxyModel->index(i, 0).data().toString());
        break;
    case ExplorerType::enum_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));

            for(int j = 0; j < item->prefixes->rowCount(); j++)
                cidr.insert(item->prefixes->child(j, 1)->text());
        }
        break;
    }

    emit sendToCidrEnum(cidr, RESULT_TYPE::CIDR);
    emit changeTabToCidrEnum();
}

void Project::action_send_ssl(){
    QSet<QString> ssl;
    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_SSL:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            ssl.insert(proxyModel->index(i, 0).data().toString());
        break;
    }

    emit sendToSSLEnum(ssl, RESULT_TYPE::CERT_ID);
    emit changeTabToSSLEnum();
}

void Project::action_send_ns(){
    QSet<QString> ns;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::enum_NS:
    case ExplorerType::passive_NS:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            ns.insert(proxyModel->index(i, 0).data().toString());
        break;
    case ExplorerType::activeDNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->NS->rowCount(); j++)
                ns.insert(item->NS->child(j, 1)->text());
        }
        break;
    }

    emit sendToNSEnum(ns, RESULT_TYPE::NS);
    emit changeTabToNSEnum();
}

void Project::action_send_mx(){
    QSet<QString> mx;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::enum_MX:
    case ExplorerType::passive_MX:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            mx.insert(proxyModel->index(i, 0).data().toString());
        break;
    case ExplorerType::activeDNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->MX->rowCount(); j++)
                mx.insert(item->MX->child(j, 1)->text());
        }
        break;
    }

    emit sendToMXEnum(mx, RESULT_TYPE::MX);
    emit changeTabToMXEnum();
}

void Project::action_openInBrowser(){
    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeURL:
    case ExplorerType::passive_URL:
        foreach(const QModelIndex &index, m_selectionModel->selectedIndexes())
            QDesktopServices::openUrl(QUrl(index.data().toString(), QUrl::TolerantMode));
        break;
    default:
        foreach(const QModelIndex &index, m_selectionModel->selectedIndexes())
            QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
        break;
    }
}

void Project::action_save_selected(){
    QString filename = QFileDialog::getSaveFileName(this, tr("Save To File"), "./");
    if(filename.isEmpty()){
        qWarning() << "PROJECT: Failed to getSaveFileName";
        return;
    }

    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(!file.isOpen()){
        qWarning() << "PROJECT: Failed to open " << filename << " For saving Results";
        return;
    }

    foreach(const QModelIndex &index, m_selectionModel->selectedIndexes())
        file.write(index.data().toString().append(NEWLINE).toUtf8());
    file.close();
}

void Project::action_copy_selected(){
    QString clipboardData;
    foreach(const QModelIndex &index, m_selectionModel->selectedIndexes())
        clipboardData.append(index.data().toString().append(NEWLINE));

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(clipboardData.trimmed());
}

void Project::action_send_selected_toEnum(const ENUMERATOR &tool){
    QSet<QString> targets;
    foreach(const QModelIndex &index, m_selectionModel->selectedIndexes())
        targets.insert(index.data().toString());

    switch (tool) {
    case ENUMERATOR::IP:
        emit sendToIpEnum(targets, RESULT_TYPE::IP);
        emit changeTabToIpEnum();
        break;
    case ENUMERATOR::ASN:
        emit sendToAsnEnum(targets, RESULT_TYPE::ASN);
        emit changeTabToAsnEnum();
        break;
    case ENUMERATOR::CIDR:
        emit sendToCidrEnum(targets, RESULT_TYPE::CIDR);
        emit changeTabToCidrEnum();
        break;
    case ENUMERATOR::NS:
        emit sendToNSEnum(targets, RESULT_TYPE::NS);
        emit changeTabToNSEnum();
        break;
    case ENUMERATOR::MX:
        emit sendToMXEnum(targets, RESULT_TYPE::MX);
        emit changeTabToMXEnum();
        break;
    case ENUMERATOR::SSL:
        emit sendToSSLEnum(targets, RESULT_TYPE::CERT_ID);
        emit changeTabToSSLEnum();
        break;
    case ENUMERATOR::EMAIL:
        emit sendToEmailEnum(targets, RESULT_TYPE::EMAIL);
        emit changeTabToEmailEnum();
        break;
    }
}

void Project::action_send_selected_toEngine(const TOOL &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;
    foreach(const QModelIndex &index, m_selectionModel->selectedIndexes())
        targets.insert(index.data().toString());

    switch (engine) {
    case TOOL::OSINT:
        emit sendToOsint(targets, result_type);
        emit changeTabToOsint();
        break;
    case TOOL::RAW:
        emit sendToRaw(targets, result_type);
        emit changeTabToRaw();
        break;
    case TOOL::BRUTE:
        emit sendToBrute(targets, result_type);
        emit changeTabToBrute();
        break;
    case TOOL::HOST:
        emit sendToHost(targets, result_type);
        emit changeTabToHost();
        break;
    case TOOL::IP:
        emit sendToIP(targets, result_type);
        emit changeTabToIP();
        break;
    case TOOL::DNS:
        emit sendToDns(targets, result_type);
        emit changeTabToDns();
        break;
    case TOOL::SSL:
        emit sendToSsl(targets, result_type);
        emit changeTabToSSL();
        break;
    case TOOL::URL:
        emit sendToUrl(targets, result_type);
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

void Project::action_remove_selected(){
    auto model_selectedIndexes = proxyModel->mapSelectionToSource(m_selectionModel->selection());
    QModelIndexList selectedIndexes = model_selectedIndexes.indexes();

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeHost:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->activeHost->invisibleRootItem()->index()) && (i->column() == 0)){
                model->map_activeHost.remove(i->data().toString());
                model->activeHost->removeRow(i->row());
            }
        }
        break;
    case ExplorerType::activeWildcard:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->activeWildcard->invisibleRootItem()->index()) && (i->column() == 0)){
                model->map_activeWildcard.remove(i->data().toString());
                model->activeWildcard->removeRow(i->row());
            }
        }
        break;
    case ExplorerType::activeDNS:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->activeDNS->invisibleRootItem()->index()) && (i->column() == 0)){
                model->map_activeDNS.remove(i->data().toString());
                model->activeDNS->removeRow(i->row());
            }
        }
        break;
    case ExplorerType::activeDNS_A:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->activeA->removeRow(i->row());
        break;
    case ExplorerType::activeDNS_AAAA:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->activeAAAA->removeRow(i->row());
        break;
    case ExplorerType::activeDNS_NS:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->activeNS->removeRow(i->row());
        break;
    case ExplorerType::activeDNS_MX:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->activeMX->removeRow(i->row());
        break;
    case ExplorerType::activeDNS_TXT:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->activeTXT->removeRow(i->row());
        break;
    case ExplorerType::activeDNS_CNAME:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->activeCNAME->removeRow(i->row());
        break;
    case ExplorerType::activeSSL:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->activeSSL->invisibleRootItem()->index()) && (i->column() == 0)){
                model->map_activeSSL.remove(i->data().toString());
                model->activeSSL->removeRow(i->row());
            }
        }
        break;
    case ExplorerType::activeSSL_sha1:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->activeSSL_sha1->removeRow(i->row());
        break;
    case ExplorerType::activeSSL_sha256:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->activeSSL_sha256->removeRow(i->row());
        break;
    case ExplorerType::activeSSL_altNames:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->activeSSL_altNames->removeRow(i->row());
        break;
    case ExplorerType::activeURL:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->activeURL->invisibleRootItem()->index()) && (i->column() == 0)){
                model->map_activeURL.remove(i->data().toString());
                model->activeURL->removeRow(i->row());
            }
        }
        break;
    case ExplorerType::passive_subdomainIp:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->passiveSubdomainIp->removeRow(i->row());
        break;
    case ExplorerType::passive_subdomain:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->passiveSubdomain->removeRow(i->row());
        break;
    case ExplorerType::passive_A:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->passiveA->removeRow(i->row());
        break;
    case ExplorerType::passive_AAAA:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->passiveAAAA->removeRow(i->row());
        break;
    case ExplorerType::passive_NS:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->passiveNS->removeRow(i->row());
        break;
    case ExplorerType::passive_MX:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->passiveMX->removeRow(i->row());
        break;
    case ExplorerType::passive_TXT:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->passiveTXT->removeRow(i->row());
        break;
    case ExplorerType::passive_CNAME:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->passiveCNAME->removeRow(i->row());
        break;
    case ExplorerType::passive_Email:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->passiveEmail->removeRow(i->row());
        break;
    case ExplorerType::passive_URL:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->passiveURL->removeRow(i->row());
        break;
    case ExplorerType::passive_ASN:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->passiveASN->removeRow(i->row());
        break;
    case ExplorerType::passive_SSL:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
            model->passiveSSL->removeRow(i->row());
        break;
    case ExplorerType::enum_IP:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->enumIP->invisibleRootItem()->index()) && (i->column() == 0)){
                model->map_enumIP.remove(i->data().toString());
                model->enumIP->removeRow(i->row());
            }
        }
        break;
    case ExplorerType::enum_ASN:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->enumASN->invisibleRootItem()->index()) && (i->column() == 0)){
                model->map_enumASN.remove(i->data().toString());
                model->enumASN->removeRow(i->row());
            }
        }
        break;
    case ExplorerType::enum_CIDR:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->enumCIDR->invisibleRootItem()->index()) && (i->column() == 0)){
                model->map_enumCIDR.remove(i->data().toString());
                model->enumCIDR->removeRow(i->row());
            }
        }
        break;
    case ExplorerType::enum_NS:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->enumNS->invisibleRootItem()->index()) && (i->column() == 0)){
                model->map_enumNS.remove(i->data().toString());
                model->enumNS->removeRow(i->row());
            }
        }
        break;
    case ExplorerType::enum_MX:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->enumMX->invisibleRootItem()->index()) && (i->column() == 0)){
                model->map_enumMX.remove(i->data().toString());
                model->enumMX->removeRow(i->row());
            }
        }
        break;
    case ExplorerType::enum_Email:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->enumEmail->invisibleRootItem()->index()) && (i->column() == 0)){
                model->map_enumEmail.remove(i->data().toString());
                model->enumEmail->removeRow(i->row());
            }
        }
        break;
    case ExplorerType::enum_SSL:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->enumSSL->invisibleRootItem()->index()) && (i->column() == 0)){
                model->map_enumSSL.remove(i->data().toString());
                model->enumSSL->removeRow(i->row());
            }
        }
        break;
    case ExplorerType::raw:
        for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i){
            if((i->parent() == model->raw->invisibleRootItem()->index()) && (i->column() == 0))
                model->raw->removeRow(i->row());
        }
        break;
    }

    ui->labelCount->setNum(proxyModel->rowCount());
    model->modified = true;
}

void Project::action_extract(bool subdomain, bool tld, bool url){
    QSet<QString> items;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeHost:
    case ExplorerType::activeURL:
    case ExplorerType::activeDNS_NS:
    case ExplorerType::activeDNS_MX:
    case ExplorerType::activeDNS_CNAME:
    case ExplorerType::activeSSL_altNames:
    case ExplorerType::passive_subdomainIp:
    case ExplorerType::passive_subdomain:
    case ExplorerType::passive_NS:
    case ExplorerType::passive_MX:
    case ExplorerType::passive_CNAME:
    case ExplorerType::passive_URL:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            items.insert(proxyModel->index(i,0).data().toString());
        break;
    case ExplorerType::activeDNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                items.insert(item->CNAME->child(j, 1)->text());
            for(int j = 0; j < item->NS->rowCount(); j++)
                items.insert(item->NS->child(j, 1)->text());
            for(int j = 0; j < item->MX->rowCount(); j++)
                items.insert(item->MX->child(j, 1)->text());
            for(int j = 0; j < item->SRV->rowCount(); j++)
                items.insert(item->SRV->child(j, 1)->text());
        }
        break;
    case ExplorerType::activeSSL:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(model->activeSSL->itemFromIndex(model_index));

            for(int j = 0; j < item->subjectAltNames->rowCount(); j++)
                items.insert(item->subjectAltNames->child(j, 1)->text());
        }
        break;
    case ExplorerType::enum_MX:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::MX *item = static_cast<s3s_item::MX*>(model->enumMX->itemFromIndex(model_index));

            for(int j = 0; j < item->domains->rowCount(); j++)
                items.insert(item->domains->child(j, 1)->text());
        }
        break;
    case ExplorerType::enum_NS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(model->enumNS->itemFromIndex(model_index));

            for(int j = 0; j < item->domains->rowCount(); j++)
                items.insert(item->domains->child(j, 1)->text());
        }
        break;
    case ExplorerType::enum_SSL:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(model->enumSSL->itemFromIndex(model_index));

            for(int j = 0; j < item->subjectAltNames->rowCount(); j++)
                items.insert(item->subjectAltNames->child(j, 1)->text());
        }
        break;
    }

    /* extracting and saving to a set to avoid repeatition */
    QSet<QString> extracts;
    foreach(const QString &item, items){
        if(subdomain)
            extracts.insert(item.split(".").at(0));
        if(tld)
            extracts.insert(item.split(".").last());
        if(url){
            QString _url = item;
            extracts.insert(_url.remove("https://").remove("http://").split("/").at(0));
        }
    }

    /* setting the data to clipboard */
    QString data;
    foreach(const QString &extract, extracts)
        data.append(extract).append(NEWLINE);

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(data.trimmed());
}

void Project::action_extract_selected(bool subdomain, bool tld, bool url){
    QSet<QString> items;
    foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes())
        items.insert(proxyIndex.data().toString());

    /* extracting and saving to a set to avoid repeatition */
    QSet<QString> extracts;
    foreach(const QString &item, items){
        if(subdomain)
            extracts.insert(item.split(".").at(0));
        if(tld)
            extracts.insert(item.split(".").last());
        if(url){
            QString _url = item;
            extracts.insert(_url.remove("https://").remove("http://").split("/").at(0));
        }
    }

    /* setting the data to clipboard */
    QString data;
    foreach(const QString &extract, extracts)
        data.append(extract).append(NEWLINE);

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(data.trimmed());
}

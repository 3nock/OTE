#include "Project.h"
#include "ui_Project.h"

#include <QFileDialog>
#include <QClipboard>
#include "src/items/SSLItem.h"


void Project::action_clear(){
    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeHost:
        model->activeHost->clear();
        model->set_Host.clear();
        break;
    case ExplorerType::activeWildcard:
        model->activeWildcard->clear();
        break;
    case ExplorerType::activeDNS:
        model->activeDNS->clear();
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
        model->passiveUrl->clear();
        break;
    case ExplorerType::passive_ASN:
        model->passiveAsn->clear();
        break;
    case ExplorerType::passive_SSL:
        model->passiveSSL->clear();
        break;
    case ExplorerType::enum_IP:
        model->enumIp->clear();
        break;
    case ExplorerType::enum_ASN:
        model->enumASN->clear();
        break;
    case ExplorerType::enum_CIDR:
        model->enumCIDR->clear();
        break;
    case ExplorerType::enum_NS:
        model->enumNS->clear();
        break;
    case ExplorerType::enum_MX:
        model->enumMX->clear();
        break;
    case ExplorerType::enum_Email:
        model->enumEmail->clear();
        break;
    case ExplorerType::enum_SSL:
        model->enumSSL->clear();
        break;
    case ExplorerType::raw:
        model->raw->clear();
        break;
    }

    model->setHeaderLabels();
    ui->labelCount->setNum(proxyModel->rowCount());
}

void Project::action_save(){
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
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QString host(proxyModel->index(i, 0).data().toString());
            QString ip(proxyModel->index(i, 1).data().toString());

            if(!ip.isEmpty())
                host.append(",").append(ip);

            file.write(host.append(NEWLINE).toUtf8());
        }
        break;
    case ExplorerType::activeDNS_SRV:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QString name(proxyModel->index(i, 0).data().toString());
            QString target(proxyModel->index(i, 1).data().toString());
            QString port(proxyModel->index(i, 1).data().toString());

            if(!target.isEmpty())
                name.append(",").append(target);
            if(!port.isEmpty())
                name.append(",").append(port);

            file.write(name.append(NEWLINE).toUtf8());
        }
        break;
    case ExplorerType::activeHost:
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
    case ExplorerType::activeWildcard:
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
            s3s_item::IP *item = static_cast<s3s_item::IP*>(model->activeHost->itemFromIndex(model_index));
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

void Project::action_copy(){
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
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QString host(proxyModel->index(i, 0).data().toString());
            QString ip(proxyModel->index(i, 1).data().toString());

            if(!ip.isEmpty())
                host.append(",").append(ip);

            clipboardData.append(host.append(NEWLINE));
        }
        break;
    case ExplorerType::activeDNS_SRV:
        for(int i = 0; i != proxyModel->rowCount(); ++i){
            QString name(proxyModel->index(i, 0).data().toString());
            QString target(proxyModel->index(i, 1).data().toString());
            QString port(proxyModel->index(i, 1).data().toString());

            if(!target.isEmpty())
                name.append(",").append(target);
            if(!port.isEmpty())
                name.append(",").append(port);

            clipboardData.append(name.append(NEWLINE));
        }
        break;
    case ExplorerType::activeHost:
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
    case ExplorerType::activeWildcard:
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
            s3s_item::IP *item = static_cast<s3s_item::IP*>(model->activeHost->itemFromIndex(model_index));
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

void Project::action_send(){
    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeHost:
        break;
    case ExplorerType::activeWildcard:
        break;
    case ExplorerType::activeDNS:
        break;
    case ExplorerType::activeDNS_A:
        break;
    case ExplorerType::activeDNS_AAAA:
        break;
    case ExplorerType::activeDNS_NS:
        break;
    case ExplorerType::activeDNS_MX:
        break;
    case ExplorerType::activeDNS_TXT:
        break;
    case ExplorerType::activeDNS_CNAME:
        break;
    case ExplorerType::activeSSL:
        break;
    case ExplorerType::activeSSL_sha1:
        break;
    case ExplorerType::activeSSL_sha256:
        break;
    case ExplorerType::activeSSL_altNames:
        break;
    case ExplorerType::activeURL:
        break;
    case ExplorerType::passive_subdomainIp:
        break;
    case ExplorerType::passive_subdomain:
        break;
    case ExplorerType::passive_A:
        break;
    case ExplorerType::passive_AAAA:
        break;
    case ExplorerType::passive_NS:
        break;
    case ExplorerType::passive_MX:
        break;
    case ExplorerType::passive_TXT:
        break;
    case ExplorerType::passive_CNAME:
        break;
    case ExplorerType::passive_Email:
        break;
    case ExplorerType::passive_URL:
        break;
    case ExplorerType::passive_ASN:
        break;
    case ExplorerType::passive_SSL:
        break;
    case ExplorerType::enum_IP:
        break;
    case ExplorerType::enum_ASN:
        break;
    case ExplorerType::enum_CIDR:
        break;
    case ExplorerType::enum_NS:
        break;
    case ExplorerType::enum_MX:
        break;
    case ExplorerType::enum_Email:
        break;
    case ExplorerType::enum_SSL:
        break;
    case ExplorerType::raw:
        break;
    }
}

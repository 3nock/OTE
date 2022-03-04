#include "Project.h"
#include "ui_Project.h"

#include <QDesktopServices>
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

void Project::action_send_host(const ENGINE &engine){
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
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                hostnames.insert(item->CNAME->child(i, 1)->text());
            for(int j = 0; j < item->NS->rowCount(); j++)
                hostnames.insert(item->NS->child(i, 1)->text());
            for(int j = 0; j < item->MX->rowCount(); j++)
                hostnames.insert(item->MX->child(i, 1)->text());
        }
        break;
    case ExplorerType::activeSSL:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(model->activeSSL->itemFromIndex(model_index));

            for(int j = 0; j < item->subjectAltNames->rowCount(); j++)
                hostnames.insert(item->subjectAltNames->child(i, 1)->text());
        }
        break;
    case ExplorerType::enum_MX:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::MX *item = static_cast<s3s_item::MX*>(model->enumMX->itemFromIndex(model_index));

            for(int j = 0; j < item->domains->rowCount(); j++)
                hostnames.insert(item->domains->child(i, 1)->text());
        }
        break;
    case ExplorerType::enum_NS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(model->enumNS->itemFromIndex(model_index));

            for(int j = 0; j < item->domains->rowCount(); j++)
                hostnames.insert(item->domains->child(i, 1)->text());
        }
        break;
    case ExplorerType::enum_SSL:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(model->enumSSL->itemFromIndex(model_index));

            for(int j = 0; j < item->subjectAltNames->rowCount(); j++)
                hostnames.insert(item->subjectAltNames->child(i, 1)->text());
        }
        break;
    }

    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(hostnames, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(hostnames, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        emit sendToBrute(hostnames, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        emit sendToActive(hostnames, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        emit sendToDns(hostnames, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToDns();
        break;
    case ENGINE::SSL:
        emit sendToSsl(hostnames, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Project::action_send_ip(const ENGINE &engine){
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
            ip.insert(proxyModel->index(i, 0).data().toString());
            ip.insert(proxyModel->index(i, 1).data().toString());
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
                ip.insert(item->A->child(i, 1)->text());
            for(int j = 0; j < item->AAAA->rowCount(); j++)
                ip.insert(item->AAAA->child(i, 1)->text());
        }
        break;
    }

    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(ip, RESULT_TYPE::IP);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(ip, RESULT_TYPE::IP);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Project::action_send_url(const ENGINE &engine){
    QSet<QString> url;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeURL:
    case ExplorerType::passive_URL:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            url.insert(proxyModel->index(i, 1).data().toString());
        break;
    }

    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(url, RESULT_TYPE::URL);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(url, RESULT_TYPE::URL);
        emit changeTabToRaw();
        break;
    case ENGINE::URL:
        emit sendToUrl(url, RESULT_TYPE::URL);
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

void Project::action_send_email(const ENGINE &engine){
    QSet<QString> emails;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_Email:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            emails.insert(proxyModel->index(i, 1).data().toString());
        break;
    case ExplorerType::enum_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));

            for(int j = 0; j < item->abuseContacts->rowCount(); j++)
                emails.insert(item->abuseContacts->child(i, 1)->text());
            for(int j = 0; j < item->emailContacts->rowCount(); j++)
                emails.insert(item->emailContacts->child(i, 1)->text());
        }
        break;
    case ExplorerType::enum_CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::CIDR *item = static_cast<s3s_item::CIDR*>(model->enumCIDR->itemFromIndex(model_index));

            for(int j = 0; j < item->abuseContacts->rowCount(); j++)
                emails.insert(item->abuseContacts->child(i, 1)->text());
            for(int j = 0; j < item->emailContacts->rowCount(); j++)
                emails.insert(item->emailContacts->child(i, 1)->text());
        }
        break;
    }

    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(emails, RESULT_TYPE::EMAIL);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(emails, RESULT_TYPE::EMAIL);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Project::action_send_asn(const ENGINE &engine){
    QSet<QString> asn;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            asn.insert(proxyModel->index(i, 1).data().toString());
        break;
    case ExplorerType::enum_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));

            for(int j = 0; j < item->peers->rowCount(); j++)
                asn.insert(item->peers->child(i, 1)->text());
        }
        break;
    case ExplorerType::enum_CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::CIDR *item = static_cast<s3s_item::CIDR*>(model->enumCIDR->itemFromIndex(model_index));

            for(int j = 0; j < item->asns->rowCount(); j++)
                asn.insert(item->asns->child(i, 1)->text());
        }
        break;
    }

    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(asn, RESULT_TYPE::ASN);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(asn, RESULT_TYPE::ASN);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Project::action_send_cidr(const ENGINE &engine){
    QSet<QString> cidr;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            cidr.insert(proxyModel->index(i, 1).data().toString());
        break;
    case ExplorerType::enum_NS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));

            for(int j = 0; j < item->prefixes->rowCount(); j++)
                cidr.insert(item->prefixes->child(i, 1)->text());
        }
        break;
    }

    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(cidr, RESULT_TYPE::CIDR);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(cidr, RESULT_TYPE::CIDR);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Project::action_send_ssl(const ENGINE &engine){
    QSet<QString> ssl;
    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_SSL:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            ssl.insert(proxyModel->index(i, 1).data().toString());
        break;
    }

    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(ssl, RESULT_TYPE::CERT_ID);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(ssl, RESULT_TYPE::CERT_ID);
        emit changeTabToRaw();
        break;
    default:
        break;
    }
}

void Project::action_send_ns(const ENGINE &engine){
    QSet<QString> ns;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeDNS_NS:
    case ExplorerType::passive_NS:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            ns.insert(proxyModel->index(i, 1).data().toString());
        break;
    case ExplorerType::activeDNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->NS->rowCount(); j++)
                ns.insert(item->NS->child(i, 1)->text());
        }
        break;
    }

    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(ns, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(ns, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        emit sendToBrute(ns, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        emit sendToActive(ns, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        emit sendToDns(ns, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToDns();
        break;
    case ENGINE::SSL:
        emit sendToSsl(ns, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToSSL();
        break;
    default:
        break;
    }
}

void Project::action_send_mx(const ENGINE &engine){
    QSet<QString> mx;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeDNS_MX:
    case ExplorerType::passive_MX:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            mx.insert(proxyModel->index(i, 1).data().toString());
        break;
    case ExplorerType::activeDNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->MX->rowCount(); j++)
                mx.insert(item->MX->child(i, 1)->text());
        }
        break;
    }

    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(mx, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(mx, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        emit sendToBrute(mx, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        emit sendToActive(mx, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        emit sendToDns(mx, RESULT_TYPE::SUBDOMAIN);
        emit changeTabToDns();
        break;
    case ENGINE::SSL:
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
            ip.insert(proxyModel->index(i, 0).data().toString());
            ip.insert(proxyModel->index(i, 1).data().toString());
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
                ip.insert(item->A->child(i, 1)->text());
            for(int j = 0; j < item->AAAA->rowCount(); j++)
                ip.insert(item->AAAA->child(i, 1)->text());
        }
        break;
    }

    emit sendToIpEnum(ip);
    emit changeTabToIpEnum();
}

void Project::action_send_email(){
    QSet<QString> emails;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_Email:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            emails.insert(proxyModel->index(i, 1).data().toString());
        break;
    case ExplorerType::enum_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));

            for(int j = 0; j < item->abuseContacts->rowCount(); j++)
                emails.insert(item->abuseContacts->child(i, 1)->text());
            for(int j = 0; j < item->emailContacts->rowCount(); j++)
                emails.insert(item->emailContacts->child(i, 1)->text());
        }
        break;
    case ExplorerType::enum_CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::CIDR *item = static_cast<s3s_item::CIDR*>(model->enumCIDR->itemFromIndex(model_index));

            for(int j = 0; j < item->abuseContacts->rowCount(); j++)
                emails.insert(item->abuseContacts->child(i, 1)->text());
            for(int j = 0; j < item->emailContacts->rowCount(); j++)
                emails.insert(item->emailContacts->child(i, 1)->text());
        }
        break;
    }

    emit sendToEmailEnum(emails);
    emit changeTabToEmailEnum();
}

void Project::action_send_asn(){
    QSet<QString> asn;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            asn.insert(proxyModel->index(i, 1).data().toString());
        break;
    case ExplorerType::enum_ASN:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));

            for(int j = 0; j < item->peers->rowCount(); j++)
                asn.insert(item->peers->child(i, 1)->text());
        }
        break;
    case ExplorerType::enum_CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::CIDR *item = static_cast<s3s_item::CIDR*>(model->enumCIDR->itemFromIndex(model_index));

            for(int j = 0; j < item->asns->rowCount(); j++)
                asn.insert(item->asns->child(i, 1)->text());
        }
        break;
    }

    emit sendToAsnEnum(asn);
    emit changeTabToAsnEnum();
}

void Project::action_send_cidr(){
    QSet<QString> cidr;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_CIDR:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            cidr.insert(proxyModel->index(i, 1).data().toString());
        break;
    case ExplorerType::enum_NS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::ASN *item = static_cast<s3s_item::ASN*>(model->enumASN->itemFromIndex(model_index));

            for(int j = 0; j < item->prefixes->rowCount(); j++)
                cidr.insert(item->prefixes->child(i, 1)->text());
        }
        break;
    }

    emit sendToCidrEnum(cidr);
    emit changeTabToCidrEnum();
}

void Project::action_send_ssl(){
    QSet<QString> ssl;
    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::passive_SSL:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            ssl.insert(proxyModel->index(i, 1).data().toString());
        break;
    }

    emit sendToSSLEnum(ssl);
    emit changeTabToSSLEnum();
}

void Project::action_send_ns(){
    QSet<QString> ns;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeDNS_NS:
    case ExplorerType::passive_NS:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            ns.insert(proxyModel->index(i, 1).data().toString());
        break;
    case ExplorerType::activeDNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->NS->rowCount(); j++)
                ns.insert(item->NS->child(i, 1)->text());
        }
        break;
    }

    emit sendToNSEnum(ns);
    emit changeTabToNSEnum();
}

void Project::action_send_mx(){
    QSet<QString> mx;

    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeDNS_MX:
    case ExplorerType::passive_MX:
        for(int i = 0; i < proxyModel->rowCount(); i++)
            mx.insert(proxyModel->index(i, 1).data().toString());
        break;
    case ExplorerType::activeDNS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->MX->rowCount(); j++)
                mx.insert(item->MX->child(i, 1)->text());
        }
        break;
    }

    emit sendToMXEnum(mx);
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

void Project::action_send_selected_toEnum(const TOOL &tool){
    QSet<QString> targets;
    foreach(const QModelIndex &index, m_selectionModel->selectedIndexes())
        targets.insert(index.data().toString());

    switch (tool) {
    case TOOL::IP:
        emit sendToIpEnum(targets);
        emit changeTabToIpEnum();
        break;
    case TOOL::ASN:
        emit sendToAsnEnum(targets);
        emit changeTabToAsnEnum();
        break;
    case TOOL::CIDR:
        emit sendToCidrEnum(targets);
        emit changeTabToCidrEnum();
        break;
    case TOOL::NS:
        emit sendToNSEnum(targets);
        emit changeTabToNSEnum();
        break;
    case TOOL::MX:
        emit sendToMXEnum(targets);
        emit changeTabToMXEnum();
        break;
    case TOOL::SSL:
        emit sendToSSLEnum(targets);
        emit changeTabToSSLEnum();
        break;
    case TOOL::EMAIL:
        emit sendToEmailEnum(targets);
        emit changeTabToEmailEnum();
        break;
    }
}

void Project::action_send_selected_toEngine(const ENGINE &engine, const RESULT_TYPE &result_type){
    QSet<QString> targets;
    foreach(const QModelIndex &index, m_selectionModel->selectedIndexes())
        targets.insert(index.data().toString());

    switch (engine) {
    case ENGINE::OSINT:
        emit sendToOsint(targets, result_type);
        emit changeTabToOsint();
        break;
    case ENGINE::RAW:
        emit sendToRaw(targets, result_type);
        emit changeTabToRaw();
        break;
    case ENGINE::BRUTE:
        emit sendToBrute(targets, result_type);
        emit changeTabToBrute();
        break;
    case ENGINE::ACTIVE:
        emit sendToActive(targets, result_type);
        emit changeTabToActive();
        break;
    case ENGINE::DNS:
        emit sendToDns(targets, result_type);
        emit changeTabToDns();
        break;
    case ENGINE::SSL:
        emit sendToSsl(targets, result_type);
        emit changeTabToSSL();
        break;
    case ENGINE::URL:
        emit sendToUrl(targets, result_type);
        emit changeTabToURL();
        break;
    default:
        break;
    }
}

void Project::action_remove_selected(){
    switch (ui->treeViewTree->property(SITEMAP_TYPE).toInt()) {
    case ExplorerType::activeHost:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->activeHost->removeRow(index.row());
        }
        break;
    case ExplorerType::activeWildcard:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->activeWildcard->removeRow(index.row());
        }
        break;
    case ExplorerType::activeDNS:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            if(index.parent() == model->activeDNS->invisibleRootItem()->index())
                model->activeDNS->removeRow(index.row());
        }
        break;
    case ExplorerType::activeDNS_A:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->activeA->removeRow(index.row());
        }
        break;
    case ExplorerType::activeDNS_AAAA:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->activeAAAA->removeRow(index.row());
        }
        break;
    case ExplorerType::activeDNS_NS:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->activeNS->removeRow(index.row());
        }
        break;
    case ExplorerType::activeDNS_MX:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->activeHost->removeRow(index.row());
        }
        break;
    case ExplorerType::activeDNS_TXT:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->activeTXT->removeRow(index.row());
        }
        break;
    case ExplorerType::activeDNS_CNAME:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->activeCNAME->removeRow(index.row());
        }
        break;
    case ExplorerType::activeSSL:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            if(index.parent() == model->activeSSL->invisibleRootItem()->index())
                model->activeSSL->removeRow(index.row());
        }
        break;
    case ExplorerType::activeSSL_sha1:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->activeSSL_sha1->removeRow(index.row());
        }
        break;
    case ExplorerType::activeSSL_sha256:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->activeSSL_sha256->removeRow(index.row());
        }
        break;
    case ExplorerType::activeSSL_altNames:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->activeSSL_altNames->removeRow(index.row());
        }
        break;
    case ExplorerType::activeURL:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->activeURL->removeRow(index.row());
        }
        break;
    case ExplorerType::passive_subdomainIp:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->passiveSubdomainIp->removeRow(index.row());
        }
        break;
    case ExplorerType::passive_subdomain:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->passiveSubdomain->removeRow(index.row());
        }
        break;
    case ExplorerType::passive_A:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->passiveA->removeRow(index.row());
        }
        break;
    case ExplorerType::passive_AAAA:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->passiveAAAA->removeRow(index.row());
        }
        break;
    case ExplorerType::passive_NS:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->passiveNS->removeRow(index.row());
        }
        break;
    case ExplorerType::passive_MX:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->passiveMX->removeRow(index.row());
        }
        break;
    case ExplorerType::passive_TXT:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->passiveTXT->removeRow(index.row());
        }
        break;
    case ExplorerType::passive_CNAME:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->passiveCNAME->removeRow(index.row());
        }
        break;
    case ExplorerType::passive_Email:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->passiveEmail->removeRow(index.row());
        }
        break;
    case ExplorerType::passive_URL:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->passiveUrl->removeRow(index.row());
        }
        break;
    case ExplorerType::passive_ASN:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->passiveAsn->removeRow(index.row());
        }
        break;
    case ExplorerType::passive_SSL:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            model->passiveSSL->removeRow(index.row());
        }
        break;
    case ExplorerType::enum_IP:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            if(index.parent() == model->enumIp->invisibleRootItem()->index())
                model->enumIp->removeRow(index.row());
        }
        break;
    case ExplorerType::enum_ASN:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            if(index.parent() == model->enumASN->invisibleRootItem()->index())
                model->enumASN->removeRow(index.row());
        }
        break;
    case ExplorerType::enum_CIDR:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            if(index.parent() == model->enumCIDR->invisibleRootItem()->index())
                model->enumCIDR->removeRow(index.row());
        }
        break;
    case ExplorerType::enum_NS:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            if(index.parent() == model->enumNS->invisibleRootItem()->index())
                model->enumNS->removeRow(index.row());
        }
        break;
    case ExplorerType::enum_MX:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            if(index.parent() == model->enumMX->invisibleRootItem()->index())
                model->enumMX->removeRow(index.row());
        }
        break;
    case ExplorerType::enum_Email:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            if(index.parent() == model->enumEmail->invisibleRootItem()->index())
                model->enumEmail->removeRow(index.row());
        }
        break;
    case ExplorerType::enum_SSL:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            if(index.parent() == model->enumSSL->invisibleRootItem()->index())
                model->enumSSL->removeRow(index.row());
        }
        break;
    case ExplorerType::raw:
        foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes()){
            QModelIndex index = proxyModel->mapToSource(proxyIndex);
            if(index.parent() == model->raw->invisibleRootItem()->index())
                model->raw->removeRow(index.row());
        }
        break;
    }

    ui->labelCount->setNum(proxyModel->rowCount());
}

void Project::action_extract(bool subdomain, bool tld){
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
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::DNS *item = static_cast<s3s_item::DNS*>(model->activeDNS->itemFromIndex(model_index));

            for(int j = 0; j < item->CNAME->rowCount(); j++)
                hostnames.insert(item->CNAME->child(i, 1)->text());
            for(int j = 0; j < item->NS->rowCount(); j++)
                hostnames.insert(item->NS->child(i, 1)->text());
            for(int j = 0; j < item->MX->rowCount(); j++)
                hostnames.insert(item->MX->child(i, 1)->text());
        }
        break;
    case ExplorerType::activeSSL:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(model->activeSSL->itemFromIndex(model_index));

            for(int j = 0; j < item->subjectAltNames->rowCount(); j++)
                hostnames.insert(item->subjectAltNames->child(i, 1)->text());
        }
        break;
    case ExplorerType::enum_MX:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::MX *item = static_cast<s3s_item::MX*>(model->enumMX->itemFromIndex(model_index));

            for(int j = 0; j < item->domains->rowCount(); j++)
                hostnames.insert(item->domains->child(i, 1)->text());
        }
        break;
    case ExplorerType::enum_NS:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::NS *item = static_cast<s3s_item::NS*>(model->enumNS->itemFromIndex(model_index));

            for(int j = 0; j < item->domains->rowCount(); j++)
                hostnames.insert(item->domains->child(i, 1)->text());
        }
        break;
    case ExplorerType::enum_SSL:
        for(int i = 0; i < proxyModel->rowCount(); i++){
            QModelIndex model_index = proxyModel->mapToSource(proxyModel->index(i, 0));
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(model->enumSSL->itemFromIndex(model_index));

            for(int j = 0; j < item->subjectAltNames->rowCount(); j++)
                hostnames.insert(item->subjectAltNames->child(i, 1)->text());
        }
        break;
    }

    /* extracting and saving to a set to avoid repeatition */
    QSet<QString> extracts;
    foreach(const QString &hostname, hostnames){
        if(subdomain)
            extracts.insert(hostname.split(".").at(0));
        if(tld)
            extracts.insert(hostname.split(".").last());
    }

    /* setting the data to clipboard */
    QString data;
    foreach(const QString &extract, extracts)
        data.append(extract).append(NEWLINE);

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(data.trimmed());
}

void Project::action_extract_selected(bool subdomain, bool tld){
    QSet<QString> hostnames;
    foreach(const QModelIndex &proxyIndex, m_selectionModel->selectedIndexes())
        hostnames.insert(proxyIndex.data().toString());

    /* extracting and saving to a set to avoid repeatition */
    QSet<QString> extracts;
    foreach(const QString &hostname, hostnames){
        if(subdomain)
            extracts.insert(hostname.split(".").at(0));
        if(tld)
            extracts.insert(hostname.split(".").last());
    }

    /* setting the data to clipboard */
    QString data;
    foreach(const QString &extract, extracts)
        data.append(extract).append(NEWLINE);

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(data.trimmed());
}

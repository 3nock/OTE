#include "Project.h"
#include "ui_Project.h"


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

}

void Project::action_copy(){

}

void Project::action_send(){

}

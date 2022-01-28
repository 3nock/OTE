#include "Project.h"
#include "ui_Project.h"


void Project::action_clear(){
    switch (ui->treeViewSiteMap->property(PROJECT_MODEL_TYPE).toInt()) {
    case ModelType::activeSubdomainIp:
        m_projectModel->activeSubdomainIp_model->clear();
        break;
    case ModelType::activeSubdomain:
        m_projectModel->activeSubdomain_model->clear();
        break;
    case ModelType::activeTld:
        m_projectModel->activeTld_model->clear();
        break;
    case ModelType::activeWildcard:
        m_projectModel->activeWildcard_model->clear();
        break;
    case ModelType::activeDNS:
        m_projectModel->activeDNS_model->clear();
        break;
    case ModelType::activeDNS_A:
        m_projectModel->activeA_model->clear();
        break;
    case ModelType::activeDNS_AAAA:
        m_projectModel->activeAAAA_model->clear();
        break;
    case ModelType::activeDNS_NS:
        m_projectModel->activeNS_model->clear();
        break;
    case ModelType::activeDNS_MX:
        m_projectModel->activeMX_model->clear();
        break;
    case ModelType::activeDNS_TXT:
        m_projectModel->activeTXT_model->clear();
        break;
    case ModelType::activeDNS_CNAME:
        m_projectModel->activeCNAME_model->clear();
        break;
    case ModelType::activeSSL:
        m_projectModel->activeSSL_model->clear();
        break;
    case ModelType::activeSSL_sha1:
        m_projectModel->activeSSL_sha1_model->clear();
        break;
    case ModelType::activeSSL_sha256:
        m_projectModel->activeSSL_sha256_model->clear();
        break;
    case ModelType::passive_subdomainIp:
        m_projectModel->passiveSubdomainIp_model->clear();
        break;
    case ModelType::passive_subdomain:
        m_projectModel->passiveSubdomain_model->clear();
        break;
    case ModelType::passive_A:
        m_projectModel->passiveA_model->clear();
        break;
    case ModelType::passive_AAAA:
        m_projectModel->passiveAAAA_model->clear();
        break;
    case ModelType::passive_NS:
        m_projectModel->passiveNS_model->clear();
        break;
    case ModelType::passive_MX:
        m_projectModel->passiveMX_model->clear();
        break;
    case ModelType::passive_TXT:
        m_projectModel->passiveTXT_model->clear();
        break;
    case ModelType::passive_CNAME:
        m_projectModel->passiveCNAME_model->clear();
        break;
    case ModelType::passive_Email:
        m_projectModel->passiveEmail_model->clear();
        break;
    case ModelType::passive_URL:
        m_projectModel->passiveUrl_model->clear();
        break;
    case ModelType::passive_ASN:
        m_projectModel->passiveAsn_model->clear();
        break;
    case ModelType::passive_SSL:
        m_projectModel->passiveSSL_model->clear();
        break;
    case ModelType::enum_IP:
        m_projectModel->enumIp_model->clear();
        break;
    case ModelType::enum_ASN:
        m_projectModel->enumASN_model->clear();
        break;
    case ModelType::enum_CIDR:
        m_projectModel->enumCIDR_model->clear();
        break;
    case ModelType::enum_NS:
        m_projectModel->enumNS_model->clear();
        break;
    case ModelType::enum_MX:
        m_projectModel->enumMX_model->clear();
        break;
    case ModelType::enum_Email:
        m_projectModel->enumEmail_model->clear();
        break;
    case ModelType::enum_SSL:
        m_projectModel->enumSSL_model->clear();
        break;
    case ModelType::enum_URL:
        m_projectModel->enumURL_model->clear();
        break;
    }

    m_projectModel->setHeaderLabels();
    m_projectModel->getRootItems();
    ui->labelCount->setNum(m_proxyModel->rowCount());
}

void Project::action_save(){

}

void Project::action_copy(){

}

void Project::action_send(){

}

#include "Project.h"
#include "ui_Project.h"


void Project::on_treeViewProjectExplorer_clicked(const QModelIndex &index){
    QModelIndex parent_index = index.parent();

    ui->comboBoxFilter->clear();
    ui->comboBoxFilter->hide();

    ui->treeViewSiteMap->setIndentation(0);
    ui->treeViewSiteMap->setSortingEnabled(true);

    if(parent_index == m_projectModel->project_explorer->index())
        return;

    if(parent_index == m_projectModel->active_explorer->index()){
        if(index == m_projectModel->activeHost_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeHost_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeHost);
            ui->comboBoxFilter->addItems({"Hostname", "IpV4", "IpV6", "Ports"});
            ui->comboBoxFilter->show();

            ui->treeViewSiteMap->header()->resizeSection(0, 200);
            ui->treeViewSiteMap->header()->resizeSection(1, 100);
            ui->treeViewSiteMap->header()->resizeSection(2, 150);
            ui->treeViewSiteMap->header()->resizeSection(3, 100);
        }
        if(index == m_projectModel->activeDNS_explorer->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 200);
            ui->treeViewSiteMap->header()->resizeSection(1, 200);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            m_proxyModel->setSourceModel(m_projectModel->activeDNS_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeDNS);
        }
        if(index == m_projectModel->activeWildcard_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeWildcard_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeWildcard);
            ui->comboBoxFilter->addItems({"Wildcard", "IpV4", "IpV6"});
            ui->comboBoxFilter->show();

            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->header()->resizeSection(1, 150);
        }
        if(index == m_projectModel->activeSSL_explorer->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            m_proxyModel->setSourceModel(m_projectModel->activeSSL_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeSSL);
            ui->comboBoxFilter->addItems({"SSL", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == m_projectModel->activeURL_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeURL_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeURL);
            ui->comboBoxFilter->addItems({"URL", "Value"});
            ui->comboBoxFilter->show();

            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->header()->resizeSection(1, 10);
            ui->treeViewSiteMap->header()->resizeSection(2, 120);
        }
    }

    if(parent_index == m_projectModel->activeDNS_explorer->index()){
        if(index == m_projectModel->activeA_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeA_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeDNS_A);
        }
        if(index == m_projectModel->activeAAAA_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeAAAA_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeDNS_AAAA);
        }
        if(index == m_projectModel->activeNS_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeNS_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeDNS_NS);
        }
        if(index == m_projectModel->activeMX_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeMX_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeDNS_MX);
        }
        if(index == m_projectModel->activeTXT_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeTXT_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeDNS_TXT);
        }
        if(index == m_projectModel->activeCNAME_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeCNAME_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeDNS_CNAME);
        }
        if(index == m_projectModel->activeSRV_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeSRV_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeDNS_SRV);
            ui->comboBoxFilter->addItems({"Name", "Target", "Port"});
            ui->comboBoxFilter->show();

            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->header()->resizeSection(1, 300);
        }
    }

    if(parent_index == m_projectModel->activeSSL_explorer->index()){
        if(index == m_projectModel->activeSSL_sha1_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeSSL_sha1_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeSSL_sha1);
        }
        if(index == m_projectModel->activeSSL_sha256_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeSSL_sha256_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeSSL_sha256);
        }
        if(index == m_projectModel->activeSSL_altNames_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->activeSSL_altNames_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::activeSSL_altNames);
        }
    }

    if(parent_index == m_projectModel->passive_explorer->index()){
        if(index == m_projectModel->passiveSubdomainIp_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveSubdomainIp_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_subdomainIp);
            ui->comboBoxFilter->addItems({"Subdomain", "IpAddress"});
            ui->comboBoxFilter->show();

            ui->treeViewSiteMap->header()->resizeSection(0, 300);
        }
        if(index == m_projectModel->passiveSubdomain_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveSubdomain_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_subdomain);
        }
        if(index == m_projectModel->passiveA_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveA_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_A);
        }
        if(index == m_projectModel->passiveAAAA_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveAAAA_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_AAAA);
        }
        if(index == m_projectModel->passiveCidr_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveCidr_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_CIDR);
        }
        if(index == m_projectModel->passiveNS_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveNS_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_NS);
        }
        if(index == m_projectModel->passiveMX_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveMX_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_MX);
        }
        if(index == m_projectModel->passiveTXT_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveTXT_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_TXT);
        }
        if(index == m_projectModel->passiveCNAME_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveCNAME_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_CNAME);
        }
        if(index == m_projectModel->passiveEmail_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveEmail_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_Email);
        }
        if(index == m_projectModel->passiveUrl_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveUrl_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_URL);
        }
        if(index == m_projectModel->passiveAsn_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveAsn_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_ASN);
            ui->comboBoxFilter->addItems({"ASN", "name"});
            ui->comboBoxFilter->show();

            ui->treeViewSiteMap->header()->resizeSection(0, 300);
        }
        if(index == m_projectModel->passiveSSL_explorer->index()){
            m_proxyModel->setSourceModel(m_projectModel->passiveSSL_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::passive_SSL);
        }
    }

    if(parent_index == m_projectModel->enums_explorer->index()){
        if(index == m_projectModel->enumIp_explorer->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            m_proxyModel->setSourceModel(m_projectModel->enumIp_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::enum_IP);
            ui->comboBoxFilter->addItems({"IP", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == m_projectModel->enumMX_explorer->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            m_proxyModel->setSourceModel(m_projectModel->enumMX_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::enum_MX);
            ui->comboBoxFilter->addItems({"MX", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == m_projectModel->enumASN_explorer->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            m_proxyModel->setSourceModel(m_projectModel->enumASN_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::enum_ASN);
            ui->comboBoxFilter->addItems({"ASN", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == m_projectModel->enumCIDR_explorer->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            m_proxyModel->setSourceModel(m_projectModel->enumCIDR_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::enum_CIDR);
            ui->comboBoxFilter->addItems({"CIDR", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == m_projectModel->enumNS_explorer->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            m_proxyModel->setSourceModel(m_projectModel->enumNS_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::enum_NS);
            ui->comboBoxFilter->addItems({"NS", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == m_projectModel->enumSSL_explorer->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            m_proxyModel->setSourceModel(m_projectModel->enumSSL_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::enum_SSL);
            ui->comboBoxFilter->addItems({"SSL", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == m_projectModel->enumEmail_explorer->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            m_proxyModel->setSourceModel(m_projectModel->enumEmail_model);
            ui->treeViewSiteMap->setProperty(PROJECT_MODEL_TYPE, ModelType::enum_Email);
            ui->comboBoxFilter->addItems({"Email", "Value"});
            ui->comboBoxFilter->show();
        }
    }

    ui->comboBoxFilter->setCurrentIndex(0);
    ui->labelCount->setNum(m_proxyModel->rowCount());
}

void Project::on_lineEditFilter_textChanged(const QString &filterKeyword){
    m_proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        m_proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        m_proxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewSiteMap->setModel(m_proxyModel);
    ui->labelCount->setNum(m_proxyModel->rowCount());
}

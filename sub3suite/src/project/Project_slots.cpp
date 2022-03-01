#include "Project.h"
#include "ui_Project.h"


void Project::on_treeViewProjectExplorer_clicked(const QModelIndex &index){
    QModelIndex parent_index = index.parent();

    ui->comboBoxFilter->clear();
    ui->comboBoxFilter->hide();

    ui->treeViewSiteMap->setIndentation(0);
    ui->treeViewSiteMap->setSortingEnabled(true);

    if(parent_index == model->explorer->project->index())
        return;

    if(parent_index == model->explorer->active->index()){
        if(index == model->explorer->activeHost->index()){
            proxyModel->setSourceModel(model->activeHost);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeHost);
            ui->comboBoxFilter->addItems({"Hostname", "IpV4", "IpV6", "Ports"});
            ui->comboBoxFilter->show();

            ui->treeViewSiteMap->header()->resizeSection(0, 200);
            ui->treeViewSiteMap->header()->resizeSection(1, 100);
            ui->treeViewSiteMap->header()->resizeSection(2, 150);
            ui->treeViewSiteMap->header()->resizeSection(3, 100);
        }
        if(index == model->explorer->activeDNS->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 200);
            ui->treeViewSiteMap->header()->resizeSection(1, 200);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            proxyModel->setSourceModel(model->activeDNS);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeDNS);
        }
        if(index == model->explorer->activeWildcard->index()){
            proxyModel->setSourceModel(model->activeWildcard);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeWildcard);
            ui->comboBoxFilter->addItems({"Wildcard", "IpV4", "IpV6"});
            ui->comboBoxFilter->show();

            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->header()->resizeSection(1, 150);
        }
        if(index == model->explorer->activeSSL->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            proxyModel->setSourceModel(model->activeSSL);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeSSL);
            ui->comboBoxFilter->addItems({"SSL", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == model->explorer->activeURL->index()){
            proxyModel->setSourceModel(model->activeURL);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeURL);
            ui->comboBoxFilter->addItems({"URL", "Value"});
            ui->comboBoxFilter->show();

            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->header()->resizeSection(1, 10);
            ui->treeViewSiteMap->header()->resizeSection(2, 120);
        }
    }

    if(parent_index == model->explorer->activeDNS->index()){
        if(index == model->explorer->activeA->index()){
            proxyModel->setSourceModel(model->activeA);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeDNS_A);
        }
        if(index == model->explorer->activeAAAA->index()){
            proxyModel->setSourceModel(model->activeAAAA);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeDNS_AAAA);
        }
        if(index == model->explorer->activeNS->index()){
            proxyModel->setSourceModel(model->activeNS);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeDNS_NS);
        }
        if(index == model->explorer->activeMX->index()){
            proxyModel->setSourceModel(model->activeMX);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeDNS_MX);
        }
        if(index == model->explorer->activeTXT->index()){
            proxyModel->setSourceModel(model->activeTXT);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeDNS_TXT);
        }
        if(index == model->explorer->activeCNAME->index()){
            proxyModel->setSourceModel(model->activeCNAME);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeDNS_CNAME);
        }
        if(index == model->explorer->activeSRV->index()){
            proxyModel->setSourceModel(model->activeSRV);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeDNS_SRV);
            ui->comboBoxFilter->addItems({"Name", "Target", "Port"});
            ui->comboBoxFilter->show();

            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->header()->resizeSection(1, 300);
        }
    }

    if(parent_index == model->explorer->activeSSL->index()){
        if(index == model->explorer->activeSSL_sha1->index()){
            proxyModel->setSourceModel(model->activeSSL_sha1);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeSSL_sha1);
        }
        if(index == model->explorer->activeSSL_sha256->index()){
            proxyModel->setSourceModel(model->activeSSL_sha256);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeSSL_sha256);
        }
        if(index == model->explorer->activeSSL_altNames->index()){
            proxyModel->setSourceModel(model->activeSSL_altNames);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::activeSSL_altNames);
        }
    }

    if(parent_index == model->explorer->passive->index()){
        if(index == model->explorer->passiveSubdomainIp->index()){
            proxyModel->setSourceModel(model->passiveSubdomainIp);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_subdomainIp);
            ui->comboBoxFilter->addItems({"Subdomain", "IpAddress"});
            ui->comboBoxFilter->show();

            ui->treeViewSiteMap->header()->resizeSection(0, 300);
        }
        if(index == model->explorer->passiveSubdomain->index()){
            proxyModel->setSourceModel(model->passiveSubdomain);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_subdomain);
        }
        if(index == model->explorer->passiveA->index()){
            proxyModel->setSourceModel(model->passiveA);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_A);
        }
        if(index == model->explorer->passiveAAAA->index()){
            proxyModel->setSourceModel(model->passiveAAAA);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_AAAA);
        }
        if(index == model->explorer->passiveCidr->index()){
            proxyModel->setSourceModel(model->passiveCidr);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_CIDR);
        }
        if(index == model->explorer->passiveNS->index()){
            proxyModel->setSourceModel(model->passiveNS);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_NS);
        }
        if(index == model->explorer->passiveMX->index()){
            proxyModel->setSourceModel(model->passiveMX);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_MX);
        }
        if(index == model->explorer->passiveTXT->index()){
            proxyModel->setSourceModel(model->passiveTXT);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_TXT);
        }
        if(index == model->explorer->passiveCNAME->index()){
            proxyModel->setSourceModel(model->passiveCNAME);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_CNAME);
        }
        if(index == model->explorer->passiveEmail->index()){
            proxyModel->setSourceModel(model->passiveEmail);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_Email);
        }
        if(index == model->explorer->passiveUrl->index()){
            proxyModel->setSourceModel(model->passiveUrl);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_URL);
        }
        if(index == model->explorer->passiveAsn->index()){
            proxyModel->setSourceModel(model->passiveAsn);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_ASN);
            ui->comboBoxFilter->addItems({"ASN", "name"});
            ui->comboBoxFilter->show();

            ui->treeViewSiteMap->header()->resizeSection(0, 300);
        }
        if(index == model->explorer->passiveSSL->index()){
            proxyModel->setSourceModel(model->passiveSSL);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::passive_SSL);
        }
    }

    if(parent_index == model->explorer->enums->index()){
        if(index == model->explorer->enumIp->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            proxyModel->setSourceModel(model->enumIp);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::enum_IP);
            ui->comboBoxFilter->addItems({"IP", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == model->explorer->enumMX->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            proxyModel->setSourceModel(model->enumMX);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::enum_MX);
            ui->comboBoxFilter->addItems({"MX", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == model->explorer->enumASN->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            proxyModel->setSourceModel(model->enumASN);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::enum_ASN);
            ui->comboBoxFilter->addItems({"ASN", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == model->explorer->enumCIDR->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            proxyModel->setSourceModel(model->enumCIDR);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::enum_CIDR);
            ui->comboBoxFilter->addItems({"CIDR", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == model->explorer->enumNS->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            proxyModel->setSourceModel(model->enumNS);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::enum_NS);
            ui->comboBoxFilter->addItems({"NS", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == model->explorer->enumSSL->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            proxyModel->setSourceModel(model->enumSSL);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::enum_SSL);
            ui->comboBoxFilter->addItems({"SSL", "Value"});
            ui->comboBoxFilter->show();
        }
        if(index == model->explorer->enumEmail->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            proxyModel->setSourceModel(model->enumEmail);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::enum_Email);
            ui->comboBoxFilter->addItems({"Email", "Value"});
            ui->comboBoxFilter->show();
        }
    }

    if(parent_index == model->explorer->custom->index()){
        if(index == model->explorer->raw->index()){
            ui->treeViewSiteMap->header()->resizeSection(0, 300);
            ui->treeViewSiteMap->setIndentation(15);
            ui->treeViewSiteMap->setSortingEnabled(false);

            proxyModel->setSourceModel(model->raw);
            ui->treeViewSiteMap->setProperty(SITEMAP_TYPE, ExplorerType::raw);
            ui->comboBoxFilter->addItems({"Property", "Value"});
            ui->comboBoxFilter->show();
        }
    }

    ui->comboBoxFilter->setCurrentIndex(0);
    ui->labelCount->setNum(proxyModel->rowCount());
}

void Project::on_lineEditFilter_textChanged(const QString &filterKeyword){
    proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewSiteMap->setModel(proxyModel);
    ui->labelCount->setNum(proxyModel->rowCount());
}

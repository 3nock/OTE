#include "Project.h"
#include "ui_Project.h"


Project::Project(QWidget *parent, ProjectModel *projectModel) :QWidget(parent),
    ui(new Ui::Project),
    m_projectModel(projectModel),
    m_siteMapProxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    ui->treeViewProjectExplorer->setModel(m_projectModel->projectExplorerModel);

    /* placeholder texts */
    ui->lineEditSiteMapFilter->setPlaceholderText("Filter...");

    /* underdevelopment txt */
    ui->labelGraph->setDisabled(true);
    ui->labelAnalysis->setDisabled(true);
}
Project::~Project(){
    delete m_siteMapProxyModel;
    delete ui;
}

/* changing the model */
void Project::on_treeViewProjectExplorer_clicked(const QModelIndex &index){
    if(index.parent() == m_projectModel->explorerRootItem->index())
        return;

    QString type(index.data().toString());
    QString engine(index.parent().data().toString());

    if(engine == "Active"){
        if(type == "Subdomain Ip")
            ui->treeViewSiteMap->setModel(m_projectModel->activeSubdomainIp_model);
        if(type == "Subdomain")
            ui->treeViewSiteMap->setModel(m_projectModel->activeSubdomain_model);
        if(type == "TLD")
            ui->treeViewSiteMap->setModel(m_projectModel->activeTld_model);
        if(type == "DNS")
            ui->treeViewSiteMap->setModel(m_projectModel->activeDNS_model);
        if(type == "Widlcard")
            ui->treeViewSiteMap->setModel(m_projectModel->activeWildcard_model);
        if(type == "SSL")
            ui->treeViewSiteMap->setModel(m_projectModel->activeSSL_model);
    }

    if(engine == "DNS"){
        if(type == "A")
            ui->treeViewSiteMap->setModel(m_projectModel->activeA_model);
        if(type == "AAAA")
            ui->treeViewSiteMap->setModel(m_projectModel->activeAAAA_model);
        if(type == "NS")
            ui->treeViewSiteMap->setModel(m_projectModel->activeNS_model);
        if(type == "MX")
            ui->treeViewSiteMap->setModel(m_projectModel->activeMX_model);
        if(type == "TXT")
            ui->treeViewSiteMap->setModel(m_projectModel->activeTXT_model);
        if(type == "CNAME")
            ui->treeViewSiteMap->setModel(m_projectModel->activeCNAME_model);
        if(type == "SRV")
            ui->treeViewSiteMap->setModel(m_projectModel->activeSRV_model);
    }

    if(engine == "SSL"){
        if(type == "SHA-1")
            ui->treeViewSiteMap->setModel(m_projectModel->activeSSL_sha1_model);
        if(type == "SHA-256")
            ui->treeViewSiteMap->setModel(m_projectModel->activeSSL_sha256_model);
    }

    if(engine == "Passive"){
        if(type == "Subdomain Ip")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveSubdomainIp_model);
        if(type == "Subdomain")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveSubdomain_model);
        if(type == "A")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveA_model);
        if(type == "AAAA")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveAAAA_model);
        if(type == "CIDR")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveCidr_model);
        if(type == "NS")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveNS_model);
        if(type == "MX")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveMX_model);
        if(type == "TXT")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveTXT_model);
        if(type == "CNAME")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveCNAME_model);
        if(type == "EMAIL")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveEmail_model);
        if(type == "URL")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveUrl_model);
        if(type == "ASN")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveAsn_model);
        if(type == "SSL")
            ui->treeViewSiteMap->setModel(m_projectModel->passiveSSL_model);
    }

    if(engine == "Enum"){
        if(type == "IP")
            ui->treeViewSiteMap->setModel(m_projectModel->enumIp_model);
        if(type == "MX")
            ui->treeViewSiteMap->setModel(m_projectModel->enumMX_model);
        if(type == "Domain")
            ui->treeViewSiteMap->setModel(m_projectModel->enumDomain_model);
        if(type == "ASN")
            ui->treeViewSiteMap->setModel(m_projectModel->enumASN_model);
        if(type == "CIDR")
            ui->treeViewSiteMap->setModel(m_projectModel->enumCIDR_model);
        if(type == "NS")
            ui->treeViewSiteMap->setModel(m_projectModel->enumNS_model);
        if(type == "SSL")
            ui->treeViewSiteMap->setModel(m_projectModel->enumSSL_model);
        if(type == "Email")
            ui->treeViewSiteMap->setModel(m_projectModel->enumEmail_model);
        if(type == "URL")
            ui->treeViewSiteMap->setModel(m_projectModel->enumURL_model);
    }
}

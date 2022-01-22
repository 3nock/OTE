#include "Project.h"
#include "ui_Project.h"

#include <QFile>


Project::Project(QWidget *parent, ProjectModel *projectModel) :QWidget(parent),
    ui(new Ui::Project),
    m_projectModel(projectModel),
    m_proxyModel(new QSortFilterProxyModel)
{
    this->m_initUI();

    /* underdevelopment txt */
    QFile file(":/files/res/files/under_development.html");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QByteArray data = file.readAll();
        ui->textBrowserGraph->append(data);
        ui->textBrowserAnalysis->append(data);
        file.close();
    }

    /* data models */
    ui->treeViewProjectExplorer->setModel(m_projectModel->projectExplorerModel);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setRecursiveFilteringEnabled(true);
    m_proxyModel->setFilterKeyColumn(0);
    ui->treeViewSiteMap->setModel(m_proxyModel);
}
Project::~Project(){
    this->m_saveProject();
    delete m_proxyModel;
    delete ui;
}

void Project::initProject(QString projectFile){
    m_projectFile = projectFile;

    if(m_projectFile != "Temp")
        this->m_openProject();

    this->m_setupProjetct();
}

void Project::m_openProject(){

}

void Project::m_saveProject(){

}

void Project::m_closeProject(){
    this->m_saveProject();
}

void Project::m_setupProjetct(){

}

/* changing the model */
void Project::on_treeViewProjectExplorer_clicked(const QModelIndex &index){
    if(index.parent() == m_projectModel->explorerRootItem->index())
        return;

    QString type(index.data().toString());
    QString engine(index.parent().data().toString());

    if(engine == "Active"){
        if(type == "Subdomain Ip"){
            m_proxyModel->setSourceModel(m_projectModel->activeSubdomainIp_model);
            ui->labelType->setText("subdomain&Ip");
        }
        if(type == "Subdomain"){
            m_proxyModel->setSourceModel(m_projectModel->activeSubdomain_model);
            ui->labelType->setText("Subdomain");
        }
        if(type == "TLD"){
            m_proxyModel->setSourceModel(m_projectModel->activeTld_model);
            ui->labelType->setText("TLD");
        }
        if(type == "DNS"){
            m_proxyModel->setSourceModel(m_projectModel->activeDNS_model);
            ui->labelType->setText("DNS");
        }
        if(type == "Wildcard"){
            m_proxyModel->setSourceModel(m_projectModel->activeWildcard_model);
            ui->labelType->setText("Widlcard");
        }
        if(type == "SSL"){
            m_proxyModel->setSourceModel(m_projectModel->activeSSL_model);
            ui->labelType->setText("SSL");
        }
    }

    if(engine == "DNS"){
        if(type == "A"){
            m_proxyModel->setSourceModel(m_projectModel->activeA_model);
            ui->labelType->setText("A");
        }
        if(type == "AAAA"){
            m_proxyModel->setSourceModel(m_projectModel->activeAAAA_model);
            ui->labelType->setText("AAAA");
        }
        if(type == "NS"){
            m_proxyModel->setSourceModel(m_projectModel->activeNS_model);
            ui->labelType->setText("NS");
        }
        if(type == "MX"){
            m_proxyModel->setSourceModel(m_projectModel->activeMX_model);
            ui->labelType->setText("MX");
        }
        if(type == "TXT"){
            m_proxyModel->setSourceModel(m_projectModel->activeTXT_model);
            ui->labelType->setText("TXT");
        }
        if(type == "CNAME"){
            m_proxyModel->setSourceModel(m_projectModel->activeCNAME_model);
            ui->labelType->setText("CNAME");
        }
        if(type == "SRV"){
            m_proxyModel->setSourceModel(m_projectModel->activeSRV_model);
            ui->labelType->setText("SRV");
        }
    }

    if(engine == "SSL"){
        if(type == "SHA-1"){
            m_proxyModel->setSourceModel(m_projectModel->activeSSL_sha1_model);
            ui->labelType->setText("SHA-1");
        }
        if(type == "SHA-256"){
            m_proxyModel->setSourceModel(m_projectModel->activeSSL_sha256_model);
            ui->labelType->setText("SHA-2");
        }
    }

    if(engine == "Passive"){
        if(type == "Subdomain Ip"){
            m_proxyModel->setSourceModel(m_projectModel->passiveSubdomainIp_model);
            ui->labelType->setText("Subdomain&Ip");
        }
        if(type == "Subdomain"){
            m_proxyModel->setSourceModel(m_projectModel->passiveSubdomain_model);
            ui->labelType->setText("Subdomain");
        }
        if(type == "A"){
            m_proxyModel->setSourceModel(m_projectModel->passiveA_model);
            ui->labelType->setText("A");
        }
        if(type == "AAAA"){
            m_proxyModel->setSourceModel(m_projectModel->passiveAAAA_model);
            ui->labelType->setText("AAAA");
        }
        if(type == "CIDR"){
            m_proxyModel->setSourceModel(m_projectModel->passiveCidr_model);
            ui->labelType->setText("CIDR");
        }
        if(type == "NS"){
            m_proxyModel->setSourceModel(m_projectModel->passiveNS_model);
            ui->labelType->setText("NS");
        }
        if(type == "MX"){
            m_proxyModel->setSourceModel(m_projectModel->passiveMX_model);
            ui->labelType->setText("MX");
        }
        if(type == "TXT"){
            m_proxyModel->setSourceModel(m_projectModel->passiveTXT_model);
            ui->labelType->setText("TXT");
        }
        if(type == "CNAME"){
            m_proxyModel->setSourceModel(m_projectModel->passiveCNAME_model);
            ui->labelType->setText("CNAME");
        }
        if(type == "EMAIL"){
            m_proxyModel->setSourceModel(m_projectModel->passiveEmail_model);
            ui->labelType->setText("EMAIL");
        }
        if(type == "URL"){
            m_proxyModel->setSourceModel(m_projectModel->passiveUrl_model);
            ui->labelType->setText("URL");
        }
        if(type == "ASN"){
            m_proxyModel->setSourceModel(m_projectModel->passiveAsn_model);
            ui->labelType->setText("ASN");
        }
        if(type == "SSL"){
            m_proxyModel->setSourceModel(m_projectModel->passiveSSL_model);
            ui->labelType->setText("SSL");
        }
    }

    if(engine == "Enum"){
        if(type == "IP"){
            m_proxyModel->setSourceModel(m_projectModel->enumIp_model);
            ui->labelType->setText("IP");
        }
        if(type == "MX"){
            m_proxyModel->setSourceModel(m_projectModel->enumMX_model);
            ui->labelType->setText("MX");
        }
        if(type == "ASN"){
            m_proxyModel->setSourceModel(m_projectModel->enumASN_model);
            ui->labelType->setText("ASN");
        }
        if(type == "CIDR"){
            m_proxyModel->setSourceModel(m_projectModel->enumCIDR_model);
            ui->labelType->setText("CIDR");
        }
        if(type == "NS"){
            m_proxyModel->setSourceModel(m_projectModel->enumNS_model);
            ui->labelType->setText("NS");
        }
        if(type == "SSL"){
            m_proxyModel->setSourceModel(m_projectModel->enumSSL_model);
            ui->labelType->setText("SSL");
        }
        if(type == "Email"){
            m_proxyModel->setSourceModel(m_projectModel->enumEmail_model);
            ui->labelType->setText("Email");
        }
        if(type == "URL"){
            m_proxyModel->setSourceModel(m_projectModel->enumURL_model);
            ui->labelType->setText("URL");
        }
    }

    ui->comboBoxFilter->setCurrentIndex(0);
    ui->labelOriginalCount->setNum(m_proxyModel->sourceModel()->rowCount());
    ui->labelFilterCount->setNum(m_proxyModel->rowCount());
}

void Project::m_initUI(){
    /* setup ui */
    ui->setupUi(this);

    /* setting widgets special properties for diff stylesheets */
    ui->labelType->setProperty("default_color", true);
    ui->labelFilterCount->setProperty("default_color", true);
    ui->labelOriginalCount->setProperty("default_color", true);

    /* hiding un-used widgets */
    ui->comboBoxFilter->hide();

    /* placeholder texts */
    ui->lineEditFilter->setPlaceholderText("Filter...");

    /* resizing */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.23))
                                        << static_cast<int>((this->width() * 0.77)));

    ui->splitter_2->setSizes(QList<int>() << static_cast<int>((this->width() * 0.80))
                                               << static_cast<int>((this->width() * 0.20)));
}

void Project::on_lineEditFilter_textChanged(const QString &filterKeyword){
    m_proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        m_proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        m_proxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewSiteMap->setModel(m_proxyModel);
    ui->labelFilterCount->setNum(m_proxyModel->rowCount());
}

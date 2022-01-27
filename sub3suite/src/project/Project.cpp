#include "Project.h"
#include "ui_Project.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


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
    ui->treeViewProjectExplorer->setModel(m_projectModel->model_explorer);
    ui->treeViewProjectExplorer->expand(m_projectModel->project_explorer->index());

    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_proxyModel->setRecursiveFilteringEnabled(true);
    m_proxyModel->setFilterKeyColumn(0);
    ui->treeViewSiteMap->setModel(m_proxyModel);

    this->init_notesMenuBar();
    this->init_sitemapMenuBar();
}
Project::~Project(){
    delete m_proxyModel;
    delete ui;
}

/* changing the model */
/* compare indexes instead of strings */
void Project::on_treeViewProjectExplorer_clicked(const QModelIndex &index){
    if(index.parent() == m_projectModel->project_explorer->index())
        return;

    QString type(index.data().toString());
    QString engine(index.parent().data().toString());

    if(engine == "Active"){
        if(type == "Subdomain Ip"){
            m_proxyModel->setSourceModel(m_projectModel->activeSubdomainIp_model);
        }
        if(type == "Subdomain"){
            m_proxyModel->setSourceModel(m_projectModel->activeSubdomain_model);
        }
        if(type == "TLD"){
            m_proxyModel->setSourceModel(m_projectModel->activeTld_model);
        }
        if(type == "DNS"){
            m_proxyModel->setSourceModel(m_projectModel->activeDNS_model);
        }
        if(type == "Wildcard"){
            m_proxyModel->setSourceModel(m_projectModel->activeWildcard_model);
        }
        if(type == "SSL"){
            m_proxyModel->setSourceModel(m_projectModel->activeSSL_model);
        }
    }

    if(engine == "DNS"){
        if(type == "A"){
            m_proxyModel->setSourceModel(m_projectModel->activeA_model);
        }
        if(type == "AAAA"){
            m_proxyModel->setSourceModel(m_projectModel->activeAAAA_model);
        }
        if(type == "NS"){
            m_proxyModel->setSourceModel(m_projectModel->activeNS_model);
        }
        if(type == "MX"){
            m_proxyModel->setSourceModel(m_projectModel->activeMX_model);
        }
        if(type == "TXT"){
            m_proxyModel->setSourceModel(m_projectModel->activeTXT_model);
        }
        if(type == "CNAME"){
            m_proxyModel->setSourceModel(m_projectModel->activeCNAME_model);
        }
        if(type == "SRV"){
            m_proxyModel->setSourceModel(m_projectModel->activeSRV_model);
        }
    }

    if(engine == "SSL"){
        if(type == "SHA-1"){
            m_proxyModel->setSourceModel(m_projectModel->activeSSL_sha1_model);
        }
        if(type == "SHA-256"){
            m_proxyModel->setSourceModel(m_projectModel->activeSSL_sha256_model);
        }
    }

    if(engine == "Passive"){
        if(type == "Subdomain Ip"){
            m_proxyModel->setSourceModel(m_projectModel->passiveSubdomainIp_model);
        }
        if(type == "Subdomain"){
            m_proxyModel->setSourceModel(m_projectModel->passiveSubdomain_model);
        }
        if(type == "A"){
            m_proxyModel->setSourceModel(m_projectModel->passiveA_model);
        }
        if(type == "AAAA"){
            m_proxyModel->setSourceModel(m_projectModel->passiveAAAA_model);
        }
        if(type == "CIDR"){
            m_proxyModel->setSourceModel(m_projectModel->passiveCidr_model);
        }
        if(type == "NS"){
            m_proxyModel->setSourceModel(m_projectModel->passiveNS_model);
        }
        if(type == "MX"){
            m_proxyModel->setSourceModel(m_projectModel->passiveMX_model);
        }
        if(type == "TXT"){
            m_proxyModel->setSourceModel(m_projectModel->passiveTXT_model);
        }
        if(type == "CNAME"){
            m_proxyModel->setSourceModel(m_projectModel->passiveCNAME_model);
        }
        if(type == "EMAIL"){
            m_proxyModel->setSourceModel(m_projectModel->passiveEmail_model);
        }
        if(type == "URL"){
            m_proxyModel->setSourceModel(m_projectModel->passiveUrl_model);
        }
        if(type == "ASN"){
            m_proxyModel->setSourceModel(m_projectModel->passiveAsn_model);
        }
        if(type == "SSL"){
            m_proxyModel->setSourceModel(m_projectModel->passiveSSL_model);
        }
    }

    if(engine == "Enum"){
        if(type == "IP"){
            m_proxyModel->setSourceModel(m_projectModel->enumIp_model);
        }
        if(type == "MX"){
            m_proxyModel->setSourceModel(m_projectModel->enumMX_model);
        }
        if(type == "ASN"){
            m_proxyModel->setSourceModel(m_projectModel->enumASN_model);
        }
        if(type == "CIDR"){
            m_proxyModel->setSourceModel(m_projectModel->enumCIDR_model);
        }
        if(type == "NS"){
            m_proxyModel->setSourceModel(m_projectModel->enumNS_model);
        }
        if(type == "SSL"){
            m_proxyModel->setSourceModel(m_projectModel->enumSSL_model);
        }
        if(type == "Email"){
            m_proxyModel->setSourceModel(m_projectModel->enumEmail_model);
        }
        if(type == "URL"){
            m_proxyModel->setSourceModel(m_projectModel->enumURL_model);
        }
    }

    ui->comboBoxFilter->setCurrentIndex(0);
    ui->labelFilterCount->setNum(m_proxyModel->rowCount());
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

void Project::m_initUI(){
    /* setup ui */
    ui->setupUi(this);

    /* setting widgets special properties for diff stylesheets */
    ui->labelFilterCount->setProperty("default_color", true);
    ui->treeViewProjectExplorer->setProperty("no_item_border", true);
    ui->tabWidget->setProperty("upside", true);

    /* hiding un-used widgets */
    ui->comboBoxFilter->hide();

    /* placeholder texts */
    ui->lineEditFilter->setPlaceholderText("Filter...");

    /* resizing */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.20))
                           << static_cast<int>((this->width() * 0.80)));
}

void Project::init_sitemapMenuBar(){
    m_sitemapMenuBar = new QMenuBar(this);
    m_sitemapMenuBar->addAction("Save As", this, [=](){});
    m_sitemapMenuBar->addAction("Clear", this, [=](){});
    m_sitemapMenuBar->addAction("Send To", this, [=](){});
    m_sitemapMenuBar->addAction("Expand", this, [=](){});
    m_sitemapMenuBar->addAction("Collapse", this, [=](){});
    ui->horizontalLayoutMap->insertWidget(0, m_sitemapMenuBar);
}

void Project::init_notesMenuBar(){
    m_notesMenuBar = new QMenuBar(this);
    m_notesMenuBar->addAction("Clear", this, [=](){ui->plainTextEdit->clear();});
    ui->horizontalLayoutNotes->insertWidget(0, m_notesMenuBar);
}

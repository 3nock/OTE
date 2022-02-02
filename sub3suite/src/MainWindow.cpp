/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QPushButton>
#include <QDesktopServices>

#include "src/dialogs/StartupDialog.h"
#include "src/dialogs/AboutDialog.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/LogViewerDialog.h"
#include "src/dialogs/PreferenceDialog.h"
#include "src/dialogs/DocumentationDialog.h"
#include "src/dialogs/ProjectConfigDialog.h"

#include "src/utils/Config.h"

/*
 * TODO:
 *      status from all engines, so as to alert during close event
 *      if some engines are still active
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* init */
    this->m_registerMetaTypes();
    this->m_initEngines();
    this->m_documentation();
    this->recents();
    this->initActions();

    /* build info */
    QAction* buildInfo = new QAction(tr("%1 (%2)").arg("Feb 01 2022").arg("BETA"), this);
    buildInfo->setEnabled(false);
    ui->menubar->addAction(buildInfo);

    /* welcome */
    ui->statusbar->showMessage("Welcome!", 5000);
}
MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    /* check if project is configured, if not prompt to save project */
    if(projectModel->projectInfo.isNew ||  projectModel->projectInfo.isTemporary){
        if(!projectModel->projectInfo.isConfigured)
        {
            /* configure project */
            ProjectConfigDialog projectConfigDialog(projectModel, this);
            projectConfigDialog.setModal(true);
            projectConfigDialog.exec();
        }
        /* then save the project */
        projectModel->saveProject();
    }
    if(projectModel->projectInfo.isExisting){
        /* save project */
        projectModel->closeProject();
    }

    /* check if any engine or enumerator is active and prompt to continue */
}

void MainWindow::initActions(){
    ui->menuTools->addAction("OSINT Engine", this, [=](){this->onChangeTabToOsint();});
    ui->menuTools->addAction("RAW Engine", this, [=](){this->onChangeTabToRaw();});
    ui->menuTools->addSeparator();
    ui->menuTools->addAction("BRUTE Engine", this, [=](){this->onChangeTabToBrute();});
    ui->menuTools->addAction("ACTIVE Engine", this, [=](){this->onChangeTabToActive();});
    ui->menuTools->addAction("DNS Engine", this, [=](){this->onChangeTabToDns();});
    ui->menuTools->addAction("SSL Engine", this, [=](){this->onChangeTabToSSL();});
    ui->menuTools->addSeparator();
    ui->menuTools->addAction("IP Enumerator", this, [=](){this->onChangeTabToIpEnum();});
    ui->menuTools->addAction("ASN Enumerator", this, [=](){this->onChangeTabToAsnEnum();});
    ui->menuTools->addAction("CIDR Enumerator", this, [=](){this->onChangeTabToCidrEnum();});
    ui->menuTools->addAction("NS Enumerator", this, [=](){this->onChangeTabToNSEnum();});
    ui->menuTools->addAction("MX Enumerator", this, [=](){this->onChangeTabToMXEnum();});
    ui->menuTools->addAction("SSL Enumerator", this, [=](){this->onChangeTabToSSLEnum();});
    ui->menuTools->addAction("URL Enumerator", this, [=](){this->onChangeTabToUrlEnum();});
    ui->menuTools->addAction("Email Enumerator", this, [=](){this->onChangeTabToEmailEnum();});
}

void MainWindow::recents(){
    m_menuRecents = new QMenu(this);

    CONFIG.beginGroup("recent_projects");
    QStringList recentList =  CONFIG.allKeys();


    if(recentList.count() < 1)
        return;

    /* append the projects to the recents menu */
    QString projectName;
    QString projectPath;
    for(int i = 0; i < recentList.count(); i++){
        projectName = recentList.at(i);
        projectPath = CONFIG.value(projectName).toString();
        QString name = projectName.remove("recent_projects/");
        m_menuRecents->addAction(name, this, [=](){projectModel->openExistingProject(name, projectPath);});
    }

    /* set the menu */
    ui->actionRecentProjects->setMenu(m_menuRecents);
    CONFIG.endGroup();
}

void MainWindow::initProject(ProjectStruct projectInfo){
    ui->statusbar->showMessage("Opening Project....", 6000);
    projectModel->openProject(projectInfo);
}

void MainWindow::m_registerMetaTypes(){
    qDebug() << "Registering meta types...";

    qRegisterMetaType<scan::Log>("scan::Log");
    qRegisterMetaType<dns::ScanResult>("dns::ScanResult");
    qRegisterMetaType<ScanLog>("ScanLog");
    qRegisterMetaType<CidrModelStruct>("CidrModelStruct");
    qRegisterMetaType<MXModelStruct>("MXModelStruct");
    qRegisterMetaType<CidrModelStruct>("CidrModelStruct");
    qRegisterMetaType<s3s_struct::ASN>("s3s_struct::ASN");
    qRegisterMetaType<QSslCertificate>("QSslCertificate");
}

void MainWindow::m_initEngines(){
    /* Project */
    projectModel = new ProjectModel;
    project = new Project(this, projectModel);

    /* Engines */
    osint = new Osint(this, projectModel);
    brute = new Brute(this, projectModel);
    active = new Active(this, projectModel);
    dns = new Dns(this, projectModel);
    ssl = new Ssl(this, projectModel);
    raw = new Raw(this, projectModel);

    /* Enumerators */
    ipEnum = new IpEnum(this, projectModel);
    asnEnum = new ASNEnum(this, projectModel);
    cidrEnum = new CidrEnum(this, projectModel);
    nsEnum = new NSEnum(this, projectModel);
    mxEnum = new MXEnum(this, projectModel);
    sslEnum = new SSLEnum(this, projectModel);
    emailEnum = new EmailEnum(this, projectModel);
    urlEnum = new UrlEnum(this, projectModel);

    /* connecting signals and slots */
    this->m_connectSignals(osint);
    this->m_connectSignals(brute);
    this->m_connectSignals(active);
    this->m_connectSignals(dns);
    this->m_connectSignals(ssl);
    this->m_connectSignals(raw);

    /* passive tabwidget */
    ui->tabWidgetPassive->insertTab(0, osint, "OSINT");
    ui->tabWidgetPassive->insertTab(1, raw, "RAW");
    ui->tabWidgetPassive->setCurrentIndex(0);
    /* active tabwidget */
    ui->tabWidgetActive->insertTab(0, brute, "BRUTE");
    ui->tabWidgetActive->insertTab(1, active, "ACTIVE");
    ui->tabWidgetActive->insertTab(2, dns, "DNS");
    ui->tabWidgetActive->insertTab(3, ssl, "SSL");
    ui->tabWidgetActive->setCurrentIndex(0);
    /* tools tabwidget */
    ui->tabWidgetEnums->insertTab(0, ipEnum, "IP");
    ui->tabWidgetEnums->insertTab(1, asnEnum, "ASN");
    ui->tabWidgetEnums->insertTab(2, cidrEnum, "CIDR");
    ui->tabWidgetEnums->insertTab(3, nsEnum, "NS");
    ui->tabWidgetEnums->insertTab(4, mxEnum, "MX");
    ui->tabWidgetEnums->insertTab(5, sslEnum, "SSL");
    ui->tabWidgetEnums->insertTab(6, emailEnum, "EMAIL");
    ui->tabWidgetEnums->insertTab(7, urlEnum, "URL");
    /* main tabwidget */
    ui->tabWidgetMain->insertTab(3, project, "Project");
    ui->tabWidgetMain->setCurrentIndex(0);
}

void MainWindow::m_connectSignals(AbstractEngine *engine){
    /* sending results */
    connect(engine, SIGNAL(sendResultsToOsint(QString, RESULT_TYPE)), osint, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToActive(QString, RESULT_TYPE)), active, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToBrute(QString, RESULT_TYPE)), brute, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToDns(QString, RESULT_TYPE)), dns, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToCert(QString, RESULT_TYPE)), ssl, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToRaw(QString, RESULT_TYPE)), raw, SLOT(onReceiveTargets(QString, RESULT_TYPE)));

    /* change tab to Engine */
    connect(engine, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(engine, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(engine, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(engine, SIGNAL(changeTabToDns()), this, SLOT(onChangeTabToDns()));
    connect(engine, SIGNAL(changeTabToRaw()), this, SLOT(onChangeTabToRaw()));
    connect(engine, SIGNAL(changeTabToSSL()), this, SLOT(onChangeTabToSSL()));
    /* change tab to Enumerator */
    connect(engine, SIGNAL(changeTabToIpEnum()), this, SLOT(onChangeTabToIpEnum()));
    connect(engine, SIGNAL(changeTabToAsnEnum()), this, SLOT(onChangeTabToAsnEnum()));
    connect(engine, SIGNAL(changeTabToCidrEnum()), this, SLOT(onChangeTabToCidrEnum()));
    connect(engine, SIGNAL(changeTabToNSEnum()), this, SLOT(onChangeTabToNSEnum()));
    connect(engine, SIGNAL(changeTabToMXEnum()), this, SLOT(onChangeTabToMXEnum()));
    connect(engine, SIGNAL(changeTabToSSLEnum()), this, SLOT(onChangeTabToSSLEnum()));
    connect(engine, SIGNAL(changeTabToEmailEnum()), this, SLOT(onChangeTabToEmailEnum()));
    connect(engine, SIGNAL(changeTabToUrlEnum()), this, SLOT(onChangeTabToUrlEnum()));
}

void MainWindow::m_documentation(){
    QWidget *cornerWidget_active = new QWidget(this);
    QWidget *cornerWidget_passive = new QWidget(this);
    QWidget *cornerWidget_enums = new QWidget(this);

    QHBoxLayout *hbox_active = new QHBoxLayout(cornerWidget_active);
    QHBoxLayout *hbox_passive = new QHBoxLayout(cornerWidget_passive);
    QHBoxLayout *hbox_enums = new QHBoxLayout(cornerWidget_enums);

    s3s_ClickableLabel *documentation_active = new s3s_ClickableLabel("", this);
    s3s_ClickableLabel *documentation_passive = new s3s_ClickableLabel("", this);
    s3s_ClickableLabel *documentation_enums = new s3s_ClickableLabel("", this);

    documentation_active->setPixmap(QPixmap(":/img/res/icons/help.png"));
    documentation_passive->setPixmap(QPixmap(":/img/res/icons/help.png"));
    documentation_enums->setPixmap(QPixmap(":/img/res/icons/help.png"));

    hbox_active->addWidget(documentation_active);
    hbox_passive->addWidget(documentation_passive);
    hbox_enums->addWidget(documentation_enums);

    hbox_active->setContentsMargins(0, 0, 10, 2);
    hbox_passive->setContentsMargins(0, 0, 10, 2);
    hbox_enums->setContentsMargins(0, 0, 10, 2);

    ui->tabWidgetPassive->setCornerWidget(cornerWidget_passive);
    ui->tabWidgetActive->setCornerWidget(cornerWidget_active);
    ui->tabWidgetEnums->setCornerWidget(cornerWidget_enums);

    connect(documentation_passive, &s3s_ClickableLabel::clicked, this, &MainWindow::onDocumentation_passive);
    connect(documentation_active, &s3s_ClickableLabel::clicked, this, &MainWindow::onDocumentation_active);
    connect(documentation_enums, &s3s_ClickableLabel::clicked, this, &MainWindow::onDocumentation_tools);
}

void MainWindow::onReceiveStatus(QString status){
    ui->statusbar->showMessage(status, 5000);
}


/* changing tabs */

void MainWindow::onChangeTabToOsint(){
    ui->tabWidgetMain->setCurrentIndex(0);
    ui->tabWidgetPassive->setCurrentIndex(0);
}

void MainWindow::onChangeTabToRaw(){
    ui->tabWidgetMain->setCurrentIndex(0);
    ui->tabWidgetPassive->setCurrentIndex(1);
}

void MainWindow::onChangeTabToBrute(){
    ui->tabWidgetMain->setCurrentIndex(1);
    ui->tabWidgetActive->setCurrentIndex(0);
}

void MainWindow::onChangeTabToActive(){
    ui->tabWidgetMain->setCurrentIndex(1);
    ui->tabWidgetActive->setCurrentIndex(1);
}

void MainWindow::onChangeTabToDns(){
    ui->tabWidgetMain->setCurrentIndex(1);
    ui->tabWidgetActive->setCurrentIndex(2);
}

void MainWindow::onChangeTabToSSL(){
    ui->tabWidgetMain->setCurrentIndex(1);
    ui->tabWidgetActive->setCurrentIndex(3);
}

void MainWindow::onChangeTabToIpEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(0);
}

void MainWindow::onChangeTabToAsnEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(1);
}

void MainWindow::onChangeTabToCidrEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(2);
}

void MainWindow::onChangeTabToNSEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(3);
}

void MainWindow::onChangeTabToMXEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(4);
}

void MainWindow::onChangeTabToSSLEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(5);
}

void MainWindow::onChangeTabToEmailEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(6);
}

void MainWindow::onChangeTabToUrlEnum(){
    ui->tabWidgetMain->setCurrentIndex(2);
    ui->tabWidgetEnums->setCurrentIndex(7);
}

/* Actions */

void MainWindow::on_actionAbout_triggered(){
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    aboutDialog->show();
}

void MainWindow::on_actionAboutQt_triggered(){
    QMessageBox::aboutQt(this, "aboutQt");
}

void MainWindow::on_actionExit_triggered(){
    QApplication::exit();
}

void MainWindow::on_actionBlog_triggered(){
    QDesktopServices::openUrl(QUrl("https://3nock.github.io", QUrl::TolerantMode));
}

void MainWindow::on_actionTwitter_triggered(){
    QDesktopServices::openUrl(QUrl("https://twitter.com/sub3suite", QUrl::TolerantMode));
}

void MainWindow::onDocumentation_active(){
    DocumentationDialog *documentationDialog = nullptr;

    switch (ui->tabWidgetActive->currentIndex()) {
    case 0:
        documentationDialog = new DocumentationDialog(ENGINE::BRUTE, this);
        break;
    case 1:
        documentationDialog = new DocumentationDialog(ENGINE::ACTIVE, this);
        break;
    case 2:
        documentationDialog = new DocumentationDialog(ENGINE::DNS, this);
        break;
    case 3:
        documentationDialog = new DocumentationDialog(ENGINE::CERT, this);
        break;
    }

    documentationDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    documentationDialog->show();
}

void MainWindow::onDocumentation_passive(){
    DocumentationDialog *documentationDialog = nullptr;
    switch (ui->tabWidgetPassive->currentIndex()) {
    case 0:
        documentationDialog = new DocumentationDialog(ENGINE::OSINT, this);
        break;
    case 1:
        documentationDialog = new DocumentationDialog(ENGINE::RAW, this);
        break;
    }

    documentationDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    documentationDialog->show();
}

void MainWindow::onDocumentation_tools(){
    /*
     * nothing yet...
     */
}

void MainWindow::on_actionApiKeys_triggered(){
    ApiKeysDialog *apiKeys = new ApiKeysDialog(this);
    apiKeys->setAttribute(Qt::WA_DeleteOnClose, true);
    apiKeys->show();
}

void MainWindow::on_actionlogViewer_triggered(){
    LogViewerDialog *logViewerDialog = new LogViewerDialog(this);
    logViewerDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    logViewerDialog->show();
}

void MainWindow::on_actionPreferences_triggered(){
    PreferenceDialog *preferenceDialog = new PreferenceDialog(this);
    preferenceDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    preferenceDialog->show();
}

void MainWindow::on_actionDocumentation_triggered(){
    DocumentationDialog *documentationDialog = nullptr;
    documentationDialog = new DocumentationDialog(this);
    documentationDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    documentationDialog->show();
}

void MainWindow::on_actionConfig_triggered(){
    ProjectConfigDialog projectConfigDialog(projectModel, this);
    projectConfigDialog.setModal(true);
    projectConfigDialog.exec();
}

void MainWindow::on_actionOpenProject_triggered(){
    QString filename = QFileDialog::getOpenFileName(this, "Open", "./", "*.s3s");
    if(filename.isEmpty()){
        return;
    }

    QFile file(filename);
    if(file.exists()){
        QFileInfo fileInfo(file);
        QString name = fileInfo.fileName().remove(".s3s");

        /* opening the project */
        projectModel->openExistingProject(name, filename);
    }
}

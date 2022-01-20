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
#include "src/dialogs/preference/PreferenceDialog.h"
#include "src/dialogs/DocumentationDialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* init */
    this->m_registerMetaTypes();
    this->m_initEngines();
    this->m_documentation();

    /* build info */
    QAction* buildInfo = new QAction(tr("%1 (%2)").arg("Feb 01 2022").arg("BETA"), this);
    buildInfo->setEnabled(false);
    ui->menubar->addAction(buildInfo);

    /* welcome */
    ui->statusbar->showMessage("Welcome!", 5000);
}
MainWindow::~MainWindow(){
    /* tools */
    delete urlTool;
    delete emailTool;
    delete sslTool;
    delete mxTool;
    delete nsTool;
    delete cidrTool;
    delete asnTool;
    delete ipTool;
    delete domainTool;

    /* engines */
    delete raw;
    delete ssl;
    delete dns;
    delete active;
    delete brute;
    delete osint;
    delete ip;

    /* ... */
    delete project;
    delete ui;
}

void MainWindow::m_registerMetaTypes(){
    qDebug() << "Registering meta types...";

    qRegisterMetaType<scan::Log>("scan::Log");
    qRegisterMetaType<dns::ScanResult>("dns::ScanResult");
    qRegisterMetaType<ScanLog>("ScanLog");
    qRegisterMetaType<CidrModelStruct>("CidrModelStruct");
    qRegisterMetaType<MXModelStruct>("MXModelStruct");
    qRegisterMetaType<CidrModelStruct>("CidrModelStruct");
    qRegisterMetaType<AsModelStruct>("AsModelStruct");
    qRegisterMetaType<QSslCertificate>("QSslCertificate");
}

void MainWindow::m_initEngines(){
    /* project */
    projectModel = new ProjectModel;
    project = new Project(this, projectModel);

    /* engines */
    ip = new Ip(this, projectModel);
    osint = new Osint(this, projectModel);
    brute = new Brute(this, projectModel);
    active = new Active(this, projectModel);
    dns = new Dns(this, projectModel);
    ssl = new Ssl(this, projectModel);
    raw = new Raw(this, projectModel);

    /* tools */
    domainTool = new DomainTool(this);
    ipTool = new IpTool(this);
    asnTool = new ASNTool(this);
    cidrTool = new CidrTool(this);
    nsTool = new NSTool(this);
    mxTool = new MXTool(this);
    sslTool = new SSLTool(this);
    emailTool = new EmailTool(this);
    urlTool = new UrlTool(this);

    /* connecting signals and slots */
    this->m_connectSignals(ip);
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
    ui->tabWidgetActive->insertTab(4, ip, "IP");
    ui->tabWidgetActive->setCurrentIndex(0);
    /* tools tabwidget */
    ui->tabWidgetTools->insertTab(0, ipTool, "IP");
    ui->tabWidgetTools->insertTab(1, domainTool, "Domain");
    ui->tabWidgetTools->insertTab(2, asnTool, "ASN");
    ui->tabWidgetTools->insertTab(3, cidrTool, "CIDR");
    ui->tabWidgetTools->insertTab(4, nsTool, "NS");
    ui->tabWidgetTools->insertTab(5, mxTool, "MX");
    ui->tabWidgetTools->insertTab(6, sslTool, "SSL");
    ui->tabWidgetTools->insertTab(7, emailTool, "EMAIL");
    ui->tabWidgetTools->insertTab(8, urlTool, "URL");
    /* main tabwidget */
    ui->tabWidgetMain->insertTab(3, project, "Project");
    ui->tabWidgetMain->setCurrentIndex(0);
}

void MainWindow::m_connectSignals(AbstractEngine *engine){
    /* sending results */
    connect(engine, SIGNAL(sendResultsToOsint(QString, RESULT_TYPE)), osint, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToActive(QString, RESULT_TYPE)), active, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToBrute(QString, RESULT_TYPE)), brute, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToIp(QString, RESULT_TYPE)), ip, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToDns(QString, RESULT_TYPE)), dns, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToCert(QString, RESULT_TYPE)), ssl, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToRaw(QString, RESULT_TYPE)), raw, SLOT(onReceiveTargets(QString, RESULT_TYPE)));

    /* change of tab */
    connect(engine, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(engine, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(engine, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(engine, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(engine, SIGNAL(changeTabToDns()), this, SLOT(onChangeTabToDns()));
    connect(engine, SIGNAL(changeTabToRaw()), this, SLOT(onChangeTabToRaw()));
    connect(engine, SIGNAL(changeTabToCert()), this, SLOT(onChangeTabToCert()));
}

void MainWindow::m_documentation(){
    QWidget *cornerWidget_active = new QWidget(this);
    QWidget *cornerWidget_passive = new QWidget(this);
    QWidget *cornerWidget_tools = new QWidget(this);

    QHBoxLayout *hbox_active = new QHBoxLayout(cornerWidget_active);
    QHBoxLayout *hbox_passive = new QHBoxLayout(cornerWidget_passive);
    QHBoxLayout *hbox_tools = new QHBoxLayout(cornerWidget_tools);

    s3s_ClickableLabel *documentation_active = new s3s_ClickableLabel("", this);
    s3s_ClickableLabel *documentation_passive = new s3s_ClickableLabel("", this);
    s3s_ClickableLabel *documentation_tools = new s3s_ClickableLabel("", this);

    documentation_active->setPixmap(QPixmap(":/img/res/icons/help.png"));
    documentation_passive->setPixmap(QPixmap(":/img/res/icons/help.png"));
    documentation_tools->setPixmap(QPixmap(":/img/res/icons/help.png"));

    hbox_active->addWidget(documentation_active);
    hbox_passive->addWidget(documentation_passive);
    hbox_tools->addWidget(documentation_tools);

    hbox_active->setContentsMargins(0, 0, 10, 2);
    hbox_passive->setContentsMargins(0, 0, 10, 2);
    hbox_tools->setContentsMargins(0, 0, 10, 2);

    ui->tabWidgetPassive->setCornerWidget(cornerWidget_passive);
    ui->tabWidgetActive->setCornerWidget(cornerWidget_active);
    ui->tabWidgetTools->setCornerWidget(cornerWidget_tools);

    connect(documentation_passive, &s3s_ClickableLabel::clicked, this, &MainWindow::onDocumentation_passive);
    connect(documentation_active, &s3s_ClickableLabel::clicked, this, &MainWindow::onDocumentation_active);
    connect(documentation_tools, &s3s_ClickableLabel::clicked, this, &MainWindow::onDocumentation_tools);
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

void MainWindow::onChangeTabToCert(){
    ui->tabWidgetMain->setCurrentIndex(1);
    ui->tabWidgetActive->setCurrentIndex(3);
}

void MainWindow::onChangeTabToIp(){
    ui->tabWidgetMain->setCurrentIndex(1);
    ui->tabWidgetActive->setCurrentIndex(4);
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
    case 4:
        documentationDialog = new DocumentationDialog(ENGINE::IP, this);
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

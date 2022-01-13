/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDesktopServices>
#include "src/dialogs/AboutDialog.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/LogViewerDialog.h"
#include "src/dialogs/preference/PreferenceDialog.h"


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow),
    projectDataModel(new ProjectDataModel)
{
    ui->setupUi(this);

    /* creating and initiating the classes for the modules... */
    project = new Project(this, projectDataModel);
    /* engines */
    ip = new Ip(this, projectDataModel);
    osint = new Osint(this, projectDataModel);
    brute = new Brute(this, projectDataModel);
    active = new Active(this, projectDataModel);
    dns = new Dns(this, projectDataModel);
    ssl = new Ssl(this, projectDataModel);
    raw = new Raw(this, projectDataModel);
    /* tools */
    domainTool = new DomainTool(this);
    ipTool = new IpTool(this);
    asnTool = new ASNTool(this);
    cidrTool = new CidrTool(this);
    nsTool = new NSTool(this);
    mxTool = new MXTool(this);
    sslTool = new SSLTool(this);
    emailTool = new EmailTool(this);

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
    /* main tabwidget */
    ui->tabWidgetMain->insertTab(3, project, "Project");
    ui->tabWidgetMain->setCurrentIndex(0);

    /* registering meta-types */
    qDebug() << "Registering meta types...";
    qRegisterMetaType<scan::Log>("scan::Log");
    qRegisterMetaType<dns::Results>("dns::Results");
    qRegisterMetaType<ScanLog>("ScanLog");
    qRegisterMetaType<CidrModelStruct>("CidrModelStruct");
    qRegisterMetaType<MXModelStruct>("MXModelStruct");
    qRegisterMetaType<CidrModelStruct>("CidrModelStruct");
    qRegisterMetaType<AsModelStruct>("AsModelStruct");

    /* Welcome... */
    ui->statusbar->showMessage("Welcome!", 5000);
}
MainWindow::~MainWindow(){
    /* engines */
    delete ip;
    delete osint;
    delete brute;
    delete active;
    delete dns;
    delete raw;
    delete ssl;
    /* tools */
    delete domainTool;
    delete ipTool;
    delete asnTool;
    delete cidrTool;
    delete nsTool;
    delete mxTool;
    delete sslTool;
    delete emailTool;
    /* ... */
    delete project;
    delete projectDataModel;
    delete ui;
}

void MainWindow::m_connectSignals(AbstractEngine *engine){
    /* for all */
    connect(engine, SIGNAL(sendResultsToOsint(QString, RESULT_TYPE)), osint, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToActive(QString, RESULT_TYPE)), active, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToBrute(QString, RESULT_TYPE)), brute, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToIp(QString, RESULT_TYPE)), ip, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToDns(QString, RESULT_TYPE)), dns, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToCert(QString, RESULT_TYPE)), ssl, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    connect(engine, SIGNAL(sendResultsToRaw(QString, RESULT_TYPE)), raw, SLOT(onReceiveTargets(QString, RESULT_TYPE)));
    /* for change of tab */
    connect(engine, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(engine, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(engine, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(engine, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(engine, SIGNAL(changeTabToDns()), this, SLOT(onChangeTabToDns()));
    connect(engine, SIGNAL(changeTabToRaw()), this, SLOT(onChangeTabToRaw()));
    connect(engine, SIGNAL(changeTabToCert()), this, SLOT(onChangeTabToCert()));
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

/* Actions... */

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

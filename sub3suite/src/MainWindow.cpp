#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDesktopServices>
#include "src/dialogs/AboutDialog.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/LogViewerDialog.h"
#include "src/dialogs/preference/PreferenceDialog.h"
/* Tools */
#include "src/tools/IpTool.h"
#include "src/tools/ASNTool.h"
#include "src/tools/SSLTool.h"
#include "src/tools/EmailTool.h"
#include "src/tools/CidrTool.h"
#include "src/tools/BannerTool.h"
#include "src/tools/DomainTool.h"
#include "src/tools/NSTool.h"
#include "src/tools/MXTool.h"


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow),
    projectDataModel(new ProjectDataModel)
{
    ui->setupUi(this);

    /* creating and initiating the classes for the modules... */
    ip = new Ip(this, projectDataModel);
    osint = new Osint(this, projectDataModel);
    brute = new Brute(this, projectDataModel);
    active = new Active(this, projectDataModel);
    dns = new Dns(this, projectDataModel);
    ssl = new Ssl(this, projectDataModel);
    raw = new Raw(this, projectDataModel);
    project = new Project(this, projectDataModel);

    /* connecting signals and slots */
    this->m_connectSignals(ip);
    this->m_connectSignals(osint);
    this->m_connectSignals(brute);
    this->m_connectSignals(active);
    this->m_connectSignals(dns);
    this->m_connectSignals(ssl);
    this->m_connectSignals(raw);

    /* passive tabwidget */
    ui->tabWidgetPassive->insertTab(0, osint, "Osint");
    ui->tabWidgetPassive->insertTab(1, raw, "Raw");
    ui->tabWidgetPassive->setCurrentIndex(0);
    /* active tabwidget */
    ui->tabWidgetActive->insertTab(0, brute, "Brute");
    ui->tabWidgetActive->insertTab(1, active, "Active");
    ui->tabWidgetActive->insertTab(2, dns, "DNS");
    ui->tabWidgetActive->insertTab(3, ssl, "SSL");
    ui->tabWidgetActive->insertTab(4, ip, "IP");
    ui->tabWidgetActive->setCurrentIndex(0);
    /* main tabwidget */
    ui->tabWidgetMain->insertTab(2, project, "Project");
    ui->tabWidgetMain->setCurrentIndex(0);

    /* Welcome... */
    ui->statusbar->showMessage("Welcome!", 5000);
}
MainWindow::~MainWindow(){
    delete ip;
    delete osint;
    delete brute;
    delete active;
    delete dns;
    delete raw;
    delete ssl;
    delete project;
    //...
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

///
/// showing the status to the status label and logging to the log file...
///
void MainWindow::onReceiveStatus(QString status){
    ui->statusbar->showMessage(status, 5000);
}

///
/// logging the logs to the logfile...
///
void MainWindow::onReceiveLog(QString log){
    Q_UNUSED(log);
}

///
/// changing tabs...
///
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

///
/// Actions...
///
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

/*****************************************************************
                            TOOLS
******************************************************************/
void MainWindow::on_actionIpTool_triggered(){
    IpTool *ipChecker = new IpTool(this);
    ipChecker->setAttribute(Qt::WA_DeleteOnClose, true);
    ipChecker->show();
}

void MainWindow::on_actionASNTool_triggered(){
    ASNTool *asnTool = new ASNTool(this);
    asnTool->setAttribute(Qt::WA_DeleteOnClose, true);
    asnTool->show();
}

void MainWindow::on_actionCertTool_triggered(){
    SSLTool *certTool = new SSLTool(this);
    certTool->setAttribute(Qt::WA_DeleteOnClose, true);
    certTool->show();
}

void MainWindow::on_actionEmailTool_triggered(){
    EmailTool *emailTool = new EmailTool(this);
    emailTool->setAttribute(Qt::WA_DeleteOnClose, true);
    emailTool->show();
}

void MainWindow::on_actionCIDRTool_triggered(){
    CidrTool *cidrTool = new CidrTool(this);
    cidrTool->setAttribute(Qt::WA_DeleteOnClose, true);
    cidrTool->show();
}

void MainWindow::on_actionBannerTool_triggered(){
    /*
    BannerTool *bannerTool = new BannerTool(this);
    bannerTool->setAttribute(Qt::WA_DeleteOnClose, true);
    bannerTool->show();
    */
}

void MainWindow::on_actionDomainTool_triggered(){
    DomainTool *domainTool = new DomainTool(this);
    domainTool->setAttribute(Qt::WA_DeleteOnClose, true);
    domainTool->show();
}

void MainWindow::on_actionNSTool_triggered(){
    NSTool *nsTool = new NSTool(this);
    nsTool->setAttribute(Qt::WA_DeleteOnClose, true);
    nsTool->show();
}

void MainWindow::on_actionMXTool_triggered(){
    MXTool *mxTool = new MXTool(this);
    mxTool->setAttribute(Qt::WA_DeleteOnClose, true);
    mxTool->show();
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

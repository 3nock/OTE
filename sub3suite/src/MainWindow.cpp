#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDesktopServices>
#include "src/dialogs/AboutDialog.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/utils/logs/LogViewerDialog.h"
#include "src/utils/config/GeneralConfigDialog.h"
/* Tools */
#include "src/tools/IpTool.h"
#include "src/tools/ASNTool.h"
#include "src/tools/CertTool.h"
#include "src/tools/EmailTool.h"
#include "src/tools/CidrTool.h"
#include "src/tools/BannerTool.h"
#include "src/tools/DomainTool.h"
#include "src/tools/NSTool.h"
#include "src/tools/MXTool.h"


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow),
    status(new Status),
    resultsModel(new ResultsModel),
    projectDataModel(new ProjectDataModel)
{
    ui->setupUi(this);

    /* creating and initiating the classes for the modules... */
    ip = new Ip(this, resultsModel, projectDataModel, status);
    osint = new Osint(this, resultsModel, projectDataModel, status);
    brute = new Brute(this, resultsModel, projectDataModel, status);
    active = new Active(this, resultsModel, projectDataModel, status);
    records = new DnsRecords(this, resultsModel, projectDataModel, status);
    cert = new Cert(this, resultsModel, projectDataModel, status);
    raw = new Raw(this);
    project = new Project(this, projectDataModel);

    /* connecting signals and slots */
    this->m_connectSignals(ip);
    this->m_connectSignals(osint);
    this->m_connectSignals(brute);
    this->m_connectSignals(active);
    this->m_connectSignals(records);
    this->m_connectSignals(cert);
    this->m_connectSignals(raw);

    /* passive tabwidget */
    ui->tabWidgetPassive->insertTab(0, osint, "Osint");
    ui->tabWidgetPassive->insertTab(1, raw, "Raw");
    ui->tabWidgetPassive->setCurrentIndex(0);
    /* active tabwidget */
    ui->tabWidgetActive->insertTab(0, brute, "Brute");
    ui->tabWidgetActive->insertTab(1, active, "Active");
    ui->tabWidgetActive->insertTab(2, records, "DNS");
    ui->tabWidgetActive->insertTab(3, cert, "Cert");
    ui->tabWidgetActive->insertTab(4, ip, "Ip");
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
    delete records;
    delete raw;
    delete project;
    //...
    delete status;
    delete resultsModel;
    delete projectDataModel;
    delete ui;
}

void MainWindow::m_connectSignals(AbstractEngine *engine){
    /* for all */
    connect(engine, SIGNAL(sendResultsToOsint(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), osint, SLOT(onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToActive(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), active, SLOT(onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToBrute(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), brute, SLOT(onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToIp(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), ip, SLOT(onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToDns(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), records, SLOT(onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToCert(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), cert, SLOT(onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToRaw(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), raw, SLOT(onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToDomainTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToDomainTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToNSTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToNSTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToMXTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToMXTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToASNTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToASNTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToCidrTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToCidrTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToIpTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToIpTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToCertTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToCertTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToEmailTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToEmailTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE)));
    /* for selection */
    connect(engine, SIGNAL(sendResultsToOsint(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), osint, SLOT(onReceiveTargets(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToActive(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), active, SLOT(onReceiveTargets(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToBrute(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), brute, SLOT(onReceiveTargets(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToIp(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), ip, SLOT(onReceiveTargets(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToDns(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), records, SLOT(onReceiveTargets(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToCert(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), cert, SLOT(onReceiveTargets(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToRaw(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), raw, SLOT(onReceiveTargets(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToDomainTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToDomainTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToNSTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToNSTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToMXTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToMXTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToASNTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToASNTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToCidrTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToCidrTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToIpTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToIpTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToCertTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToCertTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
    connect(engine, SIGNAL(sendResultsToEmailTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)), this, SLOT(onSendResultsToEmailTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE)));
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

void MainWindow::onSendResultsToDomainTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToNSTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToMXTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToCertTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToIpTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToASNTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToCidrTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToEmailTool(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToDomainTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToNSTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToMXTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToCertTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToIpTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToASNTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToCidrTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE){

}

void MainWindow::onSendResultsToEmailTool(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE){

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
    CertTool *certTool = new CertTool(this);
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

void MainWindow::on_actionGeneralConfig_triggered(){
    GeneralConfigDialog *generalConfigDialog = new GeneralConfigDialog(this);
    generalConfigDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    generalConfigDialog->show();
}

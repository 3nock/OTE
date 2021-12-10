#include "MainWindow.h"
#include "ui_MainWindow.h"
//...
#include "src/dialogs/AboutDialog.h"

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


/*
 * use https://www.qcustomplot.com/ for plots
 */
MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow),
    status(new Status),
    resultsModel(new ResultsModel),
    projectDataModel(new ProjectDataModel)
{
    ui->setupUi(this);
    ///
    /// creating and initiating the classes for the modules...
    ///
    ip = new Ip(this, resultsModel, projectDataModel, status);
    osint = new Osint(this, resultsModel, projectDataModel, status);
    brute = new Brute(this, resultsModel, projectDataModel, status);
    active = new Active(this, resultsModel, projectDataModel, status);
    records = new DnsRecords(this, resultsModel, projectDataModel, status);
    cert = new Cert(this, resultsModel, projectDataModel, status);
    raw = new Raw(this);
    project = new Project(this, projectDataModel);

    // BRUTE::Sending results...
    connect(brute, SIGNAL(sendSubdomainsToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(brute, SIGNAL(sendSubdomainsToActive(ENGINE, CHOICE, PROXYMODEL_TYPE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(brute, SIGNAL(sendSubdomainsToBrute(ENGINE, CHOICE, PROXYMODEL_TYPE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(brute, SIGNAL(sendIpAddressesToIp(ENGINE, CHOICE, PROXYMODEL_TYPE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(brute, SIGNAL(sendSubdomainsToRecord(ENGINE, CHOICE, PROXYMODEL_TYPE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(brute, SIGNAL(sendSubdomainsToCert(ENGINE, CHOICE, PROXYMODEL_TYPE)), cert, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    //...
    connect(brute, SIGNAL(sendSubdomainsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendSubdomainsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendSubdomainsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendIpAddressesToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendSubdomainsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendSubdomainsToCert(QItemSelectionModel*)), cert, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // ACTIVE::Sending results...
    connect(active, SIGNAL(sendSubdomainsToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(active, SIGNAL(sendSubdomainsToActive(ENGINE, CHOICE, PROXYMODEL_TYPE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(active, SIGNAL(sendSubdomainsToBrute(ENGINE, CHOICE, PROXYMODEL_TYPE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(active, SIGNAL(sendIpAddressesToIp(ENGINE, CHOICE, PROXYMODEL_TYPE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(active, SIGNAL(sendSubdomainsToRecord(ENGINE, CHOICE, PROXYMODEL_TYPE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(active, SIGNAL(sendSubdomainsToCert(ENGINE, CHOICE, PROXYMODEL_TYPE)), cert, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    //...
    connect(active, SIGNAL(sendSubdomainsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendSubdomainsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendSubdomainsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendIpAddressesToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendSubdomainsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendSubdomainsToCert(QItemSelectionModel*)), cert, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // OSINT::Sending results...
    connect(osint, SIGNAL(sendSubdomainsToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(osint, SIGNAL(sendSubdomainsToActive(ENGINE, CHOICE, PROXYMODEL_TYPE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(osint, SIGNAL(sendSubdomainsToBrute(ENGINE, CHOICE, PROXYMODEL_TYPE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(osint, SIGNAL(sendIpAddressesToIp(ENGINE, CHOICE, PROXYMODEL_TYPE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(osint, SIGNAL(sendSubdomainsToRecord(ENGINE, CHOICE, PROXYMODEL_TYPE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(osint, SIGNAL(sendSubdomainsToCert(ENGINE, CHOICE, PROXYMODEL_TYPE)), cert, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    //...
    connect(osint, SIGNAL(sendSubdomainsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendSubdomainsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendSubdomainsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendIpAddressesToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendSubdomainsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendSubdomainsToCert(QItemSelectionModel*)), cert, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // IP::Sending results...
    connect(ip, SIGNAL(sendSubdomainsToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(ip, SIGNAL(sendSubdomainsToActive(ENGINE, CHOICE, PROXYMODEL_TYPE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(ip, SIGNAL(sendSubdomainsToBrute(ENGINE, CHOICE, PROXYMODEL_TYPE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(ip, SIGNAL(sendIpAddressesToIp(ENGINE, CHOICE, PROXYMODEL_TYPE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(ip, SIGNAL(sendSubdomainsToRecord(ENGINE, CHOICE, PROXYMODEL_TYPE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(ip, SIGNAL(sendSubdomainsToCert(ENGINE, CHOICE, PROXYMODEL_TYPE)), cert, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    //...
    connect(ip, SIGNAL(sendSubdomainsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendSubdomainsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendSubdomainsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendIpAddressesToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendSubdomainsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendSubdomainsToCert(QItemSelectionModel*)), cert, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // RECORDS::Sending results...
    connect(records, SIGNAL(sendSubdomainsToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(records, SIGNAL(sendSubdomainsToActive(ENGINE, CHOICE, PROXYMODEL_TYPE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(records, SIGNAL(sendSubdomainsToBrute(ENGINE, CHOICE, PROXYMODEL_TYPE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(records, SIGNAL(sendIpAddressesToIp(ENGINE, CHOICE, PROXYMODEL_TYPE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(records, SIGNAL(sendSubdomainsToRecord(ENGINE, CHOICE, PROXYMODEL_TYPE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(records, SIGNAL(sendSubdomainsToCert(ENGINE, CHOICE, PROXYMODEL_TYPE)), cert, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    //...
    connect(records, SIGNAL(sendSubdomainsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendSubdomainsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendSubdomainsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendIpAddressesToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendSubdomainsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendSubdomainsToCert(QItemSelectionModel*)), cert, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // CERT::Sending results...
    connect(cert, SIGNAL(sendSSLCertToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE)), cert, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(cert, SIGNAL(sendSubdomainsToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(cert, SIGNAL(sendSubdomainsToActive(ENGINE, CHOICE, PROXYMODEL_TYPE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(cert, SIGNAL(sendSubdomainsToBrute(ENGINE, CHOICE, PROXYMODEL_TYPE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(cert, SIGNAL(sendSubdomainsToRecord(ENGINE, CHOICE, PROXYMODEL_TYPE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(cert, SIGNAL(sendSubdomainsToCert(ENGINE, CHOICE, PROXYMODEL_TYPE)), cert, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    //...
    connect(cert, SIGNAL(sendSSLCertToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(cert, SIGNAL(sendSubdomainsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(cert, SIGNAL(sendSubdomainsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(cert, SIGNAL(sendSubdomainsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(cert, SIGNAL(sendSubdomainsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(cert, SIGNAL(sendSubdomainsToCert(QItemSelectionModel*)), cert, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // changing tabs to osint...
    connect(brute, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(active, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(ip, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(records, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(raw, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(cert, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    // changing tabs to active...
    connect(brute, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(osint, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(ip, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(records, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(raw, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(cert, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    // changing tabs to brute...
    connect(active, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(osint, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(ip, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(records, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(raw, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(cert, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    // changing tabs to ip...
    connect(brute, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(osint, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(active, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(records, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(raw, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(cert, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    // changing tabs to records...
    connect(brute, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    connect(osint, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    connect(ip, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    connect(active, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    connect(raw, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    connect(cert, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    // changing tabs to raw...
    connect(brute, SIGNAL(changeTabToRaw()), this, SLOT(onChangeTabToRaw()));
    connect(osint, SIGNAL(changeTabToRaw()), this, SLOT(onChangeTabToRaw()));
    connect(ip, SIGNAL(changeTabToRaw()), this, SLOT(onChangeTabToRaw()));
    connect(records, SIGNAL(changeTabToRaw()), this, SLOT(onChangeTabToRaw()));
    connect(active, SIGNAL(changeTabToRaw()), this, SLOT(onChangeTabToRaw()));
    connect(cert, SIGNAL(changeTabToRaw()), this, SLOT(onChangeTabToRaw()));
    // changing tabs to cert...
    connect(brute, SIGNAL(changeTabToCert()), this, SLOT(onChangeTabToCert()));
    connect(osint, SIGNAL(changeTabToCert()), this, SLOT(onChangeTabToCert()));
    connect(ip, SIGNAL(changeTabToCert()), this, SLOT(onChangeTabToCert()));
    connect(records, SIGNAL(changeTabToCert()), this, SLOT(onChangeTabToCert()));
    connect(active, SIGNAL(changeTabToCert()), this, SLOT(onChangeTabToCert()));
    connect(raw, SIGNAL(changeTabToCert()), this, SLOT(onChangeTabToCert()));

    ///
    /// creating tabs...
    ///

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

    ///
    /// Welcome...
    ///
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
void MainWindow::onChangeTabToRecords(){
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
    BannerTool *bannerTool = new BannerTool(this);
    bannerTool->setAttribute(Qt::WA_DeleteOnClose, true);
    bannerTool->show();
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

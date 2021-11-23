#include "MainWindow.h"
#include "ui_MainWindow.h"
//...
#include "src/dialogs/AboutDialog.h"
#include "src/tools/BannerGrabber.h"
#include "src/tools/IpTool.h"
#include "src/tools/ASNTool.h"
#include "src/tools/CertTool.h"
#include "src/tools/EmailTool.h"

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
    raw = new Raw(this);
    project = new Project(this, projectDataModel);

    // BRUTE::Sending results...
    connect(brute, SIGNAL(sendSubdomainsToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(brute, SIGNAL(sendSubdomainsToActive(ENGINE, CHOICE, PROXYMODEL_TYPE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(brute, SIGNAL(sendSubdomainsToBrute(ENGINE, CHOICE, PROXYMODEL_TYPE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(brute, SIGNAL(sendIpAddressesToIp(ENGINE, CHOICE, PROXYMODEL_TYPE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(brute, SIGNAL(sendSubdomainsToRecord(ENGINE, CHOICE, PROXYMODEL_TYPE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    //...
    connect(brute, SIGNAL(sendSubdomainsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendSubdomainsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendSubdomainsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendIpAddressesToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendSubdomainsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // ACTIVE::Sending results...
    connect(active, SIGNAL(sendSubdomainsToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(active, SIGNAL(sendSubdomainsToActive(ENGINE, CHOICE, PROXYMODEL_TYPE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(active, SIGNAL(sendSubdomainsToBrute(ENGINE, CHOICE, PROXYMODEL_TYPE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(active, SIGNAL(sendIpAddressesToIp(ENGINE, CHOICE, PROXYMODEL_TYPE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(active, SIGNAL(sendSubdomainsToRecord(ENGINE, CHOICE, PROXYMODEL_TYPE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    //...
    connect(active, SIGNAL(sendSubdomainsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendSubdomainsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendSubdomainsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendIpAddressesToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendSubdomainsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // OSINT::Sending results...
    connect(osint, SIGNAL(sendSubdomainsToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(osint, SIGNAL(sendSubdomainsToActive(ENGINE, CHOICE, PROXYMODEL_TYPE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(osint, SIGNAL(sendSubdomainsToBrute(ENGINE, CHOICE, PROXYMODEL_TYPE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(osint, SIGNAL(sendIpAddressesToIp(ENGINE, CHOICE, PROXYMODEL_TYPE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(osint, SIGNAL(sendSubdomainsToRecord(ENGINE, CHOICE, PROXYMODEL_TYPE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    //...
    connect(osint, SIGNAL(sendSubdomainsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendSubdomainsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendSubdomainsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendIpAddressesToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendSubdomainsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // IP::Sending results...
    connect(ip, SIGNAL(sendSubdomainsToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(ip, SIGNAL(sendSubdomainsToActive(ENGINE, CHOICE, PROXYMODEL_TYPE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(ip, SIGNAL(sendSubdomainsToBrute(ENGINE, CHOICE, PROXYMODEL_TYPE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(ip, SIGNAL(sendIpAddressesToIp(ENGINE, CHOICE, PROXYMODEL_TYPE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(ip, SIGNAL(sendSubdomainsToRecord(ENGINE, CHOICE, PROXYMODEL_TYPE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    //...
    connect(ip, SIGNAL(sendSubdomainsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendSubdomainsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendSubdomainsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendIpAddressesToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendSubdomainsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // RECORDS::Sending results...
    connect(records, SIGNAL(sendSubdomainsToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(records, SIGNAL(sendSubdomainsToActive(ENGINE, CHOICE, PROXYMODEL_TYPE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(records, SIGNAL(sendSubdomainsToBrute(ENGINE, CHOICE, PROXYMODEL_TYPE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(records, SIGNAL(sendIpAddressesToIp(ENGINE, CHOICE, PROXYMODEL_TYPE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    connect(records, SIGNAL(sendSubdomainsToRecord(ENGINE, CHOICE, PROXYMODEL_TYPE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE)));
    //...
    connect(records, SIGNAL(sendSubdomainsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendSubdomainsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendSubdomainsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendIpAddressesToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendSubdomainsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // changing tabs to osint...
    connect(brute, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(active, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(ip, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(records, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    // changing tabs to active...
    connect(brute, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(osint, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(ip, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(records, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    // changing tabs to brute...
    connect(active, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(osint, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(ip, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(records, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    // changing tabs to ip...
    connect(brute, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(osint, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(active, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(records, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    // changing tabs to records...
    connect(brute, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    connect(osint, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    connect(ip, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    connect(active, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));

    // creating tabs...
    ui->tabWidget_mainTab->insertTab(0, osint, "Osint");
    ui->tabWidget_mainTab->insertTab(1, raw, "Raw");
    ui->tabWidget_mainTab->insertTab(2, brute, "Brute");
    ui->tabWidget_mainTab->insertTab(3, active, "Active");
    ui->tabWidget_mainTab->insertTab(4, records, "Record");
    ui->tabWidget_mainTab->insertTab(5, ip, "Ip");
    ui->tabWidget_mainTab->insertTab(6, project, "Project");
    //...
    ui->tabWidget_mainTab->setCurrentIndex(0);
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
    ui->tabWidget_mainTab->setCurrentIndex(0);
}
void MainWindow::onChangeTabToBrute(){
    ui->tabWidget_mainTab->setCurrentIndex(1);
}
void MainWindow::onChangeTabToActive(){
    ui->tabWidget_mainTab->setCurrentIndex(2);
}
void MainWindow::onChangeTabToRecords(){
    ui->tabWidget_mainTab->setCurrentIndex(3);
}
void MainWindow::onChangeTabToIp(){
    ui->tabWidget_mainTab->setCurrentIndex(4);
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

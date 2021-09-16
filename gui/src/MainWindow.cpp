#include "MainWindow.h"
#include "ui_MainWindow.h"
//...
#include <QSettings>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),
    statusOsint(new ScanStatus),
    statusBrute(new ScanStatus),
    statusActive(new ScanStatus),
    statusIp(new ScanStatus),
    statusRecords(new ScanStatus),
    status(new Status),
    //...
    resultsModel(new ResultsModel),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ///
    /// setting statuses...
    ///
    status->osint = statusOsint;
    status->brute = statusBrute;
    status->active = statusActive;
    status->ip = statusIp;
    status->records = statusRecords;
    ///
    /// initializing program configuration file settings...
    ///
    config = new QSettings(QDir::currentPath()+"subsuite.ini", QSettings::IniFormat);
    ///
    /// creating the standard item models...
    ///
    resultsModel->ip = new QStandardItemModel;
    resultsModel->brute = new QStandardItemModel;
    resultsModel->osint = new QStandardItemModel;
    resultsModel->active = new QStandardItemModel;
    resultsModel->dnsrecords = new QStandardItemModel;
    resultsModel->srvrecords = new QStandardItemModel;
    resultsModel->project = new ProjectDataModel;
    ///
    /// creating and initiating the classes for the modules...
    ///
    Ip *ip = new Ip(this, resultsModel, status);
    Osint *osint = new Osint(this, resultsModel, status);
    Brute *brute = new Brute(this, resultsModel, status);
    Active *active = new Active(this, resultsModel, status);
    DnsRecords *records = new DnsRecords(this, resultsModel, status);
    Project *project = new Project(this, resultsModel);

    // BRUTE::Sending results...
    connect(brute, SIGNAL(sendResultsToOsint(ENGINE, CHOICE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(brute, SIGNAL(sendResultsToActive(ENGINE, CHOICE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(brute, SIGNAL(sendResultsToBrute(ENGINE, CHOICE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(brute, SIGNAL(sendResultsToIp(ENGINE, CHOICE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(brute, SIGNAL(sendResultsToRecord(ENGINE, CHOICE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    //...
    connect(brute, SIGNAL(sendResultsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendResultsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendResultsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendResultsToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendResultsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // ACTIVE::Sending results...
    connect(active, SIGNAL(sendResultsToOsint(ENGINE, CHOICE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(active, SIGNAL(sendResultsToActive(ENGINE, CHOICE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(active, SIGNAL(sendResultsToBrute(ENGINE, CHOICE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(active, SIGNAL(sendResultsToIp(ENGINE, CHOICE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(active, SIGNAL(sendResultsToRecord(ENGINE, CHOICE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    //...
    connect(active, SIGNAL(sendResultsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendResultsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendResultsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendResultsToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendResultsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // OSINT::Sending results...
    connect(osint, SIGNAL(sendResultsToOsint(ENGINE, CHOICE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(osint, SIGNAL(sendResultsToActive(ENGINE, CHOICE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(osint, SIGNAL(sendResultsToBrute(ENGINE, CHOICE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(osint, SIGNAL(sendResultsToIp(ENGINE, CHOICE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(osint, SIGNAL(sendResultsToRecord(ENGINE, CHOICE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    //...
    connect(osint, SIGNAL(sendResultsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendResultsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendResultsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendResultsToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendResultsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // IP::Sending results...
    connect(ip, SIGNAL(sendResultsToOsint(ENGINE, CHOICE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(ip, SIGNAL(sendResultsToActive(ENGINE, CHOICE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(ip, SIGNAL(sendResultsToBrute(ENGINE, CHOICE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(ip, SIGNAL(sendResultsToIp(ENGINE, CHOICE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(ip, SIGNAL(sendResultsToRecord(ENGINE, CHOICE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    //...
    connect(ip, SIGNAL(sendResultsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendResultsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendResultsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendResultsToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendResultsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // RECORDS::Sending results...
    connect(records, SIGNAL(sendResultsToOsint(ENGINE, CHOICE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(records, SIGNAL(sendResultsToActive(ENGINE, CHOICE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(records, SIGNAL(sendResultsToBrute(ENGINE, CHOICE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(records, SIGNAL(sendResultsToIp(ENGINE, CHOICE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(records, SIGNAL(sendResultsToRecord(ENGINE, CHOICE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    //...
    connect(records, SIGNAL(sendResultsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendResultsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendResultsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendResultsToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendResultsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));

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
    ui->tabWidget_mainTab->insertTab(1, brute, "Brute");
    ui->tabWidget_mainTab->insertTab(2, active, "Active");
    ui->tabWidget_mainTab->insertTab(3, records, "Record");
    ui->tabWidget_mainTab->insertTab(4, ip, "Ip");
    ui->tabWidget_mainTab->insertTab(5, project, "Project");
    //...
    ui->tabWidget_mainTab->setCurrentIndex(0);

    // Welcome...
    ui->statusbar->showMessage("Welcome!", 5000);
}
MainWindow::~MainWindow(){
    delete statusOsint;
    delete statusBrute;
    delete statusActive;
    delete statusIp;
    delete statusRecords;
    delete status;
    //...
    delete config;
    delete resultsModel;
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

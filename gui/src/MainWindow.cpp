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
    statusLevel(new ScanStatus),
    status(new GeneralStatus),
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
    status->level = statusLevel;
    ///
    /// initializing program configuration file settings...
    ///
    config = new QSettings(QDir::currentPath()+"subsuite.ini", QSettings::IniFormat);
    ///
    /// creating the standard item models...
    ///
    resultsModel->ip = new QStandardItemModel;
    resultsModel->brute = new QStandardItemModel;
    resultsModel->level = new QStandardItemModel;
    resultsModel->osint = new QStandardItemModel;
    resultsModel->active = new QStandardItemModel;
    resultsModel->dnsrecords = new QStandardItemModel;
    resultsModel->srvrecords = new QStandardItemModel;
    //...
    resultsModel->project = new ProjectDataModel;
    ///
    /// creating and initiating the classes for the modules...
    ///
    Project *project = new Project(this, resultsModel);
    Active *active = new Active(this, resultsModel);
    Level *level = new Level(this, resultsModel);
    Brute *brute = new Brute(this, resultsModel);
    DnsRecords *records = new DnsRecords(this, resultsModel);
    Ip *ip = new Ip(this, resultsModel);
    Osint *osint = new Osint(this, resultsModel);

    // BRUTE::Sending results...
    connect(brute, SIGNAL(sendResultsToOsint(ENGINE, CHOICE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(brute, SIGNAL(sendResultsToActive(ENGINE, CHOICE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(brute, SIGNAL(sendResultsToBrute(ENGINE, CHOICE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(brute, SIGNAL(sendResultsToIp(ENGINE, CHOICE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(brute, SIGNAL(sendResultsToRecord(ENGINE, CHOICE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(brute, SIGNAL(sendResultsToLevel(ENGINE, CHOICE)), level, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    //...
    connect(brute, SIGNAL(sendResultsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendResultsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendResultsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendResultsToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendResultsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(sendResultsToLevel(QItemSelectionModel*)), level, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // ACTIVE::Sending results...
    connect(active, SIGNAL(sendResultsToOsint(ENGINE, CHOICE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(active, SIGNAL(sendResultsToActive(ENGINE, CHOICE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(active, SIGNAL(sendResultsToBrute(ENGINE, CHOICE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(active, SIGNAL(sendResultsToIp(ENGINE, CHOICE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(active, SIGNAL(sendResultsToRecord(ENGINE, CHOICE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(active, SIGNAL(sendResultsToLevel(ENGINE, CHOICE)), level, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    //...
    connect(active, SIGNAL(sendResultsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendResultsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendResultsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendResultsToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendResultsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(sendResultsToLevel(QItemSelectionModel*)), level, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // OSINT::Sending results...
    connect(osint, SIGNAL(sendResultsToOsint(ENGINE, CHOICE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(osint, SIGNAL(sendResultsToActive(ENGINE, CHOICE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(osint, SIGNAL(sendResultsToBrute(ENGINE, CHOICE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(osint, SIGNAL(sendResultsToIp(ENGINE, CHOICE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(osint, SIGNAL(sendResultsToRecord(ENGINE, CHOICE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(osint, SIGNAL(sendResultsToLevel(ENGINE, CHOICE)), level, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    //...
    connect(osint, SIGNAL(sendResultsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendResultsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendResultsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendResultsToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendResultsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(sendResultsToLevel(QItemSelectionModel*)), level, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // IP::Sending results...
    connect(ip, SIGNAL(sendResultsToOsint(ENGINE, CHOICE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(ip, SIGNAL(sendResultsToActive(ENGINE, CHOICE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(ip, SIGNAL(sendResultsToBrute(ENGINE, CHOICE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(ip, SIGNAL(sendResultsToIp(ENGINE, CHOICE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(ip, SIGNAL(sendResultsToRecord(ENGINE, CHOICE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(ip, SIGNAL(sendResultsToLevel(ENGINE, CHOICE)), level, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    //...
    connect(ip, SIGNAL(sendResultsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendResultsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendResultsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendResultsToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendResultsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(sendResultsToLevel(QItemSelectionModel*)), level, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // RECORDS::Sending results...
    connect(records, SIGNAL(sendResultsToOsint(ENGINE, CHOICE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(records, SIGNAL(sendResultsToActive(ENGINE, CHOICE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(records, SIGNAL(sendResultsToBrute(ENGINE, CHOICE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(records, SIGNAL(sendResultsToIp(ENGINE, CHOICE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(records, SIGNAL(sendResultsToRecord(ENGINE, CHOICE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(records, SIGNAL(sendResultsToLevel(ENGINE, CHOICE)), level, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    //...
    connect(records, SIGNAL(sendResultsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendResultsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendResultsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendResultsToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendResultsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(sendResultsToLevel(QItemSelectionModel*)), level, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // LEVEL::sending results...
    connect(level, SIGNAL(sendResultsToOsint(ENGINE, CHOICE)), osint, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(level, SIGNAL(sendResultsToActive(ENGINE, CHOICE)), active, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(level, SIGNAL(sendResultsToBrute(ENGINE, CHOICE)), brute, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(level, SIGNAL(sendResultsToIp(ENGINE, CHOICE)), ip, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(level, SIGNAL(sendResultsToRecord(ENGINE, CHOICE)), records, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    connect(level, SIGNAL(sendResultsToLevel(ENGINE, CHOICE)), level, SLOT(onReceiveTargets(ENGINE, CHOICE)));
    //...
    connect(level, SIGNAL(sendResultsToOsint(QItemSelectionModel*)), osint, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(level, SIGNAL(sendResultsToActive(QItemSelectionModel*)), active, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(level, SIGNAL(sendResultsToBrute(QItemSelectionModel*)), brute, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(level, SIGNAL(sendResultsToIp(QItemSelectionModel*)), ip, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(level, SIGNAL(sendResultsToRecord(QItemSelectionModel*)), records, SLOT(onReceiveTargets(QItemSelectionModel*)));
    connect(level, SIGNAL(sendResultsToLevel(QItemSelectionModel*)), level, SLOT(onReceiveTargets(QItemSelectionModel*)));

    // changing tabs to osint...
    connect(brute, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(active, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(ip, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(records, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    connect(level, SIGNAL(changeTabToOsint()), this, SLOT(onChangeTabToOsint()));
    // changing tabs to active...
    connect(brute, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(osint, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(ip, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(records, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    connect(level, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));
    // changing tabs to brute...
    connect(active, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(osint, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(ip, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(records, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    connect(level, SIGNAL(changeTabToBrute()), this, SLOT(onChangeTabToBrute()));
    // changing tabs to ip...
    connect(brute, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(osint, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(active, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(records, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    connect(level, SIGNAL(changeTabToIp()), this, SLOT(onChangeTabToIp()));
    // changing tabs to records...
    connect(brute, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    connect(osint, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    connect(ip, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    connect(active, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    connect(level, SIGNAL(changeTabToRecords()), this, SLOT(onChangeTabToRecords()));
    // changing tabs to level...
    connect(brute, SIGNAL(changeTabToLevel()), this, SLOT(onChangeTabToLevel()));
    connect(osint, SIGNAL(changeTabToLevel()), this, SLOT(onChangeTabToLevel()));
    connect(ip, SIGNAL(changeTabToLevel()), this, SLOT(onChangeTabToLevel()));
    connect(records, SIGNAL(changeTabToLevel()), this, SLOT(onChangeTabToLevel()));
    connect(active, SIGNAL(changeTabToLevel()), this, SLOT(onChangeTabToLevel()));

    // creating tabs...
    ui->tabWidget_mainTab->insertTab(0, osint, "Osint");
    ui->tabWidget_mainTab->insertTab(1, brute, "Brute");
    ui->tabWidget_mainTab->insertTab(2, active, "Active");
    ui->tabWidget_mainTab->insertTab(3, records, "Record");
    ui->tabWidget_mainTab->insertTab(4, ip, "Ip");
    ui->tabWidget_mainTab->insertTab(5, level, "Level");
    ui->tabWidget_mainTab->insertTab(7, project, "Project");
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
    delete statusLevel;
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
void MainWindow::onChangeTabToLevel(){
    ui->tabWidget_mainTab->setCurrentIndex(5);
}

#include "MainWindow.h"
#include "ui_MainWindow.h"

/* ************************* Class Constructor & Destructor ********************************/

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow),
    resultsModel(new ResultsModel)
{
    ui->setupUi(this);
    ///
    /// creating the standard item models...
    ///
    resultsModel->ip = new QStandardItemModel;
    resultsModel->save = new QStandardItemModel;
    resultsModel->brute = new QStandardItemModel;
    resultsModel->level = new QStandardItemModel;
    resultsModel->osint = new QStandardItemModel;
    resultsModel->active = new QStandardItemModel;
    resultsModel->record = new QStandardItemModel;
    ///
    /// creating and initiating the classes for the modules...
    ///
    Project *project = new Project(this, resultsModel);
    Active *active = new Active(this, resultsModel);
    Level *level = new Level(this, resultsModel);
    Brute *brute = new Brute(this, resultsModel);
    Dns *records = new Dns(this, resultsModel);
    Ip *ip = new Ip(this, resultsModel);
    Save *save = new Save(this, resultsModel);
    Osint *osint = new Osint(this, resultsModel);

    // BRUTE::Sending results...
    connect(brute, SIGNAL(a_sendToOsint(ENGINE)), osint, SLOT(a_receiveTargets(ENGINE)));
    connect(brute, SIGNAL(a_sendToActive(ENGINE)), active, SLOT(a_receiveTargets(ENGINE)));
    connect(brute, SIGNAL(a_sendToBrute(ENGINE)), brute, SLOT(a_receiveTargets(ENGINE)));
    connect(brute, SIGNAL(a_sendToIp(ENGINE)), ip, SLOT(a_receiveTargets(ENGINE)));
    connect(brute, SIGNAL(a_sendToRecord(ENGINE)), records, SLOT(a_receiveTargets(ENGINE)));
    connect(brute, SIGNAL(a_sendToLevel(ENGINE)), level, SLOT(a_receiveTargets(ENGINE)));
    //...
    connect(brute, SIGNAL(c_sendToOsint(QItenSelectionModel*)), active, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(c_sendToActive(QItemSelectionModel*)), active, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(c_sendToBrute(QItemSelectionModel*)), brute, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(c_sendToIp(QItemSelectionModel*)), ip, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(c_sendToRecord(QItemSelectionModel*)), records, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(brute, SIGNAL(c_sendToLevel(QItemSelectionModel*)), level, SLOT(c_receiveTargets(QItemSelectionModel*)));

    // ACTIVE::Sending results...
    connect(active, SIGNAL(a_sendToOsint(ENGINE)), osint, SLOT(a_receiveTargets(ENGINE)));
    connect(active, SIGNAL(a_sendToActive(ENGINE)), active, SLOT(a_receiveTargets(ENGINE)));
    connect(active, SIGNAL(a_sendToBrute(ENGINE)), brute, SLOT(a_receiveTargets(ENGINE)));
    connect(active, SIGNAL(a_sendToIp(ENGINE)), ip, SLOT(a_receiveTargets(ENGINE)));
    connect(active, SIGNAL(a_sendToRecord(ENGINE)), records, SLOT(a_receiveTargets(ENGINE)));
    connect(active, SIGNAL(a_sendToLevel(ENGINE)), level, SLOT(a_receiveTargets(ENGINE)));
    //...
    connect(active, SIGNAL(c_sendToOsint(QItenSelectionModel*)), active, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(c_sendToActive(QItemSelectionModel*)), active, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(c_sendToBrute(QItemSelectionModel*)), brute, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(c_sendToIp(QItemSelectionModel*)), ip, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(c_sendToRecord(QItemSelectionModel*)), records, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(active, SIGNAL(c_sendToLevel(QItemSelectionModel*)), level, SLOT(c_receiveTargets(QItemSelectionModel*)));

    // OSINT::Sending results...
    connect(osint, SIGNAL(a_sendToOsint(ENGINE)), osint, SLOT(a_receiveTargets(ENGINE)));
    connect(osint, SIGNAL(a_sendToActive(ENGINE)), active, SLOT(a_receiveTargets(ENGINE)));
    connect(osint, SIGNAL(a_sendToBrute(ENGINE)), brute, SLOT(a_receiveTargets(ENGINE)));
    connect(osint, SIGNAL(a_sendToIp(ENGINE)), ip, SLOT(a_receiveTargets(ENGINE)));
    connect(osint, SIGNAL(a_sendToRecord(ENGINE)), records, SLOT(a_receiveTargets(ENGINE)));
    connect(osint, SIGNAL(a_sendToLevel(ENGINE)), level, SLOT(a_receiveTargets(ENGINE)));
    //...
    connect(osint, SIGNAL(c_sendToOsint(QItenSelectionModel*)), active, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(c_sendToActive(QItemSelectionModel*)), active, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(c_sendToBrute(QItemSelectionModel*)), brute, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(c_sendToIp(QItemSelectionModel*)), ip, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(c_sendToRecord(QItemSelectionModel*)), records, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(osint, SIGNAL(c_sendToLevel(QItemSelectionModel*)), level, SLOT(c_receiveTargets(QItemSelectionModel*)));

    // IP::Sending results...
    connect(ip, SIGNAL(a_sendToOsint(ENGINE)), osint, SLOT(a_receiveTargets(ENGINE)));
    connect(ip, SIGNAL(a_sendToActive(ENGINE)), active, SLOT(a_receiveTargets(ENGINE)));
    connect(ip, SIGNAL(a_sendToBrute(ENGINE)), brute, SLOT(a_receiveTargets(ENGINE)));
    connect(ip, SIGNAL(a_sendToIp(ENGINE)), ip, SLOT(a_receiveTargets(ENGINE)));
    connect(ip, SIGNAL(a_sendToRecord(ENGINE)), records, SLOT(a_receiveTargets(ENGINE)));
    connect(ip, SIGNAL(a_sendToLevel(ENGINE)), level, SLOT(a_receiveTargets(ENGINE)));
    //...
    connect(ip, SIGNAL(c_sendToOsint(QItenSelectionModel*)), active, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(c_sendToActive(QItemSelectionModel*)), active, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(c_sendToBrute(QItemSelectionModel*)), brute, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(c_sendToIp(QItemSelectionModel*)), ip, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(c_sendToRecord(QItemSelectionModel*)), records, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(ip, SIGNAL(c_sendToLevel(QItemSelectionModel*)), level, SLOT(c_receiveTargets(QItemSelectionModel*)));

    // RECORDS::Sending results...
    connect(records, SIGNAL(a_sendToOsint(ENGINE)), osint, SLOT(a_receiveTargets(ENGINE)));
    connect(records, SIGNAL(a_sendToActive(ENGINE)), active, SLOT(a_receiveTargets(ENGINE)));
    connect(records, SIGNAL(a_sendToBrute(ENGINE)), brute, SLOT(a_receiveTargets(ENGINE)));
    connect(records, SIGNAL(a_sendToIp(ENGINE)), ip, SLOT(a_receiveTargets(ENGINE)));
    connect(records, SIGNAL(a_sendToRecord(ENGINE)), records, SLOT(a_receiveTargets(ENGINE)));
    connect(records, SIGNAL(a_sendToLevel(ENGINE)), level, SLOT(a_receiveTargets(ENGINE)));
    //...
    connect(records, SIGNAL(c_sendToOsint(QItenSelectionModel*)), active, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(c_sendToActive(QItemSelectionModel*)), active, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(c_sendToBrute(QItemSelectionModel*)), brute, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(c_sendToIp(QItemSelectionModel*)), ip, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(c_sendToRecord(QItemSelectionModel*)), records, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(records, SIGNAL(c_sendToLevel(QItemSelectionModel*)), level, SLOT(c_receiveTargets(QItemSelectionModel*)));

    // LEVEL::sending results...
    connect(level, SIGNAL(a_sendToOsint(ENGINE)), osint, SLOT(a_receiveTargets(ENGINE)));
    connect(level, SIGNAL(a_sendToActive(ENGINE)), active, SLOT(a_receiveTargets(ENGINE)));
    connect(level, SIGNAL(a_sendToBrute(ENGINE)), brute, SLOT(a_receiveTargets(ENGINE)));
    connect(level, SIGNAL(a_sendToIp(ENGINE)), ip, SLOT(a_receiveTargets(ENGINE)));
    connect(level, SIGNAL(a_sendToRecord(ENGINE)), records, SLOT(a_receiveTargets(ENGINE)));
    connect(level, SIGNAL(a_sendToLevel(ENGINE)), level, SLOT(a_receiveTargets(ENGINE)));
    //...
    connect(level, SIGNAL(c_sendToOsint(QItenSelectionModel*)), active, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(level, SIGNAL(c_sendToActive(QItemSelectionModel*)), active, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(level, SIGNAL(c_sendToBrute(QItemSelectionModel*)), brute, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(level, SIGNAL(c_sendToIp(QItemSelectionModel*)), ip, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(level, SIGNAL(c_sendToRecord(QItemSelectionModel*)), records, SLOT(c_receiveTargets(QItemSelectionModel*)));
    connect(level, SIGNAL(c_sendToLevel(QItemSelectionModel*)), level, SLOT(c_receiveTargets(QItemSelectionModel*)));

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
    ui->tabWidget_mainTab->insertTab(6, save, "Save");
    ui->tabWidget_mainTab->insertTab(7, project, "Project");
    //...
    ui->tabWidget_mainTab->setCurrentIndex(0);

    // Welcome...
    ui->label_status->setText(" Welcome!");

    // creating and initiating the logfile...
    QString dateTime = QDateTime::currentDateTime().toString("dd-MM-yyyy");
    logfile = new QFile(QDir::currentPath()+"/logs/"+dateTime+".txt");
    logfile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    logfile->write("\n\n     /**************** TIME: "+QDateTime::currentDateTime().toString("hh:mm:ss").toUtf8()+" *****************/");
    logfile->flush();
}
MainWindow::~MainWindow(){
    delete resultsModel;
    delete ui;
    // closing the log file and deleting the pointer to it...
    logfile->close();
    delete logfile;
}

/* *****************************************************************
*                          custom slots Methods...
* ******************************************************************/

///
/// showing the status to the status label and logging to the log file...
///
void MainWindow::onReceiveStatus(QString status){
    ui->label_status->setText(status);
}

///
/// logging the logs to the logfile...
///
void MainWindow::onReceiveLog(QString log){
    logfile->write(NEWLINE+log.toUtf8());
    logfile->flush();
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


/****************************** Initialization Functions **********************************/

void MainWindow::checkForMissingFiles(){

}

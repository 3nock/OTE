#include "MainWindow.h"
#include "ui_MainWindow.h"

/* ************************* Class Constructor & Destructor ********************************/

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);

    ///
    /// creating and initiating the classes for the modules...
    ///
    Active *active = new Active(this);
    Level *level = new Level(this);
    Osint *osint = new Osint(this);
    Brute *brute = new Brute(this);
    Dns *dns = new Dns(this);
    Save *save = new Save(this);

    // connecting status signals from all the modules to the status slot of main window...
    connect(brute, SIGNAL(sendStatus(QString)), this, SLOT(onReceiveStatus(QString)));
    connect(dns, SIGNAL(sendStatus(QString)), this, SLOT(onReceiveStatus(QString)));
    connect(osint, SIGNAL(sendStatus(QString)), this, SLOT(onReceiveStatus(QString)));
    connect(save, SIGNAL(sendStatus(QString)), this, SLOT(onReceiveStatus(QString)));

    // connecting Logs signals from all the modules to the save on a log file...
    connect(brute, SIGNAL(sendLog(QString)), this, SLOT(onReceiveLog(QString)));
    connect(dns, SIGNAL(sendLog(QString)), this, SLOT(onReceiveLog(QString)));
    connect(osint, SIGNAL(sendLog(QString)), this, SLOT(onReceiveLog(QString)));
    connect(save, SIGNAL(sendLog(QString)), this, SLOT(onReceiveLog(QString)));

    // connecting signals to save the enumerated results to the saveResults class...
    connect(brute, SIGNAL(sendResultsToSave(QString)), save, SLOT(onReceiveResults(QString)));
    connect(dns, SIGNAL(sendResultsToSave(QString)), save, SLOT(onReceiveResults(QString)));
    connect(osint, SIGNAL(sendResultsToSave(QString)), save, SLOT(onReceiveResults(QString)));

    // connecting signal to save enumerated results...
    // connect(save, SIGNAL(getActiveResults()), active, SLOT(onSendResultsToSave()));
    connect(save, SIGNAL(getBruteResults()), brute, SLOT(onSendResultsToSave()));
    connect(save, SIGNAL(getOsintResults()), osint, SLOT(onSendResultsToSave()));

    // connecting signal to test for active subdomains from subdomins in osint enumerated results...
    // connect(osint, SIGNAL(sendResultsToActive(QString)), active, SLOT(onReceiveResults(QString)));
    // connect(active, SIGNAL(getOsintResults_active()), osint, SLOT(onSendResultsToActive()));

    // connecting signals to change tabs when saving and testing results...
    connect(osint, SIGNAL(changeTabToActive()), this, SLOT(changeTabToActive()));
    connect(osint, SIGNAL(changeTabToSave()), this, SLOT(changeTabToSave()));
    connect(brute, SIGNAL(changeTabToSave()), this, SLOT(changeTabToSave()));

    // creating tabs...
    ui->tabWidget_mainTab->insertTab(0, osint, "Osint");
    ui->tabWidget_mainTab->insertTab(1, brute, "Brute");
    ui->tabWidget_mainTab->insertTab(3, active, "Active");
    ui->tabWidget_mainTab->insertTab(4, dns, "Dns++");
    ui->tabWidget_mainTab->insertTab(5, level, "Level");
    ui->tabWidget_mainTab->insertTab(6, save, "Save");

    /* setting tab icons...
    ui->tabWidget_mainTab->setTabIcon(0, QIcon(":/img/images/O.png"));
    ui->tabWidget_mainTab->setTabIcon(1, QIcon(":/img/images/B.png"));
    ui->tabWidget_mainTab->setTabIcon(2, QIcon(":/img/images/A.png"));
    ui->tabWidget_mainTab->setTabIcon(3, QIcon(":/img/images/I.png"));
    ui->tabWidget_mainTab->setTabIcon(4, QIcon(":/img/images/S.png"));*/
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
void MainWindow::changeTabToActive(){
    ui->tabWidget_mainTab->setCurrentIndex(2);
}
void MainWindow::changeTabToSave(){
    ui->tabWidget_mainTab->setCurrentIndex(4);
}

/****************************** Initialization Functions **********************************/

void MainWindow::checkForMissingFiles(){

}

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
    Dns *dns = new Dns(this, resultsModel);
    Ip *ip = new Ip(this, resultsModel);
    Save *save = new Save(this, resultsModel);
    Osint *osint = new Osint(this, resultsModel);

    // Sending results...
    connect(brute, SIGNAL(sendToActive(ENGINE)), active, SLOT(receiveTargets(ENGINE)));

    // changing tabs...
    connect(brute, SIGNAL(changeTabToActive()), this, SLOT(onChangeTabToActive()));

    // creating tabs...
    ui->tabWidget_mainTab->insertTab(0, osint, "Osint");
    ui->tabWidget_mainTab->insertTab(1, brute, "Brute");
    ui->tabWidget_mainTab->insertTab(2, active, "Active");
    ui->tabWidget_mainTab->insertTab(3, dns, "Dns++");
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
void MainWindow::onChangeTabToActive(){
    ui->tabWidget_mainTab->setCurrentIndex(2);
}


/****************************** Initialization Functions **********************************/

void MainWindow::checkForMissingFiles(){

}

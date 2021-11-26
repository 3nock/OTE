#include "Cert.h"
#include "ui_Cert.h"
//...
#include "src/utils/Definitions.h"


Cert::Cert(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::Cert),
    m_args(new certificate::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    targets = ui->targets;
    scanConfig->name = tr("ScanConfig-Cert");
    ///
    /// ...
    ///
    ui->progressBar->hide();
    ui->buttonStop->setDisabled(true);
    ///
    /// equally seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    ///
    /// ...
    ///
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);
    ///
    /// ...
    ///
    connectActions();
}
Cert::~Cert(){
    delete ui;
    delete m_args;
}

void Cert::on_buttonStart_clicked(){

}

void Cert::on_buttonStop_clicked(){

}

void Cert::onScanThreadEnded(){

}

void Cert::onInfoLog(QString log){

}

void Cert::onErrorLog(QString log){

}

void Cert::onScanResult(QByteArray cert){

}

void Cert::onClearResults(){

}

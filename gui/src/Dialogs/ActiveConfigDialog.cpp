#include "ActiveConfigDialog.h"
#include "ui_ActiveConfigDialog.h"


ActiveConfigDialog::ActiveConfigDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ActiveConfigDialog)
{
    ui->setupUi(this);
    ///
    /// hiding appropriate widgets...
    ///
    ui->customNameservers->hide();
    this->adjustSize();

    ///
    /// setting placeholdertxts...
    ///
    ui->lineEditTimeout->setPlaceholderText("e.g. 3");
    ui->lineEditThreads->setPlaceholderText("e.g. 100");

    ///
    /// setting validators...
    ///
    ui->lineEditTimeout->setValidator(new QIntValidator(1, 1000, this));
    ui->lineEditThreads->setValidator(new QIntValidator(1, 200, this));
}
ActiveConfigDialog::~ActiveConfigDialog(){
    delete ui;
}

void ActiveConfigDialog::on_radioButtonCustomNameservers_clicked(){
    if(!ui->customNameservers->isHidden())
        return;

    ui->customNameservers->show();
    ui->groupBoxNameserver->adjustSize();
    this->adjustSize();
}

void ActiveConfigDialog::on_radioButtonRandomNameservers_clicked(){
    if(ui->customNameservers->isHidden())
        return;

    ui->customNameservers->hide();
    ui->groupBoxNameserver->adjustSize();
    this->adjustSize();
}

void ActiveConfigDialog::on_radioButtonSingleNameserver_clicked(){
    if(ui->customNameservers->isHidden())
        return;

    ui->customNameservers->hide();
    ui->groupBoxNameserver->adjustSize();
    this->adjustSize();
}

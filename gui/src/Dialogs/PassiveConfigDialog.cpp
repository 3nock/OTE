#include "PassiveConfigDialog.h"
#include "ui_PassiveConfigDialog.h"
//...
#include <QIntValidator>


PassiveConfigDialog::PassiveConfigDialog(QWidget *parent) : QDialog(parent), ui(new Ui::PassiveConfigDialog)
{
    ui->setupUi(this);

    ///
    /// adjusting the dialog size...
    ///
    this->adjustSize();

    ///
    /// setting placeholder texts..
    ///
    ui->lineEditTimeout->setPlaceholderText("e.g. 3");
    ui->lineEditMaxPages->setPlaceholderText("e.g. 100");
    ui->lineEditProxyPort->setPlaceholderText("e.g. 8080");
    ui->lineEditProxyAddress->setPlaceholderText("e.g. 10.10.10.10");

    ///
    /// validators...
    ///
    ui->lineEditTimeout->setValidator(new QIntValidator(1, 1000, this));
    ui->lineEditMaxPages->setValidator(new QIntValidator(1, 1000, this));
    ui->lineEditProxyPort->setValidator(new QIntValidator(1, 65535, this));
}
PassiveConfigDialog::~PassiveConfigDialog(){
    delete ui;
}

void PassiveConfigDialog::on_buttonOk_clicked(){

}

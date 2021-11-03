#include "IpChecker.h"
#include "ui_IpChecker.h"

IpChecker::IpChecker(QWidget *parent) : QDialog(parent), ui(new Ui::IpChecker)
{
    ui->setupUi(this);
    ///
    /// placeholder texts...
    ///
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText("Target eg 1.1.1.1");
}
IpChecker::~IpChecker(){
    delete ui;
}

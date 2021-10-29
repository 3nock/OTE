#include "IpChecker.h"
#include "ui_IpChecker.h"

IpChecker::IpChecker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IpChecker)
{
    ui->setupUi(this);
}

IpChecker::~IpChecker()
{
    delete ui;
}

#include "HostnameChecker.h"
#include "ui_HostnameChecker.h"

HostnameChecker::HostnameChecker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HostnameChecker)
{
    ui->setupUi(this);
}

HostnameChecker::~HostnameChecker()
{
    delete ui;
}

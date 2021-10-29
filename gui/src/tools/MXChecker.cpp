#include "MXChecker.h"
#include "ui_MXChecker.h"

MXChecker::MXChecker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MXChecker)
{
    ui->setupUi(this);
}

MXChecker::~MXChecker()
{
    delete ui;
}

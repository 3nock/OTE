#include "ASNChecker.h"
#include "ui_ASNChecker.h"

ASNChecker::ASNChecker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ASNChecker)
{
    ui->setupUi(this);
}

ASNChecker::~ASNChecker()
{
    delete ui;
}

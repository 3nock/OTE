#include "EmailChecker.h"
#include "ui_EmailChecker.h"

EmailChecker::EmailChecker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailChecker)
{
    ui->setupUi(this);
}

EmailChecker::~EmailChecker()
{
    delete ui;
}

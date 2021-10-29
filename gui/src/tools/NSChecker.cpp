#include "NSChecker.h"
#include "ui_NSChecker.h"

NSChecker::NSChecker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NSChecker)
{
    ui->setupUi(this);
}

NSChecker::~NSChecker()
{
    delete ui;
}

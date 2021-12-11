#include "FailedScansDialog.h"
#include "ui_FailedScansDialog.h"

FailedScansDialog::FailedScansDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FailedScansDialog)
{
    ui->setupUi(this);
}

FailedScansDialog::~FailedScansDialog()
{
    delete ui;
}

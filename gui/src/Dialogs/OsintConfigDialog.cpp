#include "OsintConfigDialog.h"
#include "ui_OsintConfigDialog.h"

OsintConfigDialog::OsintConfigDialog(QWidget *parent) :QDialog(parent),ui(new Ui::OsintConfigDialog)
{
    ui->setupUi(this);
}
OsintConfigDialog::~OsintConfigDialog(){
    delete ui;
}

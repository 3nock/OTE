#include "EnumConfigDialog.h"
#include "ui_EnumConfigDialog.h"


EnumConfigDialog::EnumConfigDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::EnumConfigDialog)
{
    ui->setupUi(this);
}
EnumConfigDialog::~EnumConfigDialog(){
    delete ui;
}

void EnumConfigDialog::on_buttonOk_clicked(){
    accept();
}

void EnumConfigDialog::on_buttonCancel_clicked(){
    accept();
}

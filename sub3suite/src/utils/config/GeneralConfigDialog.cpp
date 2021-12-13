#include "GeneralConfigDialog.h"
#include "ui_GeneralConfigDialog.h"

GeneralConfigDialog::GeneralConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralConfigDialog)
{
    ui->setupUi(this);
}

GeneralConfigDialog::~GeneralConfigDialog()
{
    delete ui;
}

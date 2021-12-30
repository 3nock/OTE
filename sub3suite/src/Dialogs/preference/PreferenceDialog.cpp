#include "PreferenceDialog.h"
#include "ui_PreferenceDialog.h"

PreferenceDialog::PreferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);
}

PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

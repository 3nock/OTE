#include "LogViewerDialog.h"
#include "ui_LogViewerDialog.h"

LogViewerDialog::LogViewerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogViewerDialog)
{
    ui->setupUi(this);
}

LogViewerDialog::~LogViewerDialog()
{
    delete ui;
}

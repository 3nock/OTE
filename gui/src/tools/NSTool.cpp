#include "NSTool.h"
#include "ui_NSTool.h"

NSTool::NSTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NSTool)
{
    ui->setupUi(this);
}

NSTool::~NSTool()
{
    delete ui;
}

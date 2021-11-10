#include "MXTool.h"
#include "ui_MXTool.h"

MXTool::MXTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MXTool)
{
    ui->setupUi(this);
}

MXTool::~MXTool()
{
    delete ui;
}

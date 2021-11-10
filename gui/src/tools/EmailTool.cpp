#include "EmailTool.h"
#include "ui_EmailTool.h"

EmailTool::EmailTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailTool)
{
    ui->setupUi(this);
}

EmailTool::~EmailTool()
{
    delete ui;
}

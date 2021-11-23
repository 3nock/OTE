#include "DomainTool.h"
#include "ui_DomainTool.h"

DomainTool::DomainTool(QWidget *parent) : QDialog(parent), ui(new Ui::DomainTool)
{
    ui->setupUi(this);
}

DomainTool::~DomainTool()
{
    delete ui;
}

#include "CertTool.h"
#include "ui_CertTool.h"

CertTool::CertTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CertTool)
{
    ui->setupUi(this);
}

CertTool::~CertTool()
{
    delete ui;
}

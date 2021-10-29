#include "CertChecker.h"
#include "ui_CertChecker.h"

CertChecker::CertChecker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CertChecker)
{
    ui->setupUi(this);
}

CertChecker::~CertChecker()
{
    delete ui;
}

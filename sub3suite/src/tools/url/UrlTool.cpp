#include "UrlTool.h"
#include "ui_UrlTool.h"

UrlTool::UrlTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UrlTool)
{
    ui->setupUi(this);
}

UrlTool::~UrlTool()
{
    delete ui;
}

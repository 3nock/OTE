#include "ExtractorTool.h"
#include "ui_ExtractorTool.h"

ExtractorTool::ExtractorTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtractorTool)
{
    ui->setupUi(this);
}

ExtractorTool::~ExtractorTool()
{
    delete ui;
}

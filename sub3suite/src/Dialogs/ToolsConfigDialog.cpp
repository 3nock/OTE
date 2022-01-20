#include "ToolsConfigDialog.h"
#include "ui_ToolsConfigDialog.h"


ToolsConfigDialog::ToolsConfigDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::ToolsConfigDialog)
{
    ui->setupUi(this);
}
ToolsConfigDialog::~ToolsConfigDialog(){
    delete ui;
}

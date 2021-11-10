#include "ASNTool.h"
#include "ui_ASNTool.h"

ASNTool::ASNTool(QWidget *parent) : QDialog(parent), ui(new Ui::ASNTool)
{
    ui->setupUi(this);
    ///
    /// ....
    ///
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText("asn eg 15169");
}
ASNTool::~ASNTool(){
    delete ui;
}

void ASNTool::on_buttonAnalyze_clicked(){

}

#include "DomainTool.h"
#include "ui_DomainTool.h"
#include "src/dialogs/PassiveConfigDialog.h"


DomainTool::DomainTool(QWidget *parent) : QDialog(parent), ui(new Ui::DomainTool)
{
    ui->setupUi(this);
}

DomainTool::~DomainTool()
{
    delete ui;
}

void DomainTool::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}

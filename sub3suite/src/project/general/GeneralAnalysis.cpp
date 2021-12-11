#include "GeneralAnalysis.h"
#include "ui_GeneralAnalysis.h"

GeneralAnalysis::GeneralAnalysis(QWidget *parent) :QWidget(parent),ui(new Ui::GeneralAnalysis)
{
    ui->setupUi(this);
}
GeneralAnalysis::~GeneralAnalysis(){
    delete ui;
}

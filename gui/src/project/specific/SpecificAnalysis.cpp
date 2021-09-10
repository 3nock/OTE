#include "SpecificAnalysis.h"
#include "ui_SpecificAnalysis.h"

SpecificAnalysis::SpecificAnalysis(QWidget *parent) :QWidget(parent),ui(new Ui::SpecificAnalysis)
{
    ui->setupUi(this);
}
SpecificAnalysis::~SpecificAnalysis(){
    delete ui;
}

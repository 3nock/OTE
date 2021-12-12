#include "WordlistGeneratorWidget.h"
#include "ui_WordlistGeneratorWidget.h"

WordlistGeneratorWidget::WordlistGeneratorWidget(QWidget *parent) : QWidget(parent), ui(new Ui::WordlistGeneratorWidget)
{
    ui->setupUi(this);
}
WordlistGeneratorWidget::~WordlistGeneratorWidget(){
    delete ui;
}

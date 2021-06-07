#include "WordlistGeneratorDialog.h"
#include "ui_WordlistGeneratorDialog.h"

WordListGeneratorDialog::WordListGeneratorDialog(QWidget *parent) :QDialog(parent), ui(new Ui::WordListGeneratorDialog){
    ui->setupUi(this);
}

WordListGeneratorDialog::~WordListGeneratorDialog(){
    delete ui;
}

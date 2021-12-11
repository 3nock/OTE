#include "Notes.h"
#include "ui_Notes.h"

Notes::Notes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Notes)
{
    ui->setupUi(this);
}

Notes::~Notes()
{
    delete ui;
}

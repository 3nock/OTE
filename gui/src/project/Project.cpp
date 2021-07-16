#include "Project.h"
#include "ui_Project.h"

Project::Project(QWidget *parent) :QWidget(parent), ui(new Ui::Project)
{
    ui->setupUi(this);

}
Project::~Project(){
    delete ui;
}

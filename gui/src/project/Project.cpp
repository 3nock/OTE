#include "Project.h"
#include "ui_Project.h"

Project::Project(QWidget *parent, ResultsModel *resultsModel) :QWidget(parent), ui(new Ui::Project),
    m_resultsModel(resultsModel)
{
    ui->setupUi(this);

}
Project::~Project(){
    delete ui;
}

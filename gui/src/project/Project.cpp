#include "Project.h"
#include "ui_Project.h"

Project::Project(QWidget *parent, ResultsModel *resultsModel) :QWidget(parent), ui(new Ui::Project),
    m_resultsModel(resultsModel)
{
    ui->setupUi(this);
    //...
    ui->treeView->setModel(m_resultsModel->project->model);

}
Project::~Project(){
    delete ui;
}

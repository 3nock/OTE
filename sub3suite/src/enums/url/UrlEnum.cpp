#include "UrlEnum.h"
#include "ui_UrlEnum.h"


UrlEnum::UrlEnum(QWidget *parent, ProjectModel *project) : AbstractEnum(parent, project),
    ui(new Ui::UrlEnum)
{
    ui->setupUi(this);
}
UrlEnum::~UrlEnum(){
    delete ui;
}

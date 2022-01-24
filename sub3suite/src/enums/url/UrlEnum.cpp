#include "UrlEnum.h"
#include "ui_UrlEnum.h"


UrlEnum::UrlEnum(QWidget *parent, ProjectModel *project) : AbstractEnum(parent, project),
    ui(new Ui::UrlEnum)
{
    ui->setupUi(this);

    ui->frame->setProperty("default_frame", true);

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
}
UrlEnum::~UrlEnum(){
    delete ui;
}

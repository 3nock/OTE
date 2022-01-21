#ifndef URLENUM_H
#define URLENUM_H

#include "../AbstractEnum.h"


namespace Ui {
class UrlEnum;
}

class UrlEnum : public AbstractEnum {
        Q_OBJECT

    public:
        UrlEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~UrlEnum();

    private:
        Ui::UrlEnum *ui;
};

#endif // URLENUM_H

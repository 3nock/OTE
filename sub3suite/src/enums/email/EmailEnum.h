/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef EMAILENUM_H
#define EMAILENUM_H

#include "../AbstractEnum.h"


namespace Ui {
class EmailEnum;
}

class EmailEnum : public AbstractEnum {
        Q_OBJECT

    public:
        EmailEnum(QWidget *parent = nullptr, ProjectModel *project = nullptr);
        ~EmailEnum();

    private slots:
        void on_buttonConfig_clicked();

    private:
        Ui::EmailEnum *ui;
};

#endif // EMAILENUM_H

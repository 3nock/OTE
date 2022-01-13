/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef EMAILTOOL_H
#define EMAILTOOL_H

#include <QWidget>

namespace Ui {
class EmailTool;
}

class EmailTool : public QWidget
{
    Q_OBJECT

public:
    explicit EmailTool(QWidget *parent = nullptr);
    ~EmailTool();

private slots:
    void on_buttonConfig_clicked();

private:
    Ui::EmailTool *ui;
};

#endif // EMAILCHECKER_H

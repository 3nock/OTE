/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef HOSTNAMETOOL_H
#define HOSTNAMETOOL_H

#include <QDialog>

namespace Ui {
class DomainTool;
}

class DomainTool : public QDialog
{
    Q_OBJECT

public:
    explicit DomainTool(QWidget *parent = nullptr);
    ~DomainTool();

private slots:
    void on_buttonConfig_clicked();

private:
    Ui::DomainTool *ui;
};

#endif // HOSTNAMETOOL_H

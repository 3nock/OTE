/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "EmailTool.h"
#include "ui_EmailTool.h"
#include "src/dialogs/PassiveConfigDialog.h"


EmailTool::EmailTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailTool)
{
    ui->setupUi(this);
}

EmailTool::~EmailTool()
{
    delete ui;
}

void EmailTool::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}

/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "DomainTool.h"
#include "ui_DomainTool.h"
#include "src/dialogs/PassiveConfigDialog.h"


DomainTool::DomainTool(QWidget *parent) : QWidget(parent),
    ui(new Ui::DomainTool)
{
    ui->setupUi(this);

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
}
DomainTool::~DomainTool(){
    delete ui;
}

void DomainTool::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}

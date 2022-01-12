/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "ExtractorTool.h"
#include "ui_ExtractorTool.h"

ExtractorTool::ExtractorTool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExtractorTool)
{
    ui->setupUi(this);
}

ExtractorTool::~ExtractorTool()
{
    delete ui;
}

/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "GeneralAnalysis.h"
#include "ui_GeneralAnalysis.h"

GeneralAnalysis::GeneralAnalysis(QWidget *parent) :QWidget(parent),ui(new Ui::GeneralAnalysis)
{
    ui->setupUi(this);
}
GeneralAnalysis::~GeneralAnalysis(){
    delete ui;
}

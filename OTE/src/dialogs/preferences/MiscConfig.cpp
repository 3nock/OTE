//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "MiscConfig.h"
#include "ui_MiscConfig.h"

#include <QFontDialog>
#include <QMessageBox>

#include "src/core/Config.h"


MiscConfig::MiscConfig(QWidget *parent) :
    AbstractConfig(parent),
    ui(new Ui::MiscConfig)
{
    ui->setupUi(this);

    ui->labelExplain->setText(tr("Miscellaneous Configurations"));
    ui->labelExplainFont->setText(tr("Configure the entire Application's Font type, size and style"));
}

MiscConfig::~MiscConfig()
{
    delete ui;
}

void MiscConfig::openConfig()
{
    ui->checkBoxDisableAutomaticUpdate->setChecked(OTE::gConfig.general.disableAutomaticUpdateCheck);

    ui->labelFontName->setText(OTE::gConfig.general.font_name);
    ui->labelFontSize->setText(QString::number(OTE::gConfig.general.font_size));
    ui->labelFontStyle->setText(OTE::gConfig.general.font_style);
}

void MiscConfig::saveConfig()
{
    OTE::gConfig.general.disableAutomaticUpdateCheck = ui->checkBoxDisableAutomaticUpdate->isChecked();
    OTE::gConfig.general.font_name = ui->labelFontName->text();
    OTE::gConfig.general.font_style = ui->labelFontStyle->text();
    OTE::gConfig.general.font_size = ui->labelFontSize->text().toInt();
}

void MiscConfig::resetConfig()
{

}

void MiscConfig::on_buttonChangeFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, qApp->font(), this);

    if (ok)
    {
        ui->labelFontName->setText(font.family());
        OTE::gConfig.general.font_name = font.family();
        OTE::gConfig.general.font_style = font.styleName();
        OTE::gConfig.general.font_size = font.pointSize();

        ui->labelFontName->setText(font.family());
        ui->labelFontStyle->setText(font.styleName());
        ui->labelFontSize->setText(QString::number(font.pointSize()));

        QMessageBox::information(this, tr("Info"), tr("Font Changes will be applied when you restart SpiderSuite"));
    }
}

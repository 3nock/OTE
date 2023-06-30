//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef DIALOGS_PREFERENCES_GENERALCCONFIG_H
#define DIALOGS_PREFERENCES_GENERALCCONFIG_H

#include "AbstractConfig.h"

namespace Ui {
class GeneralConfig;
}

class GeneralConfig : public AbstractConfig
{
    Q_OBJECT

public:
    explicit GeneralConfig(QWidget *parent = nullptr);
    ~GeneralConfig() override;

    void openConfig() override;
    void saveConfig() override;

private slots:
    void on_buttonChangeFont_clicked();
    void on_radioButtonDarkTheme_clicked(bool checked);
    void on_radioButtonLightTheme_clicked(bool checked);

private:
    Ui::GeneralConfig *ui;
};

#endif // DIALOGS_PREFERENCES_GENERALCCONFIG_H

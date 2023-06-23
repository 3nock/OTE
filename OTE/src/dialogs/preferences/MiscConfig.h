//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef DIALOGS_PREFERENCES_MISCCONFIG_H
#define DIALOGS_PREFERENCES_MISCCONFIG_H

#include "AbstractConfig.h"

namespace Ui {
class MiscConfig;
}

class MiscConfig : public AbstractConfig
{
    Q_OBJECT

public:
    explicit MiscConfig(QWidget *parent = nullptr);
    ~MiscConfig() override;

    void openConfig() override;
    void saveConfig() override;
    void resetConfig() override;

private slots:
    void on_buttonChangeFont_clicked();

private:
    Ui::MiscConfig *ui;
};

#endif // DIALOGS_PREFERENCES_MISCCONFIG_H

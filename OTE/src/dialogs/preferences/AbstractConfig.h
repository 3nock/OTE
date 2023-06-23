//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef DIALOGS_PREFERENCES_ABSTRACTCONFIG_H
#define DIALOGS_PREFERENCES_ABSTRACTCONFIG_H

#include <QWidget>

class AbstractConfig : public QWidget
{
    Q_OBJECT

public:
    explicit AbstractConfig(QWidget *parent = nullptr):
        QWidget(parent){}
    ~AbstractConfig(){}

    virtual void openConfig() = 0;
    virtual void saveConfig() = 0;
    virtual void resetConfig() = 0;

};

#endif // DIALOGS_PREFERENCES_ABSTRACTCONF_H

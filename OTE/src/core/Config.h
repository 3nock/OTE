//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

#include <QMap>
#include <QString>

namespace OTE {

struct Configs
{
    struct {
        QString font_name;
        QString font_style;
        int font_size;
        int highDPI = 1;

        QString recentDirectory;
        QMap<QString,QString> recents;
        bool disableAutomaticUpdateCheck;
    }general;

    struct {
        QPair<bool, int> timeout;
    }query;

    QMap<QString,QPair<QString,QString> > apiKeys;

    void fromConfigFile();
    void toConfigFile();
    void createNewConfigFile();
};

extern OTE::Configs gConfig;

} // OTE namespace

#endif // CORE_CONFIG_H
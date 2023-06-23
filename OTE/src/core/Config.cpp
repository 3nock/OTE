//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "Config.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QStandardPaths>

namespace OTE {

Configs gConfig;

void Configs::fromConfigFile()
{
    // get settings from settings file
    QString filename(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/ote.conf");

    if(!QFileInfo::exists(filename))
    {
        QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
        if(!dir.exists())
            QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
        createNewConfigFile();
        return;
    }

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Couldn't open settings file: " << file.fileName();
        return;
    }
    QJsonObject settings = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    // renew config file
    gConfig = Configs{};

    // general configurations
    QJsonObject generalObj = settings["General"].toObject();
    general.font_name = generalObj["font_name"].toString();
    general.font_style = generalObj["font_style"].toString();
    general.font_size = generalObj["font_size"].toInt();
    general.highDPI = generalObj["highDPI"].toInt();
    general.disableAutomaticUpdateCheck = generalObj["disableAutomaticUpdateCheck"].toBool();
    general.recentDirectory = generalObj["recentDirectory"].toString();

    QJsonObject recentsObj = generalObj["recents"].toObject();
    foreach(const QString &name, recentsObj.keys())
    {
        QString project = recentsObj[name].toString();
        if(QFileInfo(project).exists())
            general.recents.insert(name, project);
    }

    // api keys
    QJsonObject apiKeysObj = settings["apiKeys"].toObject();
    foreach(const QString &key, apiKeysObj.keys())
    {
        QJsonObject apiKeyObj = apiKeysObj[key].toObject();
        apiKeys.insert(key, {apiKeyObj["user"].toString(), apiKeyObj["key"].toString()});
    }
}

void Configs::toConfigFile()
{
    // general configurations
    QJsonObject generalObj;
    generalObj.insert("font_name", general.font_name);
    generalObj.insert("font_style", general.font_style);
    generalObj.insert("font_size", general.font_size);
    generalObj.insert("highDPI", general.highDPI);
    generalObj.insert("disableAutomaticUpdateCheck", general.disableAutomaticUpdateCheck);
    generalObj.insert("recentDirectory", general.recentDirectory);

    QJsonObject recentsObj;
    foreach(const QString &project_name, general.recents.keys())
        recentsObj.insert(project_name, general.recents[project_name]);
    generalObj.insert("recents", recentsObj);

    // api keys
    QJsonObject apiKeysObj;
    foreach(const QString &key, apiKeys.keys())
    {
        QJsonObject apiKeyObj;
        QPair<QString,QString> user_key_pair = apiKeys[key];

        apiKeyObj.insert("user", user_key_pair.first);
        apiKeyObj.insert("key", user_key_pair.second);

        apiKeysObj.insert(key, apiKeyObj);
    }


    QJsonObject settings;
    settings.insert("General", generalObj);
    settings.insert("apiKeys", apiKeysObj);

    // save settings
    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/ote.conf");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        qWarning() << "Couldn't open settings file: " << file.fileName();
        return;
    }
    file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner |
                        QFileDevice::ReadUser | QFileDevice::WriteUser |
                        QFileDevice::ReadGroup | QFileDevice::WriteGroup |
                        QFileDevice::ReadOther | QFileDevice::WriteOther);

    QJsonDocument doc;
    doc.setObject(settings);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}

void Configs::createNewConfigFile()
{
    QJsonObject generalObj;
#ifdef Q_OS_WIN32
    generalObj.insert("font_name", "Segoe UI");
    generalObj.insert("font_style", "Regular");
    generalObj.insert("font_size", 8);
#elif defined(Q_OS_MAC)
        // Just in case
#else
    generalObj.insert("font_name", "Sans Serif");
    generalObj.insert("font_style", "Regular");
    generalObj.insert("font_size", 8);
#endif
    generalObj.insert("highDPI", 1);
    generalObj.insert("disableAutomaticUpdateCheck", false);
    generalObj.insert("recentDirectory", "./");

    QJsonObject recentsObj;
    generalObj.insert("recents", recentsObj);

    QJsonObject settings;
    settings.insert("General", generalObj);

    // save settings
    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)+"/ote.conf");
    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner |
                            QFileDevice::ReadUser | QFileDevice::WriteUser |
                            QFileDevice::ReadGroup | QFileDevice::WriteGroup |
                            QFileDevice::ReadOther | QFileDevice::WriteOther);

        QJsonDocument doc;
        doc.setObject(settings);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
    else
        qWarning() << "Couldn't open settings file: " << file.fileName();

#ifdef Q_OS_WIN32
    general.font_name = "Segoe UI";
    general.font_style = "Regular";
    general.font_size = 8;
#elif defined(Q_OS_MAC)
        // Just in case
#else
    general.font_name = "Sans Serif";
    general.font_style = "Regular";
    general.font_size = 8;
#endif
    general.highDPI =1;
    general.disableAutomaticUpdateCheck = false;
    general.recentDirectory = "./";
}

} // namespace OTE

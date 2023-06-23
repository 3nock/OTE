//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "UpdateChecker.h"

#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

namespace OTE {

UpdateChecker::UpdateChecker(QWidget* parent):
    mParent(parent)
{
    connect(this, &UpdateChecker::finished, this, &UpdateChecker::onFinished);
}

void UpdateChecker::checkForUpdates()
{
    get(QNetworkRequest(QUrl("https://api.github.com/repos/3nock/SpiderSuite/releases/latest")));
    qDebug() << "Checking for Updates...";
}

void UpdateChecker::checkForUpdates_onStart()
{
    get(QNetworkRequest(QUrl("https://api.github.com/repos/3nock/SpiderSuite/releases/latest")));
    qDebug() << "Checking for Updates...";
    onStart = true;
}

void UpdateChecker::onFinished(QNetworkReply* reply)
{
    if(onStart)
    {
        if(reply->error() == QNetworkReply::NoError)
        {
            QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
            QJsonObject mainObj = document.object();

            QDateTime serverTime = QDateTime::fromString(mainObj["published_at"].toString(), Qt::ISODate);
            if(serverTime.isValid())
            {
                QDateTime build = QDateTime::fromString("2023-06-06", "yyyy-MM-dd");

                if(serverTime.date() > build.date()){
                    QString info = QString(tr("New build %1 available!<br>Download <a href=\"https://github.com/3nock/SpiderSuite/releases\">here</a><br><br>You are now on build %2")).arg(serverTime.date().toString()).arg(build.date().toString());
                    QMessageBox::information(mParent, tr("New Update!"), info);
                    qDebug() << "New Updates Found!";
                }
                else
                    qDebug() << "UPDATE CHECK: No New Updates...";
            }
            else
                qWarning() << "UPDATE CHECK: Error parsing Server response...";
        }
        else
            qWarning() << "UPDATE CHECK: Network Error...";
        onStart = false;
    }
    else
    {
        switch (reply->error()) {
        case QNetworkReply::NoError:
        {
            QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
            QJsonObject mainObj = document.object();

            QDateTime serverTime = QDateTime::fromString(mainObj["published_at"].toString(), Qt::ISODate);
            if(!serverTime.isValid())
            {
                QMessageBox::warning(mParent, tr("Error!"), tr("File on server could not be parsed..."));
                return;
            }

            QDateTime build = QDateTime::fromString("2023-06-06", "yyyy-MM-dd");

            QString info;
            if(serverTime.date() > build.date())
                info = QString(tr("New build %1 available!<br>Download <a href=\"https://github.com/3nock/SpiderSuite/releases\">here</a><br><br>You are now on build %2")).arg(serverTime.date().toString()).arg(build.date().toString());
            else if(serverTime.date() < build.date())
                info = QString(tr("You have a development build (%1) of SpiderSuite!")).arg(build.date().toString());
            else
                info = QString(tr("You have the latest build (%1) of SpiderSuite!")).arg(build.date().toString());

            QMessageBox::information(mParent, tr("Update Info"), info);
        }
            break;
        default:
            QMessageBox::warning(mParent, tr("Network Error!"), reply->errorString());
            break;
        }
    }
}

} // namespace OTE

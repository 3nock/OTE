//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef CORE_UTILITY_H
#define CORE_UTILITY_H

#include <stdlib.h>

#include <QBasicTimer>
#include <QNetworkReply>
#include <QIcon>
#include <QString>
#include <QStringList>
#include <QTimerEvent>

namespace OTE {

class ReplyTimeout : public QObject
{
    Q_OBJECT
public:
    ReplyTimeout(QNetworkReply* reply, int timeout) :
        QObject(reply)
    {
        Q_ASSERT(reply);
        if (reply && reply->isRunning())
            mTimer.start(timeout, this);
    }

    static void set(QNetworkReply* reply, int timeout){
        new ReplyTimeout(reply, timeout);
    }

protected:
    QBasicTimer mTimer;
    void timerEvent(QTimerEvent * ev)
    {
        if (!mTimer.isActive() || ev->timerId() != mTimer.timerId())
            return;

        QNetworkReply *reply = static_cast<QNetworkReply*>(parent());
        if (reply->isRunning())
        {
            reply->abort();
            mTimer.stop();
        }
    }
};

class Utility
{
public:
    /**
     * @brief Returns a value for the environment variable name passed;
     *        if the env var isn't set, it returns an empty string
     */
    static QString getEnvironmentVar(const QString &variable_name);

    /**
     * @brief GetApplicationIcon, returns the SpiderSuite app icon of various sizes, for linux
     * @return QIcon icon
     */
    static QIcon getApplicationIcon();
};

} // namespace OTE

#endif // CORE_UTILITY_H

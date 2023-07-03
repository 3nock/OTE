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

const QStringList gCOMMON_USERAGENTS {
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36 Edg/114.0.1823.58",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/16.5.1 Safari/605.1.1",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/114.0",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.3",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36 Edg/114.0.1823.51",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/16.5.1 Safari/605.1.15",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/15.6.6 Safari/605.1.1",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.3",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_6) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/15.6,2 Safari/605.1.1",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/113.0.0.0 Safari/537.36 OPR/99.0.0.",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/114."
};

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
     * @brief GetApplicationIcon, returns the OTE app icon of various sizes, for linux
     * @return QIcon icon
     */
    static QIcon getApplicationIcon();
};

} // namespace OTE

#endif // CORE_UTILITY_H

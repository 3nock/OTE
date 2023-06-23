//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef ENGINE_EXPLORERENGINE_H
#define ENGINE_EXPLORERENGINE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QQueue>
#include <QThread>

#include "src/template/Endpoint.h"
#include "src/template/Template.h"

namespace OTE {

namespace Engine {

class Explorer: public QObject
{
    Q_OBJECT

public:
    Explorer();
    ~Explorer();

    void start(Endpoint *endPoint);
    void start(Endpoint *endPoint, const QStringList &targets);

signals:
    void next();
    void finished();

    void result(const QString &tmplt,
                const QString &endPoint,
                const QString &target);

    void error(const QString &link, const QString &error);

public slots:
    void onStop();
    void onQuit();

    void onLookup();
    void onLookupFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *mManager;
    Endpoint *mEndpoint = nullptr;
    QQueue<QString> mTargetQueue{};
    QString mTarget{};
    int mActiveRequests = 0;

    QNetworkReply *mCurrentReply = nullptr;
};

} // namespace Engine

} // namespace OTE

#endif // ENGINE_EXPLORERENGINE_H

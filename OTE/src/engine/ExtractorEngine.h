//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef ENGINE_EXTRACTORENGINE_H
#define ENGINE_EXTRACTORENGINE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QQueue>
#include <QThread>

#include "src/template/Endpoint.h"
#include "src/template/Extractor.h"
#include "src/template/Template.h"

namespace OTE {

namespace Engine {

struct Requester {
    Endpoint *endpoint;
    Extractor *extractor;
};

class Extractor: public QObject
{
    Q_OBJECT

public:
    explicit Extractor(const QList<OTE::Endpoint*> &endpoints,
                       const QString &input,
                       const QString &output,
                       const QStringList &targets);
    ~Extractor();

signals:
    void next();
    void quitThread();

    void result(const QString &outputType,
                const QStringList &results);

    void error(const QString &link, const QString &error);

public slots:
    void onStart();
    void onStop();
    void onQuit();

    void onLookup();
    void onLookupFinished(QNetworkReply *reply);

private:
    QString mInputType;
    QString mOutputType;
    QNetworkAccessManager *mManager = nullptr;
    QQueue<QString> mTargetQueue{};
    QString mTarget;
    int mActiveRequests = 0;
    int mRequesterToUse = 0;
    QList<Requester> mRequesters;
};

} // namespace Engine

} // namespace OTE

#endif // ENGINE_EXTRACTORENGINE_H

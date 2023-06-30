//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "ExtractorEngine.h"

#include "src/core/Config.h"
#include "src/core/Database.h"
#include "src/core/Utility.h"

namespace OTE {

namespace Engine {

Extractor::Extractor(const QList<OTE::Endpoint*> &endpoints, const QString &input, const QString &output, const QStringList &targets):
    mInputType(input),
    mOutputType(output)
{
    connect(this, &Extractor::next, this, &Extractor::onLookup);

    // targets
    mTargetQueue << targets;

    mTarget = mTargetQueue.dequeue();

    // get all requesters
    foreach(OTE::Endpoint *endpoint, endpoints)
    {
        if(!endpoint->inputTypes.contains(input))
            continue;

        foreach(OTE::Extractor *extractor, endpoint->extractors)
        {
            if(extractor->name.compare(output, Qt::CaseInsensitive) != 0)
                continue;

            Requester requester;
            requester.endpoint = endpoint;
            requester.extractor = extractor;
            mRequesters << requester;
        }
    }
}

Extractor::~Extractor()
{
    delete mManager;
}

void Extractor::onStart()
{
    mManager = new QNetworkAccessManager;
    connect(mManager, &QNetworkAccessManager::finished, this, &Extractor::onLookupFinished);

    onLookup();
}

void Extractor::onStop()
{
    onQuit();
}

void Extractor::onQuit()
{
    if(mActiveRequests)
        return;
    else
        emit quitThread();
}

void Extractor::onLookup()
{
    if(mRequesterToUse >= mRequesters.count())
    {
        mRequesterToUse = 0;
        if(mTargetQueue.isEmpty())
        {
            onQuit();
            return;
        }

        mTarget = mTargetQueue.dequeue();
    }

    auto requester = mRequesters[mRequesterToUse];
    mRequesterToUse++;

    QString link(requester.endpoint->link);
    link.replace("{{input}}", mTarget);

    if(requester.endpoint->tmplt->authentication.authType == Template::AUTH_TYPE::QUERY)
    {
        if(requester.endpoint->tmplt->authentication.uses_id)
            link.replace("{{user}}", requester.endpoint->tmplt->authentication.id);
        if(requester.endpoint->tmplt->authentication.uses_key)
            link.replace("{{key}}", requester.endpoint->tmplt->authentication.key);
    }

    QUrl url(link);
    if(!url.isValid() || url.isRelative())
    {
        emit next();
        return;
    }

    QNetworkRequest request;
    request.setUrl(link);

    if(gConfig.engine.user_agent.use)
    {
        if(gConfig.engine.user_agent.oteUA)
            request.setRawHeader("User-Agent", "OTE/1.0.0");
        else if(gConfig.engine.user_agent.randomUA)
            gCOMMON_USERAGENTS.at(rand() % gCOMMON_USERAGENTS.size());
    }

    if(gConfig.engine.accept.use)
    {
        if(gConfig.engine.accept.jsonXml)
        {
            if(requester.endpoint->responseType == Endpoint::RESPONSE_TYPE::JSON)
                request.setRawHeader("Accept", "application/json");
            else if(requester.endpoint->responseType == Endpoint::RESPONSE_TYPE::XML)
                request.setRawHeader("Accept", "application/xml");
        }
        else if(gConfig.engine.accept.all)
            request.setRawHeader("Accept", "*/*");
    }

    if(requester.endpoint->tmplt->authentication.authType == Template::AUTH_TYPE::HEADER)
    {
        request.setRawHeader(requester.endpoint->tmplt->authentication.authHeader.toUtf8(),
                             requester.endpoint->tmplt->authentication.key.toUtf8());

    }

    if(requester.endpoint->tmplt->authentication.authType == Template::AUTH_TYPE::BASIC)
    {
        QString credentials = QString("%1:%2").arg(requester.endpoint->tmplt->authentication.id).arg(requester.endpoint->tmplt->authentication.key);
        QByteArray base64 = credentials.toLocal8Bit().toBase64();
        QString value = "Basic " + base64;
        request.setRawHeader("Authorization", value.toLocal8Bit());
    }

    if(requester.endpoint->tmplt->authentication.authType == Template::AUTH_TYPE::BEARER)
    {
        request.setRawHeader("Authorization", "Bearer "+requester.endpoint->tmplt->authentication.key.toUtf8());
    }

    if(requester.endpoint->requestType == Endpoint::REQUEST_TYPE::GET)
    {
        QNetworkReply* reply = mManager->get(request);
        mActiveRequests++;

        reply->setProperty("endpoint", requester.endpoint->name);
        reply->setProperty("extractor", requester.extractor->name);

        if(gConfig.engine.timeout.use)
            ReplyTimeout::set(reply, gConfig.engine.timeout.msec);
    }
    else if(requester.endpoint->requestType == Endpoint::REQUEST_TYPE::POST)
    {

        QString data(requester.endpoint->postData);
        data.replace("{{input}}", mTarget);

        if(requester.endpoint->tmplt->authentication.authType == Template::AUTH_TYPE::QUERY)
        {
            if(requester.endpoint->tmplt->authentication.uses_id)
                data.replace("{{user}}", requester.endpoint->tmplt->authentication.id);
            if(requester.endpoint->tmplt->authentication.uses_key)
                data.replace("{{key}}", requester.endpoint->tmplt->authentication.key);
        }

        QNetworkReply* reply = mManager->post(request, data.toUtf8());
        mActiveRequests++;

        reply->setProperty("endpoint", requester.endpoint->name);
        reply->setProperty("extractor", requester.extractor->name);

        if(gConfig.engine.timeout.use)
            ReplyTimeout::set(reply, gConfig.engine.timeout.msec);
    }
    else
        emit next();
}

void Extractor::onLookupFinished(QNetworkReply *reply)
{
    mActiveRequests--;

    if(reply->error() == QNetworkReply::NoError)
    {
        QString endpoint = reply->property("endpoint").toString();
        QString extractor = reply->property("extractor").toString();

        QByteArray data = reply->readAll();
        QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toByteArray();

        foreach(Requester requester, mRequesters)
        {
            if(requester.endpoint->name == endpoint &&
               requester.extractor->name == extractor)
            {
                QStringList extract = requester.extractor->extract(data);
                if(!extract.isEmpty())
                    emit result(mOutputType, Database::insertExtractorResult(mOutputType, extract));
            }
        }
    }
    else
    {
        emit error(reply->url().toString(), reply->errorString());
    }

    reply->deleteLater();

    emit next();
}

} // namespace Engine

} // namespace OTE

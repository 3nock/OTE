//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "ExplorerEngine.h"

#include "src/core/Config.h"
#include "src/core/Database.h"
#include "src/core/Utility.h"

namespace OTE {

namespace Engine {

Explorer::Explorer():
    mManager(new QNetworkAccessManager)
{
    connect(this, &Explorer::next, this, &Explorer::onLookup);
    connect(mManager, &QNetworkAccessManager::finished, this, &Explorer::onLookupFinished);
}

Explorer::~Explorer()
{
    delete mManager;
}

void Explorer::start(Endpoint *endPoint)
{
    mEndpoint = endPoint;

    mTargetQueue.clear();
    mTargetQueue << "NoTarget";

    onLookup();
}

void Explorer::start(Endpoint *endPoint, const QStringList &targets)
{
    mEndpoint = endPoint;

    mTargetQueue.clear();
    mTargetQueue << targets;

    onLookup();
}

void Explorer::onStop()
{
    // TODO: check this later, as it may cause problems
    if(mCurrentReply){
        if(!mCurrentReply->isFinished())
            mCurrentReply->abort();
    }
    onQuit();
}

void Explorer::onQuit()
{
    if(mActiveRequests)
        return;
    else
        emit finished();
}

void Explorer::onLookup()
{
    if(mTargetQueue.isEmpty())
    {
        onQuit();
        return;
    }

    mTarget = mTargetQueue.dequeue();

    QString link(mEndpoint->link);
    link.replace("{{input}}", mTarget);

    if(mEndpoint->tmplt->authentication.authType == Template::AUTH_TYPE::QUERY)
    {
        if(mEndpoint->tmplt->authentication.uses_id)
            link.replace("{{user}}", mEndpoint->tmplt->authentication.id);
        if(mEndpoint->tmplt->authentication.uses_key)
            link.replace("{{key}}", mEndpoint->tmplt->authentication.key);
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
            request.setRawHeader("User-Agent", "Mozilla/1.0.0");
    }

    if(gConfig.engine.accept.use)
    {
        if(gConfig.engine.accept.jsonXml)
        {
            if(mEndpoint->responseType == Endpoint::RESPONSE_TYPE::JSON)
                request.setRawHeader("Accept", "application/json");
            else if(mEndpoint->responseType == Endpoint::RESPONSE_TYPE::XML)
                request.setRawHeader("Accept", "application/xml");
        }
        else if(gConfig.engine.accept.all)
            request.setRawHeader("Accept", "*/*");
    }

    // set authentication tokens
    switch (mEndpoint->tmplt->authentication.authType) {
    case Template::AUTH_TYPE::BASIC:
    {
        QString credentials = QString("%1:%2").arg(mEndpoint->tmplt->authentication.id).arg(mEndpoint->tmplt->authentication.key);
        QByteArray base64 = credentials.toLocal8Bit().toBase64();
        QString value = "Basic " + base64;
        request.setRawHeader("Authorization", value.toLocal8Bit());
        break;
    }
    case Template::AUTH_TYPE::BEARER:
    {
        request.setRawHeader("Authorization", "Bearer "+mEndpoint->tmplt->authentication.key.toUtf8());
        break;
    }
    case Template::AUTH_TYPE::HEADER:
    {
        request.setRawHeader(mEndpoint->tmplt->authentication.authHeader.toUtf8(),
                             mEndpoint->tmplt->authentication.key.toUtf8());
        break;
    }
    default:
        break;
    }

    // send request
    if(mEndpoint->requestType == Endpoint::REQUEST_TYPE::GET)
    {
        mCurrentReply = mManager->get(request);
        mActiveRequests++;

        if(gConfig.engine.timeout.use)
            ReplyTimeout::set(mCurrentReply, gConfig.engine.timeout.msec);
    }
    else if(mEndpoint->requestType == Endpoint::REQUEST_TYPE::POST)
    {

        QString data(mEndpoint->postData);
        data.replace("{{input}}", mTarget);

        if(mEndpoint->tmplt->authentication.authType == Template::AUTH_TYPE::QUERY)
        {
            if(mEndpoint->tmplt->authentication.uses_id)
                data.replace("{{user}}", mEndpoint->tmplt->authentication.id);
            if(mEndpoint->tmplt->authentication.uses_key)
                data.replace("{{key}}", mEndpoint->tmplt->authentication.key);
        }

        mCurrentReply = mManager->post(request, data.toUtf8());
        mActiveRequests++;

        if(gConfig.engine.timeout.use)
            ReplyTimeout::set(mCurrentReply, gConfig.engine.timeout.msec);
    }
    else
        emit next();
}

void Explorer::onLookupFinished(QNetworkReply *reply)
{
    mActiveRequests--;

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        if(!data.isEmpty())
        {
            if(Database::insertExplorerResult(mEndpoint->tmplt->info.name, mEndpoint->name, mTarget, data))
                emit result(mEndpoint->tmplt->info.name, mEndpoint->name, mTarget);
        }
    }
    else
    {
        emit error(reply->url().toString(), reply->errorString());
    }

    reply->deleteLater();
    mCurrentReply = nullptr;
    emit next();
}

} // namespace Engine

} // namespace OTE

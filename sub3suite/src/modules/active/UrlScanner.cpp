/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "UrlScanner.h"

#include <QNetworkReply>


url::Scanner::Scanner(url::ScanArgs *args): AbstractScanner(nullptr),
      m_args(args)
{
    m_manager = new NetworkAccessManager(this, m_args->config->timeout);

    connect(m_manager, &QNetworkAccessManager::finished, this, &url::Scanner::lookupFinished);
    connect(this, &url::Scanner::next, this, &url::Scanner::lookup);
}
url::Scanner::~Scanner(){
    delete m_manager;
}

void url::Scanner::lookupFinished(QNetworkReply *reply){
    switch (reply->error()) {
    case QNetworkReply::OperationCanceledError:
    {
        scan::Log log;
        log.target = reply->url().toString();
        log.message = "Operation Cancelled due to Timeout";
        emit scanLog(log);
    }
        break;
    case QNetworkReply::NoError:
    {
        s3s_struct::URL url;
        url.url = reply->url().toString();
        url.status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        url.banner = reply->header(QNetworkRequest::ServerHeader).toString();
        url.content_type = reply->header(QNetworkRequest::ContentTypeHeader).toString();
        emit scanResult(url);
    }
        break;
    default:
    {
        scan::Log log;
        log.target = reply->url().toString();
        log.message = reply->errorString();
        emit scanLog(log);
    }
        break;
    }

    m_args->progress++;
    emit scanProgress(m_args->progress);
    emit next();
}

void url::Scanner::lookup(){
    QNetworkRequest request;
    QUrl url;

    switch (url::getTarget(m_args, url)) {
    case RETVAL::LOOKUP:
        request.setUrl(url);
        m_manager->get(request);
        break;
    case RETVAL::QUIT:
        emit quitThread();
        break;
    default:
        emit quitThread();
    }
}

RETVAL url::getTarget(url::ScanArgs *args, QUrl &url){
    /* lock */
    QMutexLocker(&args->mutex);

    if(!args->targets.isEmpty()){
         url.setUrl(args->targets.dequeue());
         return RETVAL::LOOKUP;
    }
    else
        return RETVAL::QUIT;
}

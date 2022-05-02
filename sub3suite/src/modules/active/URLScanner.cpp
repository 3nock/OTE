/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "URLScanner.h"

#include <QStack>
#include <QNetworkReply>

#include "gumbo-parser/src/gumbo.h"

/*
 * TODO:
 *      pull ssl certificate from the url
 *      reply->sslConfiguration().peerCertificate();
 */
url::Scanner::Scanner(url::ScanArgs *args): AbstractScanner(nullptr),
      m_args(args)
{
    m_manager = new url::NetworkAccessManager(this, m_args->config->timeout);

    connect(m_manager, &url::NetworkAccessManager::finished, this, &url::Scanner::lookupFinished);
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

        if(m_args->config->get_title)
            this->get_title(url, reply);

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

    reply->deleteLater();

    emit next();
}

void url::Scanner::get_title(s3s_struct::URL &url_struct, QNetworkReply *reply){
    GumboOutput *output = gumbo_parse(reply->readAll());
    GumboNode *root = output->root;

    for(unsigned int i = 0; i < root->v.element.children.length; i++)
    {
        GumboNode *head_node = static_cast<GumboNode*>(root->v.element.children.data[i]);
        if(head_node->type == GUMBO_NODE_ELEMENT && head_node->v.element.tag == GUMBO_TAG_HEAD){
            QStack<GumboNode*> node_stack;
            node_stack.push(head_node);

            GumboNode *node;
            while(!node_stack.isEmpty()) // backtracking loop...
            {
                node = node_stack.pop();
                if (node->type != GUMBO_NODE_ELEMENT)
                    continue;

                if (node->v.element.tag == GUMBO_TAG_TITLE){
                    GumboNode* title_text = static_cast<GumboNode*>(node->v.element.children.data[0]);
                    url_struct.title = title_text->v.text.text;
                    break;
                }

                GumboVector *children = &node->v.element.children;
                for(unsigned int i = 0; i < children->length; i++)
                    node_stack.push(static_cast<GumboNode*>(children->data[i]));
            }
            break;
        }
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);
}

void url::Scanner::lookup(){
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.83 Safari/537.36");
    if(m_args->config->follow_redirect)
        request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QUrl url;

    switch (url::getTarget(m_args, url)) {
    case RETVAL::LOOKUP:
        /* checks */
        if(url.isRelative())
            url.setUrl(m_args->config->scheme+"://"+url.url());
        if(!url.isValid())
            emit next();

        /* scheme change */
        if(m_args->config->force_scheme)
            url.setScheme(m_args->config->scheme);

        /* lookup */
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

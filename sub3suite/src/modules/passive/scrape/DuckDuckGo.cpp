#include "DuckDuckGo.h"
#include <QStack>


/*
 * find a way to get to next page...
 */
DuckDuckGo::DuckDuckGo(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_DUCKDUCKGO;

    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DuckDuckGo::replyFinishedSubdomain);
    if(args.output_Email)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DuckDuckGo::replyFinishedEmail);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DuckDuckGo::replyFinishedUrl);
}
DuckDuckGo::~DuckDuckGo(){
    delete manager;
}

void DuckDuckGo::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.input_Domain){
        if(args.output_Hostname){
            url.setUrl("https://html.duckduckgo.com/html/?q=site:"+target);
            request.setUrl(url);
            manager->get(request);
            return;
        }

        if(args.output_URL){
            url.setUrl("https://html.duckduckgo.com/html/?q=site:"+target);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void DuckDuckGo::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty())
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length == 2 && node->v.element.children.length == 1)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "result__url")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                QString domain = QString::fromUtf8(child->v.text.text);
                emit resultSubdomain(domain.split("/")[0].trimmed());
                log.resultsCount++;
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    this->end(reply);
}

void DuckDuckGo::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    /*
     * not yet implemented...
     */

    this->end(reply);
}

void DuckDuckGo::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty())
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length == 2 && node->v.element.children.length == 1)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "result__url")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                emit resultURL(QString::fromUtf8(child->v.text.text).trimmed());
                log.resultsCount++;
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    this->end(reply);
}

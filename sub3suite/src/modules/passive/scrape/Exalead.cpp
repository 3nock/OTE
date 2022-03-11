#include "Exalead.h"
#include <QStack>


/*
 * not very accurate...
 */
Exalead::Exalead(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = "Exalead";

    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Exalead::replyFinishedSubdomain);
    if(args.outputEmail)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Exalead::replyFinishedEmail);
    if(args.outputUrl)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Exalead::replyFinishedUrl);
}
Exalead::~Exalead(){
    delete manager;
}

void Exalead::start(){
    QNetworkRequest request;

    if(args.inputDomain){
        if(args.outputSubdomain){
            QUrl url("https://www.exalead.com/search/web/results/?q="+target+"&collapsing=off");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.outputUrl){
            QUrl url("https://www.exalead.com/search/web/results/?q="+target+"&collapsing=off");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void Exalead::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(this->getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty())
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length == 2 && node->v.element.children.length == 1)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "ellipsis")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                emit resultSubdomain(QString::fromUtf8(child->v.text.text));
                log.resultsCount++;
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    end(reply);
}

void Exalead::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    /*
     * not yet implemented...
     */

    end(reply);
}

void Exalead::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(this->getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty())
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length == 2 && node->v.element.children.length == 1)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "ellipsis")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                emit resultUrl(QString::fromUtf8(child->v.text.text));
                log.resultsCount++;
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    end(reply);
}

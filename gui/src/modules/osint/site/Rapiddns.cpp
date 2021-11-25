#include "Rapiddns.h"
#include <QStack>


/*
 * implement for subdomainIp & ip
 * it is one of my fav
 */
Rapiddns::Rapiddns(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "RapidDns";

    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Rapiddns::replyFinishedSubdomain);
    if(args->outputSubdomainIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Rapiddns::replyFinishedSubdomainIp);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Rapiddns::replyFinishedIp);
}
Rapiddns::~Rapiddns(){
    delete manager;
}

void Rapiddns::start(){
    QNetworkRequest request;
    QUrl url;

    if(args->inputDomain){
        url.setUrl("https://rapiddns.io/subdomain/"+args->target+"?full=1#result");
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args->inputIp){
        url.setUrl("https://rapiddns.io/sameip/"+args->target+"?full=1#result");
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }

    if(args->inputCidr){
        url.setUrl("https://rapiddns.io/s/"+args->target+"?full=1#result");
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Rapiddns::replyFinishedSubdomain(QNetworkReply *reply){
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
        if(node->type != GUMBO_NODE_ELEMENT || node->v.element.tag == GUMBO_TAG_SCRIPT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_TBODY && node->v.element.children.length > 0){
            GumboVector tbodyChildren = node->v.element.children;
            for(unsigned int i = 0; i < tbodyChildren.length; i++){
                GumboNode *trNode = static_cast<GumboNode*>(tbodyChildren.data[i]);
                if(trNode->type == GUMBO_NODE_ELEMENT && trNode->v.element.tag == GUMBO_TAG_TR && trNode->v.element.children.length > 1){
                    GumboNode *td = static_cast<GumboNode*>(trNode->v.element.children.data[3]);
                    if(td->type == GUMBO_NODE_ELEMENT && td->v.element.tag == GUMBO_TAG_TD && td->v.element.children.length >0){
                        GumboNode *item = static_cast<GumboNode*>(td->v.element.children.data[0]);
                        if(item->type == GUMBO_NODE_TEXT)
                        {
                            emit subdomain(QString::fromUtf8(item->v.text.text));
                            log.resultsCount++;
                        }
                    }
                }
            }
            continue;
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);
    end(reply);
}

void Rapiddns::replyFinishedSubdomainIp(QNetworkReply *reply){
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
        if(node->type != GUMBO_NODE_ELEMENT || node->v.element.tag == GUMBO_TAG_SCRIPT)
            continue;

        /* not yet implemented... */

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);
    end(reply);
}

void Rapiddns::replyFinishedIp(QNetworkReply *reply){
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
        if(node->type != GUMBO_NODE_ELEMENT || node->v.element.tag == GUMBO_TAG_SCRIPT)
            continue;

        /* not yet implemented... */

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);
    end(reply);
}

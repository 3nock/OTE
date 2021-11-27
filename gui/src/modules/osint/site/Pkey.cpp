#include "Pkey.h"
#include <QStack>


/*
 * fix the parser...
 * has subdomains&ip...
 */
Pkey::Pkey(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "PKey";

    if(args->outputSubdomainIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Pkey::replyFinishedSubdomainIp);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Pkey::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &Pkey::replyFinishedIp);
}
Pkey::~Pkey(){
    delete manager;
}

void Pkey::start(){
    QNetworkRequest request;

    QUrl url("https://www.pkey.in/tools-i/search-subdomains");
    request.setUrl(url);
    request.setRawHeader("Origin", "https://www.pkey.in");
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    //...
    QByteArray data;
    data.append("zone="+args->target+"&");
    data.append("submit=");

    /* send request */
    manager->post(request, data);
    activeRequests++;
}

void Pkey::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(this->getBody(output->root));

    bool isFirstMatch = true; // ignore the first-match which is a heading value...
    GumboNode *node;
    while(!nodes.isEmpty())
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT || node->v.element.tag == GUMBO_TAG_SCRIPT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_TD && node->v.element.attributes.length == 1 && node->v.element.children.length == 1){
            GumboAttribute *style = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(style->value) == "border-left-style: none;")
            {
                if(isFirstMatch){
                    isFirstMatch = false;
                    continue;
                }
                GumboNode *domain = static_cast<GumboNode*>(node->v.element.children.data[0]);
                emit subdomain(QString::fromUtf8(domain->v.text.text));
                log.resultsCount++;
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

void Pkey::replyFinishedSubdomainIp(QNetworkReply *reply){
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

void Pkey::replyFinishedIp(QNetworkReply *reply){
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

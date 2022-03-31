#include "DogPile.h"
#include <QStack>

DogPile::DogPile(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_DOGPILE;

    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DogPile::replyFinishedSubdomain);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DogPile::replyFinishedUrl);
}
DogPile::~DogPile(){
    delete manager;
}

void DogPile::start(){
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.83 Safari/537.36");
    QUrl url;

    if(args.input_Domain){
        if(args.output_Hostname || args.output_URL){
            url.setUrl("https://www.dogpile.com/serp?q="+target);
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_Search){
        if(args.output_Hostname || args.output_URL){
            url.setUrl("https://www.dogpile.com/serp?q=*"+target+"*");
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void DogPile::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(GumboMethods::getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty())
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length > 4)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "web-bing__title")
            {
                GumboAttribute *hrefAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[1]);
                QString url(hrefAttribute->value);
                url = url.remove("http://").remove("https://");
                emit resultSubdomain(url.split("/")[0]);
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

void DogPile::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(GumboMethods::getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty())
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length > 4)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "web-bing__title")
            {
                GumboAttribute *hrefAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[1]);
                emit resultSubdomain(hrefAttribute->value);
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

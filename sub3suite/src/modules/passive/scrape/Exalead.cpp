#include "Exalead.h"
#include <QStack>

/*
 * not very accurate...
 */
Exalead::Exalead(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_EXALEAD;

    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Exalead::replyFinishedSubdomain);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Exalead::replyFinishedUrl);
}
Exalead::~Exalead(){
    delete manager;
}

void Exalead::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.input_Domain){
        if(args.output_Hostname || args.output_URL){
            url.setUrl("https://www.exalead.com/search/web/results/?q="+target+"&collapsing=off");
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_Search){
        if(args.output_Hostname || args.output_URL){
            url.setUrl("https://www.exalead.com/search/web/results/?q="+target+"&collapsing=off");
            request.setUrl(url);
            manager->get(request);
            return;
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
    nodes.push(GumboMethods::getBody(output->root));

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
                QString url(child->v.text.text);
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

void Exalead::replyFinishedUrl(QNetworkReply *reply){
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

        if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length == 2 && node->v.element.children.length == 1)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "ellipsis")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                emit resultURL(child->v.text.text);
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

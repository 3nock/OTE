#include "Ask.h"
#include <QStack>


Ask::Ask(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_ASK;

    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Ask::replyFinishedSubdomain);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Ask::replyFinishedUrl);
}
Ask::~Ask(){
    delete manager;
}

void Ask::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.input_Domain){
        if(args.output_Hostname){
            url.setUrl("https://www.ask.com/web?q=site:*"+target+"&ad=dirN&o=0&qo=pagination&page="+QString::number(m_page));
            request.setUrl(url);
            manager->get(request);
            return;
        }
        if(args.output_URL){
            url.setUrl("https://www.ask.com/web?q=site:"+target+"/*&ad=dirN&o=0&qo=pagination&page="+QString::number(m_page));
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_Search){
        if(args.output_Hostname || args.output_URL){
            url.setUrl("https://www.ask.com/web?q=site:*"+target+".*+OR+site:*"+target+".*.*&ad=dirN&o=0&qo=pagination&page="+QString::number(m_page));
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void Ask::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty()) // backtrack loop
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 1 && node->v.element.children.length == 1)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "PartialSearchResults-item-url")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                QString domain(child->v.text.text);
                domain = domain.split("/")[0];
                emit resultSubdomain(domain);
                log.resultsCount++;

                /* avoid unwanted links */
                if(log.resultsCount == 10)
                    break;
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    this->end(reply);
}

void Ask::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty()) // backtrack loop
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 1 && node->v.element.children.length == 1)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "PartialSearchResults-item-url")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                emit resultURL(QString::fromUtf8(child->v.text.text));
                log.resultsCount++;

                /* avoid unwanted links */
                if(log.resultsCount == 10)
                    break;
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    this->end(reply);
}

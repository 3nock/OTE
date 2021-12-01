#include "Ask.h"
#include <QStack>


Ask::Ask(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Ask";

    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Ask::replyFinishedSubdomain);
    if(args->outputUrl)
        connect(manager, &MyNetworkAccessManager::finished, this, &Ask::replyFinishedUrl);
}
Ask::~Ask(){
    delete manager;
}

void Ask::start(){
    QNetworkRequest request;
    while(m_page < 10){
        m_page++;
        QUrl url("https://www.ask.com/web?q=site:"+args->target+"&page="+QString::number(m_page)+"&qid=8D6EE6BF52E0C04527E51A64F22C4534&o=0&l=dir&qsrc=998&qo=pagination");
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Ask::replyFinishedSubdomain(QNetworkReply *reply){
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

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 1 && node->v.element.children.length > 0)
        {
            GumboAttribute *a = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            QString name = QString::fromUtf8(a->name);
            QString value = QString::fromUtf8(a->value);
            if(name == "class" && value == "PartialSearchResults-item-url")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                if(child->type == GUMBO_NODE_TEXT){
                    QString item = QString::fromUtf8(child->v.text.text);
                    item = item.split("/")[0];
                    emit subdomain(item);
                    log.resultsCount++;
                }
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);
    end(reply);
}

void Ask::replyFinishedUrl(QNetworkReply *reply){
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

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 1 && node->v.element.children.length > 0)
        {
            GumboAttribute *a = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            QString name = QString::fromUtf8(a->name);
            QString value = QString::fromUtf8(a->value);
            if(name == "class" && value == "PartialSearchResults-item-url")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                if(child->type == GUMBO_NODE_TEXT){
                    QString item = QString::fromUtf8(child->v.text.text);
                    item = item.split("/")[0];
                    emit subdomain(item);
                    log.resultsCount++;
                }
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);
    end(reply);
}

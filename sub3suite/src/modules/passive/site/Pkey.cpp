#include "Pkey.h"
#include <QStack>


Pkey::Pkey(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = "PKey";

    if(args.outputSubdomainIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Pkey::replyFinishedSubdomainIp);
    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Pkey::replyFinishedSubdomain);
    if(args.outputIp)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Pkey::replyFinishedIp);
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
    data.append("zone="+target+"&");
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
                GumboNode *domain = static_cast<GumboNode*>(node->v.element.children.data[0]);
                QString hostname = domain->v.text.text;
                if(hostname.endsWith("."))
                    hostname.chop(1);
                if(hostname.contains(".")){
                    emit resultSubdomain(hostname);
                    log.resultsCount++;
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

void Pkey::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(this->getBody(output->root));

    QString hostname;
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
                GumboNode *domain = static_cast<GumboNode*>(node->v.element.children.data[0]);

                hostname = domain->v.text.text;
                if(hostname.endsWith("."))
                    hostname.chop(1);
                if(hostname.contains("."))
                {
                    GumboNode *tr = node->parent;
                    GumboNode *td_type = static_cast<GumboNode*>(tr->v.element.children.data[5]);
                    if(td_type->v.element.tag == GUMBO_TAG_TD && td_type->v.element.attributes.length == 0 && td_type->v.element.children.length == 1)
                    {
                        GumboNode *type = static_cast<GumboNode*>(td_type->v.element.children.data[0]);
                        if(QString::fromUtf8(type->v.text.text) == "A" || QString::fromUtf8(type->v.text.text) == "AAAA")
                        {
                            GumboNode *td_value = static_cast<GumboNode*>(tr->v.element.children.data[7]);
                            if(td_type->v.element.tag == GUMBO_TAG_TD && td_value->v.element.attributes.length == 1 && td_value->v.element.children.length == 1){
                                GumboNode *value = static_cast<GumboNode*>(td_value->v.element.children.data[0]);
                                QString address = value->v.text.text;
                                emit resultSubdomainIp(hostname, address);
                                log.resultsCount++;
                            }
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

        if(node->v.element.tag == GUMBO_TAG_TD && node->v.element.attributes.length == 1 && node->v.element.children.length == 1){
            GumboAttribute *style = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(style->value) == "border-left-style: none;")
            {
                GumboNode *tr = node->parent;
                GumboNode *td_type = static_cast<GumboNode*>(tr->v.element.children.data[5]);
                if(td_type->v.element.tag == GUMBO_TAG_TD && td_type->v.element.attributes.length == 0 && td_type->v.element.children.length == 1)
                {
                    GumboNode *type = static_cast<GumboNode*>(td_type->v.element.children.data[0]);
                    if(QString::fromUtf8(type->v.text.text) == "A" || QString::fromUtf8(type->v.text.text) == "AAAA")
                    {
                        GumboNode *td_value = static_cast<GumboNode*>(tr->v.element.children.data[7]);
                        if(td_type->v.element.tag == GUMBO_TAG_TD && td_value->v.element.attributes.length == 1 && td_value->v.element.children.length == 1){
                            GumboNode *value = static_cast<GumboNode*>(td_value->v.element.children.data[0]);
                            QString address = value->v.text.text;
                            emit resultIP(address);
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

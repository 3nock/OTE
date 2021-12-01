#include "PagesInventory.h"
#include <QStack>


/*
 * next page implementation not yet...
 * also has a reverse-ip...
 */
PagesInventory::PagesInventory(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "PagesInventory";

    if(args->outputSubdomainIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &PagesInventory::replyFinishedSubdomainIp);
    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &PagesInventory::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &MyNetworkAccessManager::finished, this, &PagesInventory::replyFinishedIp);
}
PagesInventory::~PagesInventory(){
    delete manager;
}

void PagesInventory::start(){
    QNetworkRequest request;
    QUrl url("https://www.pagesinventory.com/search/?s="+args->target);
    request.setUrl(url);
    manager->get(request);
    activeRequests++;
}

void PagesInventory::replyFinishedSubdomainIp(QNetworkReply *reply){
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

        if(node->v.element.tag == GUMBO_TAG_TR && node->v.element.attributes.length == 1 && node->v.element.children.length > 3)
        {
            GumboAttribute *attribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            QString attrValue = QString::fromUtf8(attribute->value);
            if(attrValue == "sude")
            {
                QString domainName;
                /* subdomain */
                GumboNode *tdSubdomain = static_cast<GumboNode*>(node->v.element.children.data[0]);
                if(tdSubdomain->type == GUMBO_NODE_ELEMENT && tdSubdomain->v.element.children.length > 0){
                    GumboNode *a = static_cast<GumboNode*>(tdSubdomain->v.element.children.data[0]);
                    if(a->type == GUMBO_NODE_ELEMENT && a->v.element.children.length > 0){
                        GumboNode *domain = static_cast<GumboNode*>(a->v.element.children.data[0]);
                        if(domain->type == GUMBO_NODE_TEXT)
                            domainName = QString::fromUtf8(domain->v.text.text);
                    }
                }
                /* ipv4... */
                GumboNode *tdIpV4 = static_cast<GumboNode*>(node->v.element.children.data[4]);
                if(tdIpV4->type == GUMBO_NODE_ELEMENT && tdIpV4->v.element.children.length > 0){
                    GumboNode *a = static_cast<GumboNode*>(tdIpV4->v.element.children.data[0]);
                    if(a->type == GUMBO_NODE_ELEMENT && a->v.element.children.length > 0){
                        GumboNode *ipv4 = static_cast<GumboNode*>(a->v.element.children.data[0]);
                        if(ipv4->type == GUMBO_NODE_TEXT)
                        {
                            QString address = QString::fromUtf8(ipv4->v.text.text);
                            emit subdomainIp(domainName, address);
                            log.resultsCount++;
                        }

                    }
                }
                /* ipv6... */
                GumboNode *tdIpv6 = static_cast<GumboNode*>(node->v.element.children.data[5]);
                if(tdIpv6->type == GUMBO_NODE_ELEMENT && tdIpv6->v.element.children.length > 0){
                    GumboNode *a = static_cast<GumboNode*>(tdIpv6->v.element.children.data[0]);
                    if(a->type == GUMBO_NODE_ELEMENT && a->v.element.children.length > 0){
                        GumboNode *ipv6 = static_cast<GumboNode*>(a->v.element.children.data[0]);
                        if(ipv6->type == GUMBO_NODE_TEXT)
                        {
                            QString address = QString::fromUtf8(ipv6->v.text.text);
                            emit subdomainIp(domainName, address);
                            log.resultsCount++;
                        }
                    }
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

void PagesInventory::replyFinishedIp(QNetworkReply *reply){
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

        if(node->v.element.tag == GUMBO_TAG_TR && node->v.element.attributes.length == 1 && node->v.element.children.length > 3)
        {
            GumboAttribute *attribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            QString attrValue = QString::fromUtf8(attribute->value);
            if(attrValue == "sude")
            {
                /* ipv4...*/
                GumboNode *tdIpV4 = static_cast<GumboNode*>(node->v.element.children.data[4]);
                if(tdIpV4->type == GUMBO_NODE_ELEMENT && tdIpV4->v.element.children.length > 0){
                    GumboNode *a = static_cast<GumboNode*>(tdIpV4->v.element.children.data[0]);
                    if(a->type == GUMBO_NODE_ELEMENT && a->v.element.children.length > 0){
                        GumboNode *ipv4 = static_cast<GumboNode*>(a->v.element.children.data[0]);
                        if(ipv4->type == GUMBO_NODE_TEXT)
                        {
                            QString address = QString::fromUtf8(ipv4->v.text.text);
                            emit ipA(address);
                            log.resultsCount++;
                        }

                    }
                }

                /* ipv6...*/
                GumboNode *tdIpv6 = static_cast<GumboNode*>(node->v.element.children.data[5]);
                if(tdIpv6->type == GUMBO_NODE_ELEMENT && tdIpv6->v.element.children.length > 0){
                    GumboNode *a = static_cast<GumboNode*>(tdIpv6->v.element.children.data[0]);
                    if(a->type == GUMBO_NODE_ELEMENT && a->v.element.children.length > 0){
                        GumboNode *ipv6 = static_cast<GumboNode*>(a->v.element.children.data[0]);
                        if(ipv6->type == GUMBO_NODE_TEXT)
                        {
                            QString address = QString::fromUtf8(ipv6->v.text.text);
                            emit ipAAAA(address);
                            log.resultsCount++;
                        }
                    }
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

void PagesInventory::replyFinishedSubdomain(QNetworkReply *reply){
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

        if(node->v.element.tag == GUMBO_TAG_TR && node->v.element.attributes.length == 1 && node->v.element.children.length > 3)
        {
            GumboAttribute *attribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            QString attrValue = QString::fromUtf8(attribute->value);
            if(attrValue == "sude")
            {
                GumboNode *tdSubdomain = static_cast<GumboNode*>(node->v.element.children.data[0]);
                if(tdSubdomain->type == GUMBO_NODE_ELEMENT && tdSubdomain->v.element.children.length > 0){
                    GumboNode *a = static_cast<GumboNode*>(tdSubdomain->v.element.children.data[0]);
                    if(a->type == GUMBO_NODE_ELEMENT && a->v.element.children.length > 0){
                        GumboNode *domain = static_cast<GumboNode*>(a->v.element.children.data[0]);
                        if(domain->type == GUMBO_NODE_TEXT)
                        {
                            QString domainName = QString::fromUtf8(domain->v.text.text);
                            emit subdomain(domainName);
                            log.resultsCount++;
                        }
                    }
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

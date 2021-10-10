#include "PagesInventory.h"

/*
 * search domain/ip
 *
 * results seperated by comma (,)
 */
PagesInventory::PagesInventory(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &PagesInventory::replyFinished);
}
PagesInventory::~PagesInventory(){
    delete manager;
}

void PagesInventory::start(){
    QNetworkRequest request;
    QUrl url("https://www.pagesinventory.com/search/?s="+args->target);
    request.setUrl(url);
    manager->get(request);
}

void PagesInventory::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        GumboOutput *output = gumbo_parse(reply->readAll());
        GumboNode *body = getBody(output->root);
        getSubdomains(body);
        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
    else
    {
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

///
/// next page implementation not yet...
///
void PagesInventory::getSubdomains(GumboNode *node){
    if(node->type != GUMBO_NODE_ELEMENT || node->v.element.tag == GUMBO_TAG_SCRIPT)
        return;

    if(node->v.element.tag == GUMBO_TAG_TR && node->v.element.attributes.length == 1 && node->v.element.children.length > 3)
    {
        GumboAttribute *attribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
        QString attrValue = QString::fromUtf8(attribute->value);
        if(attrValue == "sude"){
            QString results;
            ///
            /// subdomain...
            ///
            GumboNode *tdSubdomain = static_cast<GumboNode*>(node->v.element.children.data[0]);
            if(tdSubdomain->type == GUMBO_NODE_ELEMENT && tdSubdomain->v.element.children.length > 0){
                GumboNode *a = static_cast<GumboNode*>(tdSubdomain->v.element.children.data[0]);
                if(a->type == GUMBO_NODE_ELEMENT && a->v.element.children.length > 0){
                    GumboNode *subdomain = static_cast<GumboNode*>(a->v.element.children.data[0]);
                    if(subdomain->type == GUMBO_NODE_TEXT)
                        results.append(QString::fromUtf8(subdomain->v.text.text));
                }
            }
            ///
            /// ipv4...
            ///
            GumboNode *tdIpV4 = static_cast<GumboNode*>(node->v.element.children.data[4]);
            if(tdIpV4->type == GUMBO_NODE_ELEMENT && tdIpV4->v.element.children.length > 0){
                GumboNode *a = static_cast<GumboNode*>(tdIpV4->v.element.children.data[0]);
                if(a->type == GUMBO_NODE_ELEMENT && a->v.element.children.length > 0){
                    GumboNode *ipv4 = static_cast<GumboNode*>(a->v.element.children.data[0]);
                    if(ipv4->type == GUMBO_NODE_TEXT)
                        results.append(",").append(QString::fromUtf8(ipv4->v.text.text));

                }
            }
            ///
            /// ipv6
            ///
            GumboNode *tdIpv6 = static_cast<GumboNode*>(node->v.element.children.data[5]);
            if(tdIpv6->type == GUMBO_NODE_ELEMENT && tdIpv6->v.element.children.length > 0){
                GumboNode *a = static_cast<GumboNode*>(tdIpv6->v.element.children.data[0]);
                if(a->type == GUMBO_NODE_ELEMENT && a->v.element.children.length > 0){
                    GumboNode *ipv6 = static_cast<GumboNode*>(a->v.element.children.data[0]);
                    if(ipv6->type == GUMBO_NODE_TEXT)
                        results.append(",").append(QString::fromUtf8(ipv6->v.text.text));
                }
            }
            emit scanResults(results);
        }
        return;
    }

    GumboVector *children = &node->v.element.children;
    for(unsigned int i = 0; i < children->length; i++)
        getSubdomains(static_cast<GumboNode*>(children->data[i]));
    return;
}

GumboNode *PagesInventory::getBody(GumboNode *node){
    for(unsigned int i = 0; i < node->v.element.children.length; i++){
        GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[i]);
        if(child->type == GUMBO_NODE_ELEMENT && child->v.element.tag == GUMBO_TAG_BODY)
            return child;
    }
    return nullptr;
}

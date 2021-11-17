#include "Netcraft.h"


Netcraft::Netcraft(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    connect(manager, &MyNetworkAccessManager::finished, this, &Netcraft::replyFinished);
}
Netcraft::~Netcraft(){
    delete manager;
}

void Netcraft::start(){
    QNetworkRequest request;
    QUrl url("https://searchdns.netcraft.com/?restriction=site+ends+with&host="+args->target);
    request.setUrl(url);
    manager->get(request);
}

void Netcraft::replyFinished(QNetworkReply *reply){
    if(reply->error())
    {
        emit errorLog(reply->errorString());
    }
    else
    {
        GumboOutput *output = gumbo_parse(reply->readAll());
        getSubdomains(output->root);
        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
    reply->deleteLater();
    emit quitThread();
}

///
/// next page implementation not yet...
///
void Netcraft::getSubdomains(GumboNode *node){
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_TD && node->v.element.attributes.length == 1 && node->v.element.children.length > 1)
    {
        GumboNode *a = static_cast<GumboNode*>(node->v.element.children.data[0]);
        if(a->type == GUMBO_NODE_ELEMENT && a->v.element.tag == GUMBO_TAG_A && a->v.element.attributes.length > 1)
        {
            GumboAttribute *class_attribute = static_cast<GumboAttribute*>(a->v.element.attributes.data[0]);
            if(QString::fromUtf8(class_attribute->value) == "results-table__host")
            {
                GumboAttribute *href_attribute = static_cast<GumboAttribute*>(a->v.element.attributes.data[0]);
                QString item = QString::fromUtf8(href_attribute->value);
                item.remove("http://");
                item.remove("/");
                emit subdomain(item);
            }
        }
    }

    GumboVector *children = &node->v.element.children;
    for(unsigned int i = 0; i < children->length; i++)
        getSubdomains(static_cast<GumboNode*>(children->data[i]));
}

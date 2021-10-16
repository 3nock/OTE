#include "Rapiddns.h"

/* for Reverse-ip-lookup
 * https://rapiddns.io/sameip/216.58.223.110#result
 * brings list of domains associated with the ip
 */

Rapiddns::Rapiddns(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Rapiddns::replyFinished);
}
Rapiddns::~Rapiddns(){
    delete manager;
}

void Rapiddns::start(){
    QNetworkRequest request;
    QUrl url("https://rapiddns.io/subdomain/"+args->target+"?full=1#result");
    request.setUrl(url);
    manager->get(request);
}

void Rapiddns::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        GumboOutput *output = gumbo_parse(reply->readAll());
        getSubdomains(output->root);
        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

void Rapiddns::getSubdomains(GumboNode *node){
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_TBODY && node->v.element.children.length > 0){
        GumboVector tbodyChildren = node->v.element.children;
        for(unsigned int i = 0; i < tbodyChildren.length; i++){
            GumboNode *trNode = static_cast<GumboNode*>(tbodyChildren.data[i]);
            if(trNode->type == GUMBO_NODE_ELEMENT && trNode->v.element.tag == GUMBO_TAG_TR && trNode->v.element.children.length > 1){
                GumboNode *td = static_cast<GumboNode*>(trNode->v.element.children.data[3]);
                if(td->type == GUMBO_NODE_ELEMENT && td->v.element.tag == GUMBO_TAG_TD && td->v.element.children.length >0){
                    GumboNode *item = static_cast<GumboNode*>(td->v.element.children.data[0]);
                    if(item->type == GUMBO_NODE_TEXT)
                        emit subdomain(QString::fromUtf8(item->v.text.text));
                }
            }
        }
        return;
    }

    GumboVector *children = &node->v.element.children;
    for(unsigned int i = 0; i < children->length; i++)
        getSubdomains(static_cast<GumboNode*>(children->data[i]));
}

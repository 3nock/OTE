#include "SiteDossier.h"
#include <QStack>


/*
 * for now only page 1...
 * uses captcha...
 */
SiteDossier::SiteDossier(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.resultsCount++;

    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &SiteDossier::replyFinishedSubdomain);
}
SiteDossier::~SiteDossier(){
    delete manager;
}

void SiteDossier::start(){
    QNetworkRequest request;
    QUrl url("http://www.sitedossier.com/parentdomain/"+args->target+"/1");
    request.setUrl(url);
    manager->get(request);
    activeRequests++;
}

void SiteDossier::replyFinishedSubdomain(QNetworkReply *reply){
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

        for(unsigned int i = 0; i < node->v.element.children.length; i++){
            GumboNode *tiChild = static_cast<GumboNode*>(node->v.element.children.data[i]);
            if(tiChild->type == GUMBO_NODE_ELEMENT && tiChild->v.element.tag == GUMBO_TAG_A){
                GumboNode *link = static_cast<GumboNode*>(tiChild->v.element.children.data[0]);
                if(link->type == GUMBO_NODE_TEXT){
                    QString item = QString::fromUtf8(link->v.text.text);
                    item = item.remove(0, 7).remove("/");
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

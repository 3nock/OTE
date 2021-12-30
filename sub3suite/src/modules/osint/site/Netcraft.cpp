#include "Netcraft.h"
#include <QStack>


/*
 *  next page implementation not yet...
 *  using site+contains...
 *  has restrictions...
 */
Netcraft::Netcraft(ScanArgs args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "Netcraft";

    if(args.outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &Netcraft::replyFinishedSubdomain);
}
Netcraft::~Netcraft(){
    delete manager;
}

void Netcraft::start(){
    QNetworkRequest request;
    QUrl url("https://searchdns.netcraft.com/?restriction=site+contains&host="+target);
    request.setUrl(url);
    manager->get(request);
    activeRequests++;
}

void Netcraft::replyFinishedSubdomain(QNetworkReply *reply){
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
                    emit resultSubdomain(item);
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

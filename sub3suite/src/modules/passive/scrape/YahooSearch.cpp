#include "YahooSearch.h"
#include <QStack>


YahooSearch::YahooSearch(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_YAHOOSEARCH;

    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &YahooSearch::replyFinishedSubdomain);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &YahooSearch::replyFinishedUrl);
}
YahooSearch::~YahooSearch(){
    delete manager;
}

void YahooSearch::start(){
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.83 Safari/537.36");
    QUrl url;

    if(args.input_Domain){
        if(args.output_Hostname || args.output_URL){
            url.setUrl("https://search.yahoo.com/search?p=site:"+target+"&b=1&pz=10&bct=0&xargs=0&n=100");
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_Search){
        if(args.output_Hostname || args.output_URL){
            url.setUrl("https://search.yahoo.com/search?n=100&ei=UTF-8&va_vt=any&vo_vt=any&ve_vt=any&vp_vt=any&vst=0&vf=all&vm=p&fl=0&p="+target+"&vs=");
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void YahooSearch::replyFinishedSubdomain(QNetworkReply *reply){
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
        if(node->type != GUMBO_NODE_ELEMENT) // backtrack loop
            continue;

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 2 && node->v.element.children.length > 0)
        {
            GumboAttribute *styleAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(styleAttribute->value) == "visibility:hidden;")
            {
                GumboNode *span = static_cast<GumboNode*>(node->v.element.children.data[0]);
                GumboNode *child = static_cast<GumboNode*>(span->v.element.children.data[0]);

                emit resultSubdomain(child->v.text.text);
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

void YahooSearch::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(GumboMethods::getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty()) // backtrack loop
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 2 && node->v.element.children.length > 0)
        {
            GumboAttribute *styleAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(styleAttribute->value) == "visibility:hidden;")
            {
                GumboNode *span = static_cast<GumboNode*>(node->v.element.children.data[0]);
                GumboNode *child = static_cast<GumboNode*>(span->v.element.children.data[0]);

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

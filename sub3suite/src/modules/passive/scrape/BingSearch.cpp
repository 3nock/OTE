#include "BingSearch.h"
#include <QStack>

BingSearch::BingSearch(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_BINGSEARCH;

    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &BingSearch::replyFinishedSubdomain);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &BingSearch::replyFinishedUrl);
}
BingSearch::~BingSearch(){
    delete manager;
}

void BingSearch::start(){
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.83 Safari/537.36");
    QUrl url;

    if(args.input_Domain){
        if(args.output_Hostname){
            url.setUrl("https://www.bing.com/search?q=site:"+target+"&first=1&FORM=PORE&count=50");
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_Search){
        if(args.output_URL || args.output_Hostname){
            url.setUrl("https://www.bing.com/search?q=*"+target+"*&first=1&FORM=PORE&count=50");
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void BingSearch::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(GumboMethods::getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty()) // backtracking loop
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_CITE)
        {
            GumboNode *urlNode = static_cast<GumboNode*>(node->v.element.children.data[0]);
            QString url(urlNode->v.text.text);
            url = url.remove("https://").remove("http://").split("/")[0];
            emit resultSubdomain(url);
            log.resultsCount++;
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    this->end(reply);
}

void BingSearch::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(GumboMethods::getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty()) // backtracking loop
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_CITE)
        {
            GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
            emit resultURL(child->v.text.text);
            log.resultsCount++;
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    this->end(reply);
}

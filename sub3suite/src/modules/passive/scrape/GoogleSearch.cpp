#include "GoogleSearch.h"
#include <QStack>


/*
 * intext:"@udsm.ac.tz" site:udsm.ac.tz ==> sites containing email
 */
GoogleSearch::GoogleSearch(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_GOOGLESEARCH;

    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &GoogleSearch::replyFinishedSubdomain);
    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &GoogleSearch::replyFinishedUrl);
}
GoogleSearch::~GoogleSearch(){
    delete manager;
}

void GoogleSearch::start(){
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QUrl url;

    if(args.input_Domain) {
        if(args.output_Hostname) {
            url.setUrl("https://google.com/search?q=site:*."+target+"&btnG=Search&hl=en-US&biw=&bih=&gbv=1&start="+QString::number(m_offset)+"&filter=0&num=100");
            request.setUrl(url);
            manager->get(request);
            return;
        }

        if(args.output_URL) {
            url.setUrl("https://google.com/search?q=site:"+target+"/*&btnG=Search&hl=en-US&biw=&bih=&gbv=1&start="+QString::number(m_offset)+"&filter=0&num=100");
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_Email) {
        if(args.output_URL) {
            url.setUrl("https://google.com/search?q=intext:\""+target+"\"&btnG=Search&hl=en-US&biw=&bih=&gbv=1&start="+QString::number(m_offset)+"&filter=0&num=100");
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }

    if(args.input_Search) {
        if(args.output_URL || args.output_Hostname) {
            url.setUrl("https://google.com/search?q=site:*"+target+".*+OR+site:*"+target+".*.*&btnG=Search&hl=en-US&biw=&bih=&gbv=1&start="+QString::number(m_offset)+"&filter=0&num=100");
            request.setUrl(url);
            manager->get(request);
            return;
        }
    }
}

void GoogleSearch::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty()) // backtrack loop
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 1 && node->v.element.children.length == 1)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "BNeawe UPmit AP7Wnd")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                /* trim the results and send */
                QString subdomain(child->v.text.text);
                subdomain = subdomain.split(" ").at(0);
                emit resultSubdomain(subdomain);
                log.resultsCount++;
                continue;
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    this->end(reply);
}

void GoogleSearch::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty()) // backtrack loop
    {
        node = nodes.pop();
        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 1 && node->v.element.children.length > 0)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "egMi0 kCrYT")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                if(child->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length == 1)
                {
                    GumboAttribute *hrefAttribute = static_cast<GumboAttribute*>(child->v.element.attributes.data[0]);
                    QString url(hrefAttribute->value);
                    url = url.remove(0, 7);
                    url = url.split("&")[0];
                    emit resultURL(url);
                    continue;
                }
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    this->end(reply);
}

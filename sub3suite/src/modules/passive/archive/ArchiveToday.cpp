#include "ArchiveToday.h"
#include <QStack>

/*
 * has some problems with redirection, try using user agents...
 * keep updating the tld...
 */
ArchiveToday::ArchiveToday(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout);
    log.moduleName = OSINT_MODULE_ARCHIVETODAY;

    if(args.output_URL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ArchiveToday::replyFinishedUrl);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &ArchiveToday::replyFinishedSubdomain);
}
ArchiveToday::~ArchiveToday(){
    delete manager;
}

void ArchiveToday::start(){
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/85.0.4183.83 Safari/537.36");
    request.setRawHeader("Connection", "close");

    QUrl url;

    if(args.input_Domain){
        if(args.output_Hostname){
            url.setUrl("https://archive.md/*."+target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
        if(args.output_URL){
            url.setUrl("https://archive.md/*."+target+"/*");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }

    if(args.input_URL){
        if(args.output_Hostname){
            url.setUrl("https://archive.md/"+target);
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }

        if(args.output_URL){
            url.setUrl("https://archive.md/"+target+"*");
            request.setUrl(url);
            manager->get(request);
            activeRequests++;
        }
    }
}

void ArchiveToday::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;

    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(getBody(output->root));

    /* loop to parse and obtain subdomains from the body node... */
    while(!nodes.isEmpty()){
        GumboNode *node = nodes.pop();

        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length == 2 && node->v.element.children.length > 0)
        {
            GumboAttribute *styleAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(styleAttribute->value) == "color:#1D2D40;font-size:16px")
            {
                GumboNode *domain = static_cast<GumboNode*>(node->v.element.children.data[0]);
                if(domain->type == GUMBO_NODE_TEXT)
                {
                    emit resultSubdomain(QString::fromUtf8(domain->v.text.text));
                    log.resultsCount++;
                }
            }
        }

        /* push the children nodes to the nodes Stack and continue the loop */
        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    /* finilizing... */
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    end(reply);
}

void ArchiveToday::replyFinishedUrl(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;

    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(getBody(output->root));

    /* loop to parse and obtain subdomains from the body node... */
    while(!nodes.isEmpty()){
        GumboNode *node = nodes.pop();

        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length == 2 && node->v.element.children.length > 0)
        {
            GumboAttribute *styleAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(styleAttribute->value) == "color:#1D2D40;font-size:16px")
            {
                GumboNode *domain = static_cast<GumboNode*>(node->v.element.children.data[0]);
                if(domain->type == GUMBO_NODE_TEXT)
                {
                    emit resultURL(QString::fromUtf8(domain->v.text.text));
                    log.resultsCount++;
                }
            }
        }

        /* push the children nodes to the nodes Stack and continue the loop */
        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    /* finilizing... */
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    end(reply);
}

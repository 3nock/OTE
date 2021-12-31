#include "DogPile.h"
#include <QStack>


/*
 * doesnt show last page...
 * redirection issues...
 */
DogPile::DogPile(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "DogPile";

    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DogPile::replyFinishedSubdomain);
    if(args.outputEmail)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DogPile::replyFinishedEmail);
    if(args.outputUrl)
        connect(manager, &s3sNetworkAccessManager::finished, this, &DogPile::replyFinishedUrl);
}
DogPile::~DogPile(){
    delete manager;
}

void DogPile::start(){
    QNetworkRequest request;

    if(args.inputDomain){
        if(args.outputSubdomain){
            QUrl url("https://www.dogpile.com/serp?q="+target);
            request.setUrl(url);
            manager->get(request);
            m_firstRequest = true;
            activeRequests++;
        }

        if(args.outputUrl){
            QUrl url("https://www.dogpile.com/serp?q="+target);
            request.setUrl(url);
            manager->get(request);
            m_firstRequest = true;
            activeRequests++;
        }
    }
}

void DogPile::replyFinishedSubdomain(QNetworkReply *reply){
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

        if(m_firstRequest){
            if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length == 3 && node->v.element.children.length == 1)
            {
                GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(classAttribute->value) == "pagination__num")
                {
                    GumboAttribute *hrefAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[1]);
                    this->sendRequests(QString::fromUtf8(hrefAttribute->value));
                }
            }
        }

        if(node->v.element.tag == GUMBO_TAG_SPAN && node->v.element.attributes.length == 1 && node->v.element.children.length > 1)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "web-bing__url")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                QString domain = QString::fromUtf8(child->v.text.text);
                domain = domain.remove("http://").remove("https://");
                domain.append(target);
                emit resultSubdomain(domain);
                log.resultsCount++;
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    end(reply);
}

void DogPile::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    /*
     * not yet implemented...
     */

    end(reply);
}

void DogPile::replyFinishedUrl(QNetworkReply *reply){
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

        if(m_firstRequest){
            if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length == 3 && node->v.element.children.length == 1)
            {
                GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(classAttribute->value) == "pagination__num")
                {
                    GumboAttribute *hrefAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[1]);
                    this->sendRequests(QString::fromUtf8(hrefAttribute->value));
                }
            }
        }

        if(node->v.element.tag == GUMBO_TAG_SPAN && node->v.element.attributes.length == 1 && node->v.element.children.length > 1)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "web-bing__url")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                QString domain = QString::fromUtf8(child->v.text.text);
                domain.append(target);
                emit resultUrl(domain);
                log.resultsCount++;
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    end(reply);
}

void DogPile::sendRequests(QString anotherPage){
    QNetworkRequest request;

    if(args.inputDomain){
        if(args.outputSubdomain){
            if(m_sentPages < args.config->maxPage){
                QUrl url("https://www.dogpile.com"+anotherPage);
                request.setUrl(url);
                manager->get(request);
                m_firstRequest = false;
                activeRequests++;
                m_sentPages++;
            }
        }
    }
}

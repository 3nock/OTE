#include "Ask.h"
#include <QStack>


Ask::Ask(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Ask";

    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Ask::replyFinishedSubdomain);
    if(args->outputEmail)
        connect(manager, &MyNetworkAccessManager::finished, this, &Ask::replyFinishedEmail);
    if(args->outputUrl)
        connect(manager, &MyNetworkAccessManager::finished, this, &Ask::replyFinishedUrl);
}
Ask::~Ask(){
    delete manager;
}

void Ask::start(){
    QNetworkRequest request;

    if(args->inputDomain){
        if(args->outputSubdomain){
            QUrl url("https://www.ask.com/web?q=site:"+args->target+"&page=1&qid=8D6EE6BF52E0C04527E51A64F22C4534&o=0&l=dir&qsrc=998&qo=pagination");
            request.setUrl(url);
            manager->get(request);
            m_firstRequest = true;
            activeRequests++;
        }

        if(args->outputUrl){
            QUrl url("https://www.ask.com/web?q=site:"+args->target+"&page=1&qid=8D6EE6BF52E0C04527E51A64F22C4534&o=0&l=dir&qsrc=998&qo=pagination");
            request.setUrl(url);
            manager->get(request);
            m_firstRequest = true;
            activeRequests++;
        }
    }
}

void Ask::replyFinishedSubdomain(QNetworkReply *reply){
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
            if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 1 && node->v.element.children.length == 1)
            {
                GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(classAttribute->value) == "PartialWebPagination-link-text")
                {
                    GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                    int page = QString::fromUtf8(child->v.text.text).toInt();
                    if(page > m_lastPage)
                        m_lastPage = page;
                }
            }
        }

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 1 && node->v.element.children.length == 1)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "PartialSearchResults-item-url PartialSearchResults-item-top-url")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                QString domain = QString::fromUtf8(child->v.text.text);
                domain = domain.split("/")[0];
                emit subdomain(domain);
                log.resultsCount++;
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    if(m_firstRequest)
        this->sendRequests();

    end(reply);
}

void Ask::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    /*
     * not yet implemented...
     */

    end(reply);
}

void Ask::replyFinishedUrl(QNetworkReply *reply){
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
            if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 1 && node->v.element.children.length == 1)
            {
                GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(classAttribute->value) == "PartialWebPagination-link-text")
                {
                    GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                    int page = QString::fromUtf8(child->v.text.text).toInt();
                    if(page > m_lastPage)
                        m_lastPage = page;
                }
            }
        }

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 1 && node->v.element.children.length == 1)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "PartialSearchResults-item-url PartialSearchResults-item-top-url")
            {
                GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                emit url(QString::fromUtf8(child->v.text.text));
                log.resultsCount++;
            }
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    if(m_firstRequest)
        this->sendRequests();

    end(reply);
}

void Ask::sendRequests(){
    QNetworkRequest request;

    if(args->inputDomain){
        if(args->outputSubdomain)
        {
            ///
            /// getting the max pages to query...
            ///
            int lastPage;
            if(args->maxPage <= m_lastPage)
                lastPage = args->maxPage;
            else
                lastPage = m_lastPage;

            ///
            /// loop to send appropriate requests...
            ///
            int currentPage = 2;
            while(currentPage < lastPage){
                QUrl url("https://www.ask.com/web?q=site:"+args->target+"&page="+QString::number(currentPage)+"&qid=8D6EE6BF52E0C04527E51A64F22C4534&o=0&l=dir&qsrc=998&qo=pagination");
                request.setUrl(url);
                manager->get(request);
                m_firstRequest = false;
                activeRequests++;
                currentPage++;
            }
        }
    }
}

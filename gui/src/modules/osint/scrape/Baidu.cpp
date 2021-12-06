#include "Baidu.h"
#include <QStack>


/*
 * has scrape prevention...
 */
Baidu::Baidu(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "Baidu";

    if(args->outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &Baidu::replyFinishedSubdomain);
    if(args->outputEmail)
        connect(manager, &NetworkAccessManager::finished, this, &Baidu::replyFinishedEmail);
    if(args->outputUrl)
        connect(manager, &NetworkAccessManager::finished, this, &Baidu::replyFinishedUrl);
}
Baidu::~Baidu(){
    delete manager;
}

void Baidu::start(){
    QNetworkRequest request;

    if(args->inputDomain){
        if(args->outputSubdomain){
            QUrl url("https://www.baidu.com/s?wd=site:"+args->target+"&oq=site:"+args->target+"&pn=1");
            request.setUrl(url);
            manager->get(request);
            m_firstRequest = true;
            activeRequests++;
        }

        if(args->outputUrl){
            QUrl url("https://www.baidu.com/s?wd=site:"+args->target+"&oq=site:"+args->target+"&pn=1");
            request.setUrl(url);
            manager->get(request);
            m_firstRequest = true;
            activeRequests++;
        }
    }
}

void Baidu::replyFinishedSubdomain(QNetworkReply *reply){
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
            if(node->v.element.tag == GUMBO_TAG_SPAN && node->v.element.attributes.length == 1 && node->v.element.children.length == 1)
            {
                GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(classAttribute->value) == "page-item_M4MDr pc")
                {
                    GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                    int page = QString::fromUtf8(child->v.text.text).toInt();
                    if(page > m_lastPage)
                        m_lastPage = page;
                }
            }
        }

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 1 && node->v.element.children.length > 0)
        {
            GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(classAttribute->value) == "f13 c-gap-top-xsmall se_st_footer user-avatar")
            {
                GumboNode *a = static_cast<GumboNode*>(node->v.element.children.data[0]);
                GumboNode *child = static_cast<GumboNode*>(a->v.element.children.data[0]);
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

void Baidu::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    /*
     * not yet implemented...
     */

    end(reply);
}

void Baidu::replyFinishedUrl(QNetworkReply *reply){
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
            if(node->v.element.tag == GUMBO_TAG_SPAN && node->v.element.attributes.length == 1 && node->v.element.children.length == 1)
            {
                GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(classAttribute->value) == "page-item_M4MDr pc")
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
            if(QString::fromUtf8(classAttribute->value) == "f13 c-gap-top-xsmall se_st_footer user-avatar")
            {
                GumboNode *a = static_cast<GumboNode*>(node->v.element.children.data[0]);
                GumboNode *child = static_cast<GumboNode*>(a->v.element.children.data[0]);
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

void Baidu::sendRequests(){
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
                QUrl url("https://www.baidu.com/s?wd=site:"+args->target+"&oq=site:"+args->target+"&pn="+QString::number(currentPage));
                request.setUrl(url);
                manager->get(request);
                m_firstRequest = false;
                activeRequests++;
                currentPage++;
            }
        }
    }
}

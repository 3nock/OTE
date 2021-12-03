#include "Yahoo.h"
#include <QStack>


/*
 * has a different type of next page...
 * redirection probs...
 */
Yahoo::Yahoo(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new MyNetworkAccessManager(this);
    log.moduleName = "Yahoo";

    if(args->outputSubdomain)
        connect(manager, &MyNetworkAccessManager::finished, this, &Yahoo::replyFinishedSubdomain);
    if(args->outputEmail)
        connect(manager, &MyNetworkAccessManager::finished, this, &Yahoo::replyFinishedEmail);
    if(args->outputUrl)
        connect(manager, &MyNetworkAccessManager::finished, this, &Yahoo::replyFinishedUrl);
}
Yahoo::~Yahoo(){
    delete manager;
}

void Yahoo::start(){
    QNetworkRequest request;

    if(args->inputDomain){
        if(args->outputSubdomain){
            QUrl url("https://search.yahoo.com/search?p=site:"+args->target+"&b=1&pz=10&bct=0&xargs=0");
            request.setUrl(url);
            manager->get(request);
            m_firstRequest = true;
            activeRequests++;
        }

        if(args->outputUrl){
            QUrl url("https://search.yahoo.com/search?p=site:"+args->target+"&b=1&pz=10&bct=0&xargs=0");
            request.setUrl(url);
            manager->get(request);
            m_firstRequest = true;
            activeRequests++;
        }
    }
}

void Yahoo::replyFinishedSubdomain(QNetworkReply *reply){
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
            if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length > 1 && node->v.element.children.length > 0)
            {
                GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(classAttribute->value) == "pages")
                {
                    /*
                     next pages implemented here...
                     */
                }
            }
        }

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 2 && node->v.element.children.length > 0)
        {
            GumboAttribute *styleAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(styleAttribute->value) == "visibility:hidden;")
            {
                GumboNode *span = static_cast<GumboNode*>(node->v.element.children.data[0]);
                GumboNode *child = static_cast<GumboNode*>(span->v.element.children.data[0]);

                emit subdomain(child->v.text.text);
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

void Yahoo::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    /*
     * not yet implemented...
     */

    end(reply);
}

void Yahoo::replyFinishedUrl(QNetworkReply *reply){
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
            if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length > 1 && node->v.element.children.length > 0)
            {
                GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(classAttribute->value) == "pages")
                {
                    /*
                     next pages implemented here...
                     */
                }
            }
        }

        if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 2 && node->v.element.children.length > 0)
        {
            GumboAttribute *styleAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
            if(QString::fromUtf8(styleAttribute->value) == "visibility:hidden;")
            {
                GumboNode *span = static_cast<GumboNode*>(node->v.element.children.data[0]);
                GumboNode *child = static_cast<GumboNode*>(span->v.element.children.data[0]);

                emit url(child->v.text.text);
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

void Yahoo::sendRequests(){
    QNetworkRequest request;

    if(args->inputDomain){
        if(args->outputSubdomain)
        {
            ///
            /// getting the max pages to query...
            ///
            int lastPage;
            m_lastPage = 5;
            if(args->maxPage <= m_lastPage)
                lastPage = args->maxPage;
            else
                lastPage = m_lastPage;

            ///
            /// loop to send appropriate requests...
            ///
            int currentPage = 2;
            int first = 11;
            while(currentPage < lastPage){
                QUrl url("https://search.yahoo.com/search?p=site:"+args->target+"&b="+QString::number(first)+"&pz=10&bct=0&xargs=0");
                request.setUrl(url);
                manager->get(request);
                m_firstRequest = false;
                activeRequests++;
                currentPage++;
                first += 10;
            }
        }
    }
}

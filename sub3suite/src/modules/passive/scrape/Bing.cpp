#include "Bing.h"
#include <QStack>


/*
 * has a different type of next page...
 * redirection probs...
 */
Bing::Bing(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this);
    log.moduleName = "Bing";

    if(args.outputSubdomain)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bing::replyFinishedSubdomain);
    if(args.outputEmail)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bing::replyFinishedEmail);
    if(args.outputUrl)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Bing::replyFinishedUrl);
}
Bing::~Bing(){
    delete manager;
}

void Bing::start(){
    QNetworkRequest request;

    if(args.inputDomain){
        if(args.outputSubdomain){
            QUrl url("https://www.bing.com/search?q=site:"+target+"&first=1&FORM=PORE");
            //request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
            request.setUrl(url);
            manager->get(request);
            m_firstRequest = true;
            activeRequests++;
        }

        if(args.outputUrl){
            QUrl url("https://www.bing.com/search?q=site:"+target+"&first=1&FORM=PORE");
            //request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
            request.setUrl(url);
            manager->get(request);
            m_firstRequest = true;
            activeRequests++;
        }
    }
}

void Bing::replyFinishedSubdomain(QNetworkReply *reply){
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
            if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length > 0 && node->v.element.children.length == 1)
            {
                GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(classAttribute->value) == "b_widePag sb_bp")
                {
                    GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                    int page = QString::fromUtf8(child->v.text.text).toInt();
                    if(page > m_lastPage)
                        m_lastPage = page;
                }
            }
        }

        if(node->v.element.tag == GUMBO_TAG_CITE)
        {
            GumboNode *url = static_cast<GumboNode*>(node->v.element.children.data[0]);
            QString domain = QString::fromUtf8(url->v.text.text);
            domain = domain.remove("https://").remove("http://").split("/")[0];
            emit resultSubdomain(domain);
            log.resultsCount++;
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

void Bing::replyFinishedEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    /*
     * not yet implemented...
     */

    end(reply);
}

void Bing::replyFinishedUrl(QNetworkReply *reply){
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
            if(node->v.element.tag == GUMBO_TAG_A && node->v.element.attributes.length > 0 && node->v.element.children.length == 1)
            {
                GumboAttribute *classAttribute = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(classAttribute->value) == "b_widePag sb_bp")
                {
                    GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
                    int page = QString::fromUtf8(child->v.text.text).toInt();
                    if(page > m_lastPage)
                        m_lastPage = page;
                }
            }
        }

        if(node->v.element.tag == GUMBO_TAG_CITE)
        {
            GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
            emit resultUrl(QString::fromUtf8(child->v.text.text));
            log.resultsCount++;
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

void Bing::sendRequests(){
    QNetworkRequest request;

    if(args.inputDomain){
        if(args.outputSubdomain)
        {
            ///
            /// getting the max pages to query...
            ///
            int lastPage;
            if(args.config->maxPage <= m_lastPage)
                lastPage = args.config->maxPage;
            else
                lastPage = m_lastPage;

            ///
            /// loop to send appropriate requests...
            ///
            int currentPage = 2;
            int first = 11;
            while(currentPage < lastPage){
                QUrl url("https://www.bing.com/search?q=site:"+target+"&first="+QString::number(first)+"&FORM=PORE");
                //request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
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

#include "Dnsdumpster.h"
#include <QStack>

/*
 * fix sometimes, returns words in ip-address place...
 */
Dnsdumpster::Dnsdumpster(ScanArgs *args): AbstractOsintModule(args)
{
    manager = new NetworkAccessManager(this);
    log.moduleName = "DnsDumpster";

    if(args->outputSubdomainIp)
        connect(manager, &NetworkAccessManager::finished, this, &Dnsdumpster::replyFinishedSubdomainIp);
    if(args->outputSubdomain)
        connect(manager, &NetworkAccessManager::finished, this, &Dnsdumpster::replyFinishedSubdomain);
    if(args->outputIp)
        connect(manager, &NetworkAccessManager::finished, this, &Dnsdumpster::replyFinishedIp);
}
Dnsdumpster::~Dnsdumpster(){
    delete manager;
}

///
/// first scan is to get the crsf token...
///
void Dnsdumpster::start(){
    QNetworkRequest request;
    QUrl url("https://dnsdumpster.com");
    request.setUrl(url);
    m_queryToGetToken = true;
    manager->get(request);
    activeRequests++;
}

void Dnsdumpster::replyFinishedSubdomainIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    if(m_queryToGetToken)
        this->m_getToken(reply);

    else{
        QStack<GumboNode*> nodes;
        GumboOutput *output = gumbo_parse(reply->readAll());
        nodes.push(this->getBody(output->root));

        GumboNode *node;
        while(!nodes.isEmpty())
        {
            node = nodes.pop();
            if(node->type != GUMBO_NODE_ELEMENT)
                continue;

            if(node->v.element.tag == GUMBO_TAG_TD && node->v.element.attributes.length == 1)
            {
                GumboNode *name = static_cast<GumboNode*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(name->v.text.text) == "col-md-4") // col-md-4 == subdomain name
                {
                    GumboVector tdChildren = node->v.element.children;
                    for(unsigned int i = 0; i < tdChildren.length; i++)
                    {
                        GumboNode *tdChild = static_cast<GumboNode*>(node->v.element.children.data[i]);
                        if(tdChild->type == GUMBO_NODE_TEXT)
                            emit subdomain(QString::fromUtf8(tdChild->v.text.text));
                    }
                }
                continue;
            }

            GumboVector *children = &node->v.element.children;
            for(unsigned int i = 0; i < children->length; i++)
                nodes.push(static_cast<GumboNode*>(children->data[i]));
        }
        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }

    end(reply);
}

void Dnsdumpster::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    if(m_queryToGetToken)
        this->m_getToken(reply);

    else{
        QStack<GumboNode*> nodes;
        GumboOutput *output = gumbo_parse(reply->readAll());
        nodes.push(this->getBody(output->root));

        GumboNode *node;
        while(!nodes.isEmpty())
        {
            node = nodes.pop();
            if(node->type != GUMBO_NODE_ELEMENT)
                continue;

            if(node->v.element.tag == GUMBO_TAG_TD && node->v.element.attributes.length == 1)
            {
                GumboNode *name = static_cast<GumboNode*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(name->v.text.text) == "col-md-4") // col-md-4 == subdomain name
                {
                    GumboVector tdChildren = node->v.element.children;
                    for(unsigned int i = 0; i < tdChildren.length; i++)
                    {
                        GumboNode *tdChild = static_cast<GumboNode*>(node->v.element.children.data[i]);
                        if(tdChild->type == GUMBO_NODE_TEXT){
                            emit subdomain(QString::fromUtf8(tdChild->v.text.text));
                            log.resultsCount++;
                        }
                    }
                }
                continue;
            }

            GumboVector *children = &node->v.element.children;
            for(unsigned int i = 0; i < children->length; i++)
                nodes.push(static_cast<GumboNode*>(children->data[i]));
        }
        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }

    end(reply);
}

void Dnsdumpster::replyFinishedIp(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    if(m_queryToGetToken)
        this->m_getToken(reply);

    else{
        QStack<GumboNode*> nodes;
        GumboOutput *output = gumbo_parse(reply->readAll());
        nodes.push(this->getBody(output->root));

        GumboNode *node;
        while(!nodes.isEmpty())
        {
            node = nodes.pop();
            if(node->type != GUMBO_NODE_ELEMENT)
                continue;

            if(node->v.element.tag == GUMBO_TAG_TD && node->v.element.attributes.length == 1)
            {
                GumboNode *address = static_cast<GumboNode*>(node->v.element.attributes.data[0]);
                if(QString::fromUtf8(address->v.text.text) == "col-md-3") // col-md-3 == ip-address
                {
                    if(node->v.element.children.length > 0)
                    {
                        GumboNode *td = static_cast<GumboNode*>(node->v.element.children.data[0]);
                        if(td->type == GUMBO_NODE_TEXT){
                            emit ip(QString::fromUtf8(td->v.text.text));
                            log.resultsCount++;
                        }
                    }
                }
                continue;
            }

            GumboVector *children = &node->v.element.children;
            for(unsigned int i = 0; i < children->length; i++)
                nodes.push(static_cast<GumboNode*>(children->data[i]));
        }
        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }

    end(reply);
}

///
/// after obtaining the csrf token, send the request to obtain results...
///
void Dnsdumpster::m_getToken(QNetworkReply *reply){
    QString token;
    QStack<GumboNode*> nodes;

    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(this->getBody(output->root));

    GumboNode *node;
    while(!nodes.isEmpty()){
        node = nodes.pop();

        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_INPUT && node->v.element.attributes.length == 3)
        {
            GumboNode *name = static_cast<GumboNode*>(node->v.element.attributes.data[1]);
            if(QString::fromUtf8(name->v.text.text) == "csrfmiddlewaretoken"){
                GumboNode *csrfValue = static_cast<GumboNode*>(node->v.element.attributes.data[2]);
                token = QString::fromUtf8(csrfValue->v.text.text);
            }
            continue;
        }

        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }
    gumbo_destroy_output(&kGumboDefaultOptions, output);

    /* Request to obtain the Results */
    QNetworkRequest request;
    request.setRawHeader("Referer", "https://dnsdumpster.com");
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setUrl(QUrl("https://dnsdumpster.com"));

    QByteArray data;
    data.append("csrfmiddlewaretoken="+token+"&");
    data.append("targetip="+args->target+"&");
    data.append("user=free");

    manager->post(request, data);
    m_queryToGetToken = false;
    activeRequests++;
}

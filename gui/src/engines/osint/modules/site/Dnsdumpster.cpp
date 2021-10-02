#include "Dnsdumpster.h"


Dnsdumpster::Dnsdumpster(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Dnsdumpster::replyFinished);
}
Dnsdumpster::~Dnsdumpster(){
    delete manager;
}

void Dnsdumpster::start(){
    ///
    /// first scan to get the crsf token...
    ///
    QNetworkRequest request;
    QUrl url("https://dnsdumpster.com");
    request.setUrl(url);
    firstScanToGetToken = true;
    manager->get(request);
}

void Dnsdumpster::replyFinished(QNetworkReply *reply){
    if(firstScanToGetToken){
        if(reply->error())
        {
            emit errorLog(reply->errorString());
        }
        else
        {
            GumboOutput *output = gumbo_parse(reply->readAll());
            getToken(output->root);
            gumbo_destroy_output(&kGumboDefaultOptions, output);
            ///
            /// getting the subdomains...
            ///
            if(!m_token.isEmpty())
            {
                QByteArray data;
                QNetworkRequest request;
                request.setRawHeader("Referer", "https://dnsdumpster.com");
                request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
                request.setUrl(QUrl("https://dnsdumpster.com"));
                data.append("csrfmiddlewaretoken="+m_token);
                data.append("&");
                data.append("targetip="+target);
                data.append("&");
                data.append("user=free");
                manager->post(request, data);
                firstScanToGetToken = false;
            }
        }
        reply->deleteLater();
    }
    else
    {
        if(reply->error())
        {
            // an error occured...
        }
        else
        {
            GumboOutput *output = gumbo_parse(reply->readAll());
            getSubdomains(output->root);
            gumbo_destroy_output(&kGumboDefaultOptions, output);
        }
        reply->deleteLater();
        emit quitThread();
    }
}

void Dnsdumpster::getToken(GumboNode *node){
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_INPUT && node->v.element.attributes.length == 3)
    {
        GumboNode *name = static_cast<GumboNode*>(node->v.element.attributes.data[1]);
        if(QString::fromUtf8(name->v.text.text) == "csrfmiddlewaretoken"){
            GumboNode *csrfValue = static_cast<GumboNode*>(node->v.element.attributes.data[2]);
            m_token = QString::fromUtf8(csrfValue->v.text.text);
        }
        return;
    }

    GumboVector *children = &node->v.element.children;
    for(unsigned int i = 0; i < children->length; i++)
        getToken(static_cast<GumboNode*>(children->data[i]));
    return;
}

void Dnsdumpster::getSubdomains(GumboNode *node){
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_TD && node->v.element.attributes.length == 1)
    {
        GumboNode *name = static_cast<GumboNode*>(node->v.element.attributes.data[0]);
        if(QString::fromUtf8(name->v.text.text) == "col-md-4") // col-md-4 == subdomain name
        {
            GumboVector tdChildren = node->v.element.children;
            bool key = true, value = false;
            QString bannerName, bannerValue;
            for(unsigned int i = 0; i < tdChildren.length; i++)
            {
                GumboNode *tdChild = static_cast<GumboNode*>(node->v.element.children.data[i]);
                if(tdChild->type == GUMBO_NODE_TEXT)
                    emit scanResults(QString::fromUtf8(tdChild->v.text.text));

                ///
                /// if the particular subdomain has any banner...
                ///
                if(tdChild->type == GUMBO_NODE_ELEMENT && tdChild->v.element.tag == GUMBO_TAG_SPAN){
                    if(tdChild->v.element.children.length > 0){
                        GumboNode *span = static_cast<GumboNode*>(tdChild->v.element.children.data[0]);
                        if(span->type == GUMBO_NODE_TEXT && key){
                            bannerName = QString::fromUtf8(span->v.text.text);
                            key = false; value = true;
                            continue;
                        }
                        if(span->type == GUMBO_NODE_TEXT && value){
                            bannerValue = QString::fromUtf8(span->v.text.text);
                            key = true; value = false;
                            m_banner.insert(bannerName, bannerValue);
                        }
                    }
                }
            }
        }
        /*
              Ip-Address of the Subdomain, and the following col-md-3 is the name

        if(QString::fromUtf8(name->v.text.text) == "col-md-3") // col-md-3 == ip-address
        {
            if(node->v.element.children.length > 0)
            {
                GumboNode *td = static_cast<GumboNode*>(node->v.element.children.data[0]);
                if(td->type == GUMBO_NODE_TEXT)
                    emit scanResults(QString::fromUtf8(td->v.text.text));
            }
        }
        */
        return;
    }

    GumboVector *children = &node->v.element.children;
    for(unsigned int i = 0; i < children->length; i++)
        getSubdomains(static_cast<GumboNode*>(children->data[i]));
    return;
}

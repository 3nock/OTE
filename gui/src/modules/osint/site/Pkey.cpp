#include "Pkey.h"

Pkey::Pkey(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Pkey::replyFinished);
}
Pkey::~Pkey(){
    delete manager;
}

void Pkey::start(){
    QNetworkRequest request;
    QByteArray data;
    QUrl url("https://www.pkey.in/tools-i/search-subdomains");
    request.setUrl(url);
    request.setRawHeader("Pragma", "no-cache");
    request.setRawHeader("Origin", "https://www.pkey.in");
    request.setRawHeader("Accept-Encoding", "gzip, deflate, br");
    request.setRawHeader("Accept-Language", "en-US,en;q=0.9,it;q=0.8");
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_13_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36");
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8");
    request.setRawHeader("Cache-Control", "no-cache");
    request.setRawHeader("Referer", "http://www.pkey.in/tools-i/search-subdomains");
    request.setRawHeader("Connection", "keep-alive");
    //request.setRawHeader("DNT", "1");
    data.append("zone="+args->target);
    data.append("&");
    data.append("submit=");
    ///
    /// ignore ssl verification...
    ///
    QSslConfiguration conf = request.sslConfiguration();
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    request.setSslConfiguration(conf);
    manager->post(request, data);
}

void Pkey::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        GumboOutput *output = gumbo_parse(reply->readAll());
        getSubdomains(output->root);
        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

void Pkey::getSubdomains(GumboNode *node){
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_TBODY && node->v.element.children.length > 1){
        GumboVector tbodyChildren = node->v.element.children;
        for(unsigned int i = 1; i < tbodyChildren.length; i++){
            GumboNode *trNode = static_cast<GumboNode*>(tbodyChildren.data[i]);
            if(trNode->type == GUMBO_NODE_ELEMENT && trNode->v.element.tag == GUMBO_TAG_TR && trNode->v.element.children.length > 1){
                GumboNode *td = static_cast<GumboNode*>(trNode->v.element.children.data[0]);
                if(td->type == GUMBO_NODE_ELEMENT && td->v.element.tag == GUMBO_TAG_TD && td->v.element.children.length >0){
                    GumboNode *item = static_cast<GumboNode*>(td->v.element.children.data[0]);
                    if(item->type == GUMBO_NODE_TEXT)
                        emit subdomain(QString::fromUtf8(item->v.text.text));
                }
            }
        }
        return;
    }

    GumboVector *children = &node->v.element.children;
    for(unsigned int i = 0; i < children->length; i++)
        getSubdomains(static_cast<GumboNode*>(children->data[i]));
}

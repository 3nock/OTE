#include "Bing.h"


Bing::Bing(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Bing::replyFinished);
}
Bing::~Bing(){
    delete manager;
}

void Bing::start(){
    QNetworkRequest request;
    while(m_page < 10){
        m_page++;
        QUrl url("https://www.Bing.com/web?q=site:"+args->target+"&page="+QString::number(m_page)+"&qid=8D6EE6BF52E0C04527E51A64F22C4534&o=0&l=dir&qsrc=998&qo=pagination");
        request.setUrl(url);
        manager->get(request);
        activeRequests++;
    }
}

void Bing::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        GumboOutput *output = gumbo_parse(reply->readAll());
        GumboNode *body = getBody(output->root);
        if(body != nullptr)
            getLinks(body);
        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    activeRequests--;
    if(activeRequests == 0)
        emit quitThread();
}

void Bing::getLinks(GumboNode *node){
    if(node->type != GUMBO_NODE_ELEMENT || node->v.element.tag == GUMBO_TAG_SCRIPT)
        return;

    if(node->v.element.tag == GUMBO_TAG_DIV && node->v.element.attributes.length == 1 && node->v.element.children.length > 0)
    {
        GumboAttribute *a = static_cast<GumboAttribute*>(node->v.element.attributes.data[0]);
        QString name = QString::fromUtf8(a->name);
        QString value = QString::fromUtf8(a->value);
        if(name == "class" && value == "PartialSearchResults-item-url")
        {
            GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[0]);
            if(child->type == GUMBO_NODE_TEXT){
                QString subdomain = QString::fromUtf8(child->v.text.text);
                subdomain = subdomain.split("/")[0];
                emit scanResults(subdomain);
            }
        }
    }

    GumboVector *children = &node->v.element.children;
    for(unsigned int i = 0; i < children->length; i++)
        getLinks(static_cast<GumboNode*>(children->data[i]));
    return;
}

GumboNode *Bing::getBody(GumboNode *node){
    for(unsigned int i = 0; i < node->v.element.children.length; i++){
        GumboNode *child = static_cast<GumboNode*>(node->v.element.children.data[i]);
        if(child->type == GUMBO_NODE_ELEMENT && child->v.element.tag == GUMBO_TAG_BODY)
            return child;
    }
    return nullptr;
}

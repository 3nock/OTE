#include "Crtsh.h"


Crtsh::Crtsh(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Crtsh::replyFinished);
}
Crtsh::~Crtsh(){
    delete manager;
}

void Crtsh::start(){
    QNetworkRequest request;
    QUrl url("https://crt.sh/?q="+args->target);
    request.setUrl(url);
    manager->get(request);
}

void Crtsh::replyFinished(QNetworkReply *reply){
    if(reply->error())
    {
        emit errorLog(reply->errorString());
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

void Crtsh::getSubdomains(GumboNode* node) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }

  if (node->v.element.tag == GUMBO_TAG_TD && node->v.element.attributes.length == 0) {
        GumboVector tdChildren = node->v.element.children;
        for(unsigned int i = 0; i < tdChildren.length; i++)
        {
            GumboNode *td = static_cast<GumboNode*>(tdChildren.data[i]);
            if(td->type == GUMBO_NODE_TEXT)
                emit scanResults(QString::fromUtf8(td->v.text.text));
        }
        return;
  }

  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i)
    getSubdomains(static_cast<GumboNode*>(children->data[i]));
}

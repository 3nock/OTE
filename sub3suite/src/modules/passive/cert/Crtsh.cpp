#include "Crtsh.h"
#include <QStack>

/*
 * use regex for some queries
 */
Crtsh::Crtsh(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_CRTSH;

    if(args.output_SSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Crtsh::replyFinishedSSL);
    if(args.output_EnumSSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Crtsh::replyFinishedEnumSSL);
    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &Crtsh::replyFinishedSubdomain);
}
Crtsh::~Crtsh(){
    delete manager;
}

void Crtsh::start(){
    QNetworkRequest request;
    QUrl url("https://crt.sh/?q="+target);
    request.setUrl(url);
    manager->get(request);
}

void Crtsh::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    if(args.input_Domain){
        /* a stack for storing the GumboNodes for parsing by backtracking... */
        QStack<GumboNode*> m_nodes;
        /* getting the body node... */
        GumboOutput *output = gumbo_parse(reply->readAll());
        m_nodes.push(getBody(output->root));

        /* loop to parse and obtain subdomains from the body node... */
        while(!m_nodes.isEmpty()){
            GumboNode *node = m_nodes.pop();

            if(node->type != GUMBO_NODE_ELEMENT)
                continue;

            if (node->v.element.tag == GUMBO_TAG_TD && node->v.element.attributes.length == 0) {
                  GumboVector tdChildren = node->v.element.children;
                  for(unsigned int i = 0; i < tdChildren.length; i++)
                  {
                      GumboNode *td = static_cast<GumboNode*>(tdChildren.data[i]);
                      if(td->type == GUMBO_NODE_TEXT){
                          QString domain = QString::fromUtf8(td->v.text.text);
                          emit resultSubdomain(domain);
                          log.resultsCount++;
                      }
                  }
                  continue;
            }

            /* push the children m_nodes to the m_nodes Stack and continue the loop */
            GumboVector *children = &node->v.element.children;
            for(unsigned int i = 0; i < children->length; i++)
                m_nodes.push(static_cast<GumboNode*>(children->data[i]));
        }
        /* finilizing... */
        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }

    if(args.input_SSL)
    {
        if(m_queryToGetId)
            m_getCertId(reply); // get the crtsh certificate id and request to download the certificate...
        else
        {
            QList<QSslCertificate> certList = QSslCertificate::fromData(reply->readAll(), QSsl::Pem);
            foreach(const QSslCertificate &cert, certList){
                foreach(const QString &altName, cert.subjectAlternativeNames()){
                    emit resultSubdomain(altName);
                    log.resultsCount++;
                }
            }
        }
    }

    this->end(reply);
}

void Crtsh::replyFinishedSSL(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    if(m_queryToGetId)
        this->m_getCertId(reply);

    else{

    }

    this->end(reply);
}

void Crtsh::replyFinishedEnumSSL(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    if(m_queryToGetId)
        m_getCertId(reply); // get the crtsh certificate id and request to download the certificate...
    else
        emit resultRawSSL(reply->readAll()); // get and send the raw certificate for analysis

    this->end(reply);
}

void Crtsh::m_getCertId(QNetworkReply *reply){

    /* a stack for storing the GumboNodes for parsing by backtracking... */
    QStack<GumboNode*> m_nodes;
    /* getting the body node... */
    GumboOutput *output = gumbo_parse(reply->readAll());
    m_nodes.push(getBody(output->root));

    /* loop to parse and obtain subdomains from the body node... */
    while(!m_nodes.isEmpty()){
        GumboNode *node = m_nodes.pop();

        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_TH && node->v.element.attributes.length == 1 && node->v.element.children.length > 0)
        {
            GumboNode *cert = static_cast<GumboNode*>(node->v.element.children.data[0]);
            if(!(cert->type == GUMBO_NODE_TEXT && QString::fromUtf8(cert->v.text.text) == "crt.sh ID"))
                continue;

            /* getting the certId */
            GumboNode *tr = node->parent;
            GumboNode *td = static_cast<GumboNode*>(tr->v.element.children.data[3]);
            GumboNode *a = static_cast<GumboNode*>(td->v.element.children.data[0]);
            GumboNode *id = static_cast<GumboNode*>(a->v.element.children.data[0]);
            if(id->type == GUMBO_NODE_TEXT){
                QString certId = QString::fromUtf8(id->v.text.text);
                ///
                /// sending request to download the ssl cert...
                ///
                QNetworkRequest request;
                QUrl url("https://crt.sh/?d="+certId);
                request.setUrl(url);
                manager->get(request);
                m_queryToGetId = false;
                break;
            }
        }

        /* push the children nodes to the m_nodes Stack and continue the loop */
        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            m_nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    /* finilizing... */
    gumbo_destroy_output(&kGumboDefaultOptions, output);
}

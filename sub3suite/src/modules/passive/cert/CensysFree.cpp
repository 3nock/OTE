#include "CensysFree.h"
#include <QStack>

/*
 * fix for ip-input and sslcert-input
 */
CensysFree::CensysFree(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_CENSYS;

    if(args.output_Hostname)
        connect(manager, &s3sNetworkAccessManager::finished, this, &CensysFree::replyFinishedSubdomain);
    if(args.output_SSL)
        connect(manager, &s3sNetworkAccessManager::finished, this, &CensysFree::replyFinishedSSL);
}
CensysFree::~CensysFree(){
    delete manager;
}

void CensysFree::start(){
    QNetworkRequest request;

    QUrl url;
    if(args.input_Domain){
        url.setUrl("https://censys.io/domain/"+target+"/table");
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_IP){
        url.setUrl("https://censys.io/ipv4/"+target+"/table");
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.input_SSL){
        url.setUrl("https://censys.io/certificates/"+target+"/table");
        request.setUrl(url);
        manager->get(request);
    }
}

void CensysFree::replyFinishedSSL(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(getBody(output->root));

    /* loop to parse and obtain subdomains from the body node */
    while(!nodes.isEmpty()){
        GumboNode *node = nodes.pop();

        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        if(node->v.element.tag == GUMBO_TAG_TBODY){
            for(unsigned int i = 0; i < node->v.element.children.length; i++)
            {
                GumboNode *tr = static_cast<GumboNode*>(node->v.element.children.data[i]);
                if(!(tr->type == GUMBO_NODE_ELEMENT && tr->v.element.tag == GUMBO_TAG_TR && tr->v.element.children.length >1))
                    continue;

                GumboNode *nameTD = static_cast<GumboNode*>(tr->v.element.children.data[1]);
                if(!(nameTD->type == GUMBO_NODE_ELEMENT && nameTD->v.element.tag == GUMBO_TAG_TD && nameTD->v.element.children.length > 2))
                    continue;

                GumboNode *small = static_cast<GumboNode*>(nameTD->v.element.children.data[2]);
                if(!(small->type == GUMBO_NODE_ELEMENT && small->v.element.tag == GUMBO_TAG_SMALL && small->v.element.children.length > 0))
                    continue;

                GumboNode* b = static_cast<GumboNode*>(small->v.element.children.data[0]);
                if(!(b->type == GUMBO_NODE_ELEMENT && b->v.element.tag == GUMBO_TAG_B && b->v.element.children.length > 0))
                    continue;

                GumboNode *name = static_cast<GumboNode*>(b->v.element.children.data[0]);
                if(!(name->type == GUMBO_NODE_TEXT))
                    continue;

                ///
                /// get the value from the second td cell if equal to any of the strings...
                ///
                if((QString::fromUtf8(name->v.text.text) == "443.https.tls.certificate.parsed.fingerprint_sha1") ||
                   (QString::fromUtf8(name->v.text.text) == "parsed.fingerprint_sha1") ||
                   (QString::fromUtf8(name->v.text.text) == "25.smtp.starttls.tls.certificate.parsed.fingerprint_sha1") ||
                   (QString::fromUtf8(name->v.text.text) == "443.https_www.tls.certificate.parsed.fingerprint_sha1"))
                {
                    GumboNode *valueTD = static_cast<GumboNode*>(tr->v.element.children.data[2]);
                    if(!(valueTD->type == GUMBO_NODE_ELEMENT && valueTD->v.element.tag == GUMBO_TAG_TD && valueTD->v.element.children.length > 0))
                        continue;

                    GumboNode *table_shortDIV = static_cast<GumboNode*>(valueTD->v.element.children.data[1]);
                    if(!(table_shortDIV->type == GUMBO_NODE_ELEMENT && table_shortDIV->v.element.tag == GUMBO_TAG_DIV && table_shortDIV->v.element.children.length > 0))
                        continue;

                    GumboNode *value = static_cast<GumboNode*>(table_shortDIV->v.element.children.data[0]);
                    if(!(value->type == GUMBO_NODE_TEXT))
                        continue;

                    QString sha1 = QString::fromUtf8(value->v.text.text);
                    /* cleaning the string */
                    sha1 = sha1.simplified();

                    /* emiting the sha1 */
                    emit resultSSL(sha1);
                    log.resultsCount++;
                }
            }
        }

        /* push the children nodes to the nodes Stack and continue the loop */
        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    this->end(reply);
}

void CensysFree::replyFinishedSubdomain(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QStack<GumboNode*> nodes;
    GumboOutput *output = gumbo_parse(reply->readAll());
    nodes.push(getBody(output->root));

    /* loop to parse and obtain subdomains from the body node */
    while(!nodes.isEmpty()){
        GumboNode *node = nodes.pop();

        if(node->type != GUMBO_NODE_ELEMENT)
            continue;

        ///
        /// tr has 2 cells <td>name</td> <td>value</td>
        ///
        if(node->v.element.tag == GUMBO_TAG_TBODY){
            for(unsigned int i = 0; i < node->v.element.children.length; i++)
            {
                GumboNode *tr = static_cast<GumboNode*>(node->v.element.children.data[i]);
                if(!(tr->type == GUMBO_NODE_ELEMENT && tr->v.element.tag == GUMBO_TAG_TR && tr->v.element.children.length >1))
                    continue;

                GumboNode *nameTD = static_cast<GumboNode*>(tr->v.element.children.data[1]);
                if(!(nameTD->type == GUMBO_NODE_ELEMENT && nameTD->v.element.tag == GUMBO_TAG_TD && nameTD->v.element.children.length > 2))
                    continue;

                GumboNode *small = static_cast<GumboNode*>(nameTD->v.element.children.data[2]);
                if(!(small->type == GUMBO_NODE_ELEMENT && small->v.element.tag == GUMBO_TAG_SMALL && small->v.element.children.length > 0))
                    continue;

                GumboNode* b = static_cast<GumboNode*>(small->v.element.children.data[0]);
                if(!(b->type == GUMBO_NODE_ELEMENT && b->v.element.tag == GUMBO_TAG_B && b->v.element.children.length > 0))
                    continue;

                GumboNode *name = static_cast<GumboNode*>(b->v.element.children.data[0]);
                if(!(name->type == GUMBO_NODE_TEXT))
                    continue;

                ///
                /// get the value from the second td cell if equal to any of the strings...
                ///
                if((QString::fromUtf8(name->v.text.text) == "25.smtp.starttls.tls.certificate.parsed.names") ||
                   (QString::fromUtf8(name->v.text.text) == "25.smtp.starttls.tls.certificate.parsed.extensions.subject_alt_name.dns_names") ||
                   (QString::fromUtf8(name->v.text.text) == "443.https.tls.certificate.parsed.names") ||
                   (QString::fromUtf8(name->v.text.text) == "443.https.tls.certificate.parsed.subject.common_name") ||
                   (QString::fromUtf8(name->v.text.text) == "443.https_www.tls.certificate.parsed.names") ||
                   (QString::fromUtf8(name->v.text.text) == "443.https.tls.certificate.parsed.extensions.subject_alt_name.dns_names") ||
                   (QString::fromUtf8(name->v.text.text) == "443.https_www.tls.certificate.parsed.extensions.subject_alt_name.dns_names") ||
                   (QString::fromUtf8(name->v.text.text) == "parsed.names") ||
                   (QString::fromUtf8(name->v.text.text) == "parsed.extensions.subject_alt_name.dns_names") ||
                   (QString::fromUtf8(name->v.text.text) == "parsed.__expanded_names"))
                {
                    GumboNode *valueTD = static_cast<GumboNode*>(tr->v.element.children.data[2]);
                    if(!(valueTD->type == GUMBO_NODE_ELEMENT && valueTD->v.element.tag == GUMBO_TAG_TD && valueTD->v.element.children.length > 0))
                        continue;

                    GumboNode *table_shortDIV = static_cast<GumboNode*>(valueTD->v.element.children.data[1]);
                    if(!(table_shortDIV->type == GUMBO_NODE_ELEMENT && table_shortDIV->v.element.tag == GUMBO_TAG_DIV && table_shortDIV->v.element.children.length > 0))
                        continue;

                    GumboNode *value = static_cast<GumboNode*>(table_shortDIV->v.element.children.data[0]);
                    if(!(value->type == GUMBO_NODE_TEXT))
                        continue;

                    QString domainNames = QString::fromUtf8(value->v.text.text);
                    domainNames = domainNames.simplified().remove(" ");
                    QStringList domainList = domainNames.split(",");

                    foreach(const QString &domain, domainList){
                        emit resultSubdomain(domain);
                        log.resultsCount++;
                    }
                }
            }
            continue;
        }

        /* push the children nodes to the nodes Stack and continue the loop */
        GumboVector *children = &node->v.element.children;
        for(unsigned int i = 0; i < children->length; i++)
            nodes.push(static_cast<GumboNode*>(children->data[i]));
    }

    gumbo_destroy_output(&kGumboDefaultOptions, output);

    this->end(reply);
}

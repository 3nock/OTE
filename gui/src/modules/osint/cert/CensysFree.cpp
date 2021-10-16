#include "CensysFree.h"

/*
 * Also has ip Search
 *
 * getting various infos,
 * eg subdomainIp smtp names list...
 *
 * available:
 *      443.https.tls.certificate.parsed.names -> domain names
 *      443.https.tls.certificate.parsed.subject.common_name -> common name
 *      443.https_www.tls.certificate.parsed.extensions.subject_alt_name.dns_names
 *      443.https_www.tls.certificate.parsed.names
 *      443.https.tls.certificate.parsed.extensions.subject_alt_name.dns_names
 *
 *      80.http.get.title
 *      80.http.get.metadata.manufacturer
 *      ports
 *      protocols
 *      443.https.tls.version
 *      443.https_www.tls.certificate.parsed.extensions.authority_info_access.issuer_urls ->certificate issuer
 *      443.https_www.tls.certificate.parsed.extensions.authority_info_access.ocsp_urls ^
 *
 *  has various interesting fields...
 *      asn, location, ip, mask, city, name, issuer etc...
 */

CensysFree::CensysFree(ScanArgs *args):
    AbstractOsintModule(args)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &CensysFree::replyFinished);
}
CensysFree::~CensysFree(){
    delete manager;
}

void CensysFree::start(){
    QNetworkRequest request;
    QUrl url("https://censys.io/domain/"+args->target+"/table");
    // QUrl url("https://censys.io/ipv4/"+target-ip+"196.44.161.196/table");
    // QUrl url("https://censys.io/ipv4/"+target-certificate-name+"196.44.161.196/table");
    request.setUrl(url);
    manager->get(request);
}

void CensysFree::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
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

void CensysFree::getSubdomains(GumboNode *node){
    if(node->type != GUMBO_NODE_ELEMENT)
        return;

    if(node->v.element.tag == GUMBO_TAG_TBODY){
        for(unsigned int i = 0; i < node->v.element.children.length; i++){
            ///
            /// tr has 2 cells <td>name</td> <td>value</td>
            ///
            GumboNode *tr = static_cast<GumboNode*>(node->v.element.children.data[i]);
            if(tr->type == GUMBO_NODE_ELEMENT && tr->v.element.tag == GUMBO_TAG_TR && tr->v.element.children.length >1){
                GumboNode *nameTD = static_cast<GumboNode*>(tr->v.element.children.data[1]);
                if(nameTD->type == GUMBO_NODE_ELEMENT && nameTD->v.element.tag == GUMBO_TAG_TD && nameTD->v.element.children.length > 2){
                    GumboNode *smsubdomainIp = static_cast<GumboNode*>(nameTD->v.element.children.data[2]);
                    if(smsubdomainIp->type == GUMBO_NODE_ELEMENT && smsubdomainIp->v.element.tag == GUMBO_TAG_SMALL && smsubdomainIp->v.element.children.length > 0){
                        GumboNode* b = static_cast<GumboNode*>(smsubdomainIp->v.element.children.data[0]);
                        if(b->type == GUMBO_NODE_ELEMENT && b->v.element.tag == GUMBO_TAG_B && b->v.element.children.length > 0){
                            GumboNode *name = static_cast<GumboNode*>(b->v.element.children.data[0]);
                            if(name->type == GUMBO_NODE_TEXT){
                                if((QString::fromUtf8(name->v.text.text) == "25.smtp.starttls.tls.certificate.parsed.names") ||
                                   (QString::fromUtf8(name->v.text.text) == "25.smtp.starttls.tls.certificate.parsed.extensions.subject_alt_name.dns_names") ||
                                   (QString::fromUtf8(name->v.text.text) == "443.https.tls.certificate.parsed.names") ||
                                   (QString::fromUtf8(name->v.text.text) == "443.https.tls.certificate.parsed.subject.common_name") ||
                                   (QString::fromUtf8(name->v.text.text) == "443.https_www.tls.certificate.parsed.names") ||
                                   (QString::fromUtf8(name->v.text.text) == "443.https.tls.certificate.parsed.extensions.subject_alt_name.dns_names") ||
                                   (QString::fromUtf8(name->v.text.text) == "443.https_www.tls.certificate.parsed.extensions.subject_alt_name.dns_names"))
                                {
                                    ///
                                    /// get the value from the second td cell...
                                    ///
                                    GumboNode *valueTD = static_cast<GumboNode*>(tr->v.element.children.data[2]);
                                    if(valueTD->type == GUMBO_NODE_ELEMENT && valueTD->v.element.tag == GUMBO_TAG_TD && valueTD->v.element.children.length > 0){
                                        GumboNode *table_shortDIV = static_cast<GumboNode*>(valueTD->v.element.children.data[1]);
                                        if(table_shortDIV->type == GUMBO_NODE_ELEMENT && table_shortDIV->v.element.tag == GUMBO_TAG_DIV && table_shortDIV->v.element.children.length > 0){
                                            GumboNode *value = static_cast<GumboNode*>(table_shortDIV->v.element.children.data[0]);
                                            if(value->type == GUMBO_NODE_TEXT){
                                                QString smtpNames = QString::fromUtf8(value->v.text.text);
                                                smtpNames = smtpNames.simplified().remove(" ");
                                                QStringList smtpNamesList = smtpNames.split(",");
                                                foreach(const QString &smtp, smtpNamesList)
                                                    emit subdomain(smtp);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        return;
    }

    GumboVector *children = &node->v.element.children;
    for(unsigned int i = 0; i < children->length; i++)
        getSubdomains(static_cast<GumboNode*>(children->data[i]));
}

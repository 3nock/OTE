#include "TruMail.h"

#define EMAIL 0


TruMail::TruMail(ScanArgs args): AbstractOsintModule(args)
{
    manager = new s3sNetworkAccessManager(this, args.config->timeout, args.config->setTimeout);
    log.moduleName = OSINT_MODULE_TRUMAIL;

    if(args.output_Raw)
        connect(manager, &s3sNetworkAccessManager::finished, this, &TruMail::replyFinishedRawJson);
    if(args.output_EnumEmail)
        connect(manager, &s3sNetworkAccessManager::finished, this, &TruMail::replyFinishedEnumEmail);
}
TruMail::~TruMail(){
    delete manager;
}

void TruMail::start(){
    QNetworkRequest request;
    QUrl url;

    if(args.output_Raw){
        switch(args.raw_query_id){
        case EMAIL:
            url.setUrl("https://api.trumail.io/v2/lookups/json?email="+target);
            break;
        }
        request.setUrl(url);
        manager->get(request);
        return;
    }

    if(args.output_EnumEmail){
        url.setUrl("https://api.trumail.io/v2/lookups/json?email="+target);
        request.setUrl(url);
        manager->get(request);
    }
}

void TruMail::replyFinishedEnumEmail(QNetworkReply *reply){
    if(reply->error()){
        this->onError(reply);
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject mainObj = document.object();

    if(mainObj["hostExists"].toBool()){
        s3s_struct::Email email;
        email.email = target;
        email.domain = mainObj["domain"].toString();
        email.free = mainObj["free"].toBool();
        email.hostExists = mainObj["hostExists"].toBool();
        email.disposable = mainObj["disposable"].toBool();
        email.deliverable = mainObj["deliverable"].toBool();

        emit resultEnumEmail(email);
    }

    this->end(reply);
}

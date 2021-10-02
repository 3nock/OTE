#include "ArchiveToday.h"

/*
 * returns subdomains and their screenshot
 */

/*
 * Handling possible redirect
 *
     * Reply is finished!
     * We'll ask for the reply about the Redirection attribute
     * http://doc.trolltech.com/qnetworkrequest.html#Attribute-enum

    QVariant possibleRedirectUrl =
             reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    // We'll deduct if the redirection is valid in the redirectUrl function
    _urlRedirectedTo = this->redirectUrl(possibleRedirectUrl.toUrl(),
                                         _urlRedirectedTo);

    // If the URL is not empty, we're being redirected.
    if(!_urlRedirectedTo.isEmpty()) {
        QString text = QString("QNAMRedirect::replyFinished: Redirected to ")
                              .append(_urlRedirectedTo.toString());
        this->_textContainer->setText(text);

        // We'll do another request to the redirection url.
        this->_qnam->get(QNetworkRequest(_urlRedirectedTo));
    }
    else {
        //
         * We weren't redirected anymore
         * so we arrived to the final destination...

        QString text = QString("QNAMRedirect::replyFinished: Arrived to ")
                              .append(reply->url().toString());
        this->_textContainer->setText(text);
        // ...so this can be cleared.
        _urlRedirectedTo.clear();
    }
    // Clean up.
    reply->deleteLater();
}

QUrl QNAMRedirect::redirectUrl(const QUrl& possibleRedirectUrl,
                               const QUrl& oldRedirectUrl) const {
    QUrl redirectUrl;

     * Check if the URL is empty and
     * that we aren't being fooled into a infinite redirect loop.
     * We could also keep track of how many redirects we have been to
     * and set a limit to it, but we'll leave that to you.

    if(!possibleRedirectUrl.isEmpty() &&
       possibleRedirectUrl != oldRedirectUrl) {
        redirectUrl = possibleRedirectUrl;
    }
    return redirectUrl;
}
*/

ArchiveToday::ArchiveToday(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &ArchiveToday::replyFinished);
}
ArchiveToday::~ArchiveToday(){
    delete manager;
}

void ArchiveToday::start(){
    QNetworkRequest request;
    QUrl url("http://archive.is/*."+target);
    request.setUrl(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    manager->get(request);
}

void ArchiveToday::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError){
        emit scanResults(QString::fromUtf8(reply->readAll()));
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

void ArchiveToday::getSubdomains(GumboNode *node){
    Q_UNUSED(node);
}

void ArchiveToday::getScreenshots(GumboNode *node){
    Q_UNUSED(node);
}


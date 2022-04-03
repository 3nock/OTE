#include "UpdateChecker.h"

#include "src/utils/Config.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QMessageBox>
#include <QNetworkReply>
#include <QIcon>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


UpdateChecker::UpdateChecker(QWidget* parent):
    m_parent(parent)
{
    connect(this, &UpdateChecker::finished, this, &UpdateChecker::onFinished);
}

void UpdateChecker::checkForUpdates(){
    get(QNetworkRequest(QUrl("https://api.github.com/repos/3nock/sub3suite/releases/latest")));
    qDebug() << "Checking for Updates...";
}

void UpdateChecker::checkForUpdates_onStart(){
    get(QNetworkRequest(QUrl("https://api.github.com/repos/3nock/sub3suite/releases/latest")));
    qDebug() << "Checking for Updates...";
    onStart = true;
}

void UpdateChecker::onFinished(QNetworkReply* reply){
    if(onStart){
        if(reply->error() == QNetworkReply::NoError){
            QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
            QJsonObject mainObj = document.object();

            QDateTime serverTime = QDateTime::fromString(mainObj["published_at"].toString(), Qt::ISODate);
            if(serverTime.isValid())
            {
                QString url = mainObj["assets"].toArray().at(0).toObject()["browser_download_url"].toString();
                QDateTime build = QDateTime::fromString(CONFIG.value("build_date").toString(), "yyyy-MM-dd");

                if(serverTime.date() > build.date()){
                    QString info = QString(tr("New build %1 available!<br>Download <a href=\"https://github.com/3nock/sub3suite/releases\">here</a><br><br>You are now on build %2")).arg(serverTime.toString()).arg(build.toString());
                    QMessageBox::information(m_parent, tr("New Update!"), info);
                    qDebug() << "New Updates Found!";
                }else
                    qDebug() << "UPDATE CHECK: No New Updates...";
            }else
                qWarning() << "UPDATE CHECK: Error parsing Server response...";
        }else
            qWarning() << "UPDATE CHECK: Network Error...";
        onStart = false;
    }
    else{
        switch (reply->error()) {
        case QNetworkReply::NoError:
        {
            QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
            QJsonObject mainObj = document.object();

            QDateTime serverTime = QDateTime::fromString(mainObj["published_at"].toString(), Qt::ISODate);
            if(!serverTime.isValid())
            {
                QMessageBox::warning(m_parent, tr("Error!"), tr("File on server could not be parsed..."));
                return;
            }

            QString url = mainObj["assets"].toArray().at(0).toObject()["browser_download_url"].toString();
            QDateTime build = QDateTime::fromString(CONFIG.value("build_date").toString(), "yyyy-MM-dd");

            QString info;
            if(serverTime.date() > build.date())
                info = QString(tr("New build %1 available!<br>Download <a href=\"https://github.com/3nock/sub3suite/releases\">here</a><br><br>You are now on build %2")).arg(serverTime.toString()).arg(build.toString());
            else if(serverTime.date() < build.date())
                info = QString(tr("You have a development build (%1) of sub3suite!")).arg(build.toString());
            else
                info = QString(tr("You have the latest build (%1) of sub3suite!")).arg(build.toString());

            QMessageBox::information(m_parent, tr("Update Info"), info);
        }
            break;
        default:
            QMessageBox::warning(m_parent, tr("Network Error!"), reply->errorString());
            break;
        }
    }
}

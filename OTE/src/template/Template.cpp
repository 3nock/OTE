//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "Template.h"

#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDomComment>

#include "src/core/Config.h"
#include "src/template/Endpoint.h"
#include "src/template/Extractor.h"

namespace OTE {

QList<Template*> gTemplates;
InputOutput gInputOutput;

Template::Template()
{

}

Template::~Template()
{
    foreach(Endpoint *endpoint, endpoints)
        delete endpoint;
}

bool Template::hasExtractors() const
{
    foreach(OTE::Endpoint *endpoint, endpoints)
    {
        if(endpoint->extractors.isEmpty())
            continue;

        return true;
    }
    return false;
}

Template *Template::FromJSON(const QByteArray &json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json);
    if(!doc.isObject())
        return nullptr;

    Template *tmplt = new Template;
    QJsonObject templateObj = doc.object();

    // author
    QJsonObject authorObj = templateObj["author"].toObject();
    tmplt->author.name = authorObj["name"].toString();
    tmplt->author.social = authorObj["social"].toString();

    // info
    QJsonObject infoObj = templateObj["info"].toObject();
    tmplt->info.name = infoObj["name"].toString();
    tmplt->info.link = infoObj["link"].toString();
    tmplt->info.documentation = infoObj["documentation"].toString();
    tmplt->info.description = infoObj["description"].toString();

    // authentication
    QJsonObject authObj = templateObj["authentication"].toObject();
    tmplt->authentication.link = authObj["link"].toString();
    tmplt->authentication.uses_id = authObj["uses_id"].toBool();
    tmplt->authentication.uses_key = authObj["uses_key"].toBool();

    QString authType = authObj["authType"].toString();
    if(authType.compare("QUERY", Qt::CaseInsensitive) == 0)
        tmplt->authentication.authType = AUTH_TYPE::QUERY;
    else if(authType.compare("HEADER", Qt::CaseInsensitive) == 0)
        tmplt->authentication.authType = AUTH_TYPE::HEADER;
    else if(authType.compare("BASIC", Qt::CaseInsensitive) == 0)
        tmplt->authentication.authType = AUTH_TYPE::BASIC;
    else if(authType.compare("BEARER", Qt::CaseInsensitive) == 0)
            tmplt->authentication.authType = AUTH_TYPE::BEARER;

    tmplt->authentication.authHeader = authObj["authHeader"].toString();

    // endpoints
    foreach(const QJsonValue &value, templateObj["endpoints"].toArray())
    {
        Endpoint *endpoint = new Endpoint(tmplt);
        QJsonObject endpointObj = value.toObject();

        endpoint->name = endpointObj["name"].toString();
        endpoint->description = endpointObj["description"].toString();
        endpoint->link = endpointObj["link"].toString();
        endpoint->postData = endpointObj["postData"].toString();

        if(endpointObj["resquestType"].toString().compare("POST", Qt::CaseInsensitive) == 0)
            endpoint->requestType = Endpoint::REQUEST_TYPE::POST;
        else
            endpoint->requestType = Endpoint::REQUEST_TYPE::GET;

        if(endpointObj["responseType"].toString().compare("XML", Qt::CaseInsensitive) == 0)
            endpoint->responseType = Endpoint::RESPONSE_TYPE::XML;
        else
            endpoint->responseType = Endpoint::RESPONSE_TYPE::JSON;

        foreach(const QJsonValue &input, endpointObj["inputTypes"].toArray())
            endpoint->inputTypes << input.toString();

        foreach(const QJsonValue &value, endpointObj["extractors"].toArray())
        {
            QJsonObject extractorObj = value.toObject();

            Extractor *extractor = Extractor::createExtractor(endpoint, extractorObj["script"].toString().toUtf8());
            if(extractor == nullptr)
                continue;

            extractor->name = extractorObj["name"].toString();
            endpoint->extractors << extractor;
        }

        tmplt->endpoints << endpoint;
    }

    return tmplt;
}

QByteArray Template::ToJSON(Template *tmplt)
{
    // author
    QJsonObject authorObj;
    authorObj.insert("name", tmplt->author.name);
    authorObj.insert("social", tmplt->author.social);

    // info
    QJsonObject infoObj;
    infoObj.insert("name", tmplt->info.name);
    infoObj.insert("link", tmplt->info.link);
    infoObj.insert("documentation", tmplt->info.documentation);
    infoObj.insert("description", tmplt->info.description);

    // authentication
    QJsonObject authObj;
    authObj.insert("link", tmplt->authentication.link);
    authObj.insert("uses_id", tmplt->authentication.uses_id);
    authObj.insert("uses_key", tmplt->authentication.uses_key);

    if(tmplt->authentication.authType == AUTH_TYPE::QUERY)
        authObj.insert("authType", "QUERY");
    else if(tmplt->authentication.authType == AUTH_TYPE::HEADER)
        authObj.insert("authType", "HEADER");
    else if(tmplt->authentication.authType == AUTH_TYPE::BEARER)
        authObj.insert("authType", "BEARER");
    else if(tmplt->authentication.authType == AUTH_TYPE::BASIC)
        authObj.insert("authType", "BASIC");
    else
        authObj.insert("authType", "");

    authObj.insert("authHeader", tmplt->authentication.authHeader);

    // endpoints
    QJsonArray endpointsArr;
    foreach(Endpoint *endpoint, tmplt->endpoints)
    {
        QJsonObject endpointObj;

        endpointObj.insert("name", endpoint->name);
        endpointObj.insert("description", endpoint->description);
        endpointObj.insert("link", endpoint->link);
        endpointObj.insert("postData", endpoint->postData);

        if(endpoint->requestType == Endpoint::REQUEST_TYPE::POST)
            endpointObj.insert("resquestType", "POST");
        else
            endpointObj.insert("resquestType", "GET");

        if(endpoint->responseType == Endpoint::RESPONSE_TYPE::XML)
            endpointObj.insert("responseType", "XML");
        else
            endpointObj.insert("responseType", "JSON");

        QJsonArray inputTypesArr;
        foreach(const QString &inputType, endpoint->inputTypes)
            inputTypesArr.append(inputType);
        endpointObj.insert("inputTypes", inputTypesArr);

        QJsonArray extractorsArr;
        foreach(Extractor *extractor, endpoint->extractors)
        {
            QJsonObject extractorObj;
            extractorObj.insert("name", extractor->name);
            extractorObj.insert("script", QString::fromUtf8(extractor->script));
            extractorsArr.append(extractorObj);
        }
        endpointObj.insert("extractors", extractorsArr);

        endpointsArr.append(endpointObj);
    }

    QJsonObject tmpltObj;
    tmpltObj.insert("author", authorObj);
    tmpltObj.insert("info", infoObj);
    tmpltObj.insert("authentication", authObj);
    tmpltObj.insert("endpoints", endpointsArr);

    QJsonDocument doc;
    doc.setObject(tmpltObj);
    return doc.toJson();
}

void Template::SaveTemplate(Template *tmplt)
{
    QString filename(QCoreApplication::applicationDirPath());
    filename.append("/templates/").append(tmplt->info.name).append(".json");

    QFile file(filename);
    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        file.write(Template::ToJSON(tmplt));
        file.close();
    }
    else
        qWarning() << "Failed to open file: " << filename << " for saving";
}

void Template::loadTemplates()
{
    QString location(QCoreApplication::applicationDirPath());
    location.append("/templates/");

    for (const QFileInfo &fileInfo : QDir(location).entryInfoList(QDir::Files))
    {
        if(fileInfo.completeSuffix().compare("json", Qt::CaseInsensitive) != 0)
            continue;

        QFile file(fileInfo.absoluteFilePath());

        if(file.open(QIODevice::ReadOnly))
        {
            Template *tmplt = Template::FromJSON(file.readAll());

            if(tmplt)
            {
                if(OTE::gConfig.apiKeys.contains(tmplt->info.name))
                {
                    QPair<QString,QString> user_key_pair = OTE::gConfig.apiKeys[tmplt->info.name];
                    tmplt->authentication.id = user_key_pair.first;
                    tmplt->authentication.key = user_key_pair.second;
                }

                foreach(OTE::Endpoint *endpoint, tmplt->endpoints)
                {
                    if(endpoint->extractors.isEmpty() || endpoint->inputTypes.isEmpty())
                        continue;

                    foreach(const QString &inputType, endpoint->inputTypes)
                    {
                        if(gInputOutput.inputTypes.contains(inputType))
                            continue;
                        gInputOutput.inputTypes << inputType;
                    }

                    foreach(OTE::Extractor *extractor, endpoint->extractors)
                    {
                        if(gInputOutput.outputTypes.contains(extractor->name))
                            continue;
                        gInputOutput.outputTypes << extractor->name;
                    }
                }

                qSort(gInputOutput.inputTypes);
                qSort(gInputOutput.outputTypes);
                //std::sort(gInputOutput.inputTypes.begin(), gInputOutput.inputTypes.end());
                //std::sort(gInputOutput.outputTypes.begin(), gInputOutput.outputTypes.end());

                gTemplates << tmplt;
            }

            file.close();
        }
    }
}


TemplateItem::TemplateItem(Template *_tmplt):
    QStandardItem(_tmplt->info.name),
    tmplt(_tmplt)
{
    configureIcon();

    auto f = font();
    f.setBold(true);
    setFont(f);
}

TemplateItem::~TemplateItem()
{
    takeColumn(0);
    while(rowCount())
        removeRow(0);

    foreach(auto item, endpointItems)
        delete item;
}

void TemplateItem::configureIcon()
{
    if(tmplt->authentication.uses_key)
    {
        if(tmplt->authentication.key.isEmpty())
        {
            if(OTE::gConfig.general.theme == OTE::THEME::DARK)
                setIcon(QIcon(":/icons/res/icons/locked.png"));
            else
                setIcon(QIcon(":/icons/res/icons/locked_black.png"));
        }
        else
        {
            if(OTE::gConfig.general.theme == OTE::THEME::DARK)
                setIcon(QIcon(":/icons/res/icons/unlocked.png"));
            else
                setIcon(QIcon(":/icons/res/icons/unlocked_black.png"));
        }
    }
    else
    {
        if(OTE::gConfig.general.theme == OTE::THEME::DARK)
            setIcon(QIcon(":/icons/res/icons/unlocked.png"));
        else
            setIcon(QIcon(":/icons/res/icons/unlocked_black.png"));
    }
}

void TemplateItem::configureExtractors()
{
    foreach(OTE::Endpoint *endpoint, tmplt->endpoints)
    {
        if(endpoint->inputTypes.isEmpty() || endpoint->extractors.isEmpty())
            continue;

        EndpointItem *item = new EndpointItem(endpoint);
        item->setCheckable(true);
        endpointItems << item;
    }
}

bool TemplateItem::configureExtractors(const QString &input, const QString &output)
{
    bool hasExtractor = false;

    takeColumn(0);
    while(rowCount())
        removeRow(0);

    foreach(EndpointItem *endpointItem, endpointItems)
    {
        endpointItem->setCheckState(Qt::Unchecked);

        if(endpointItem->endpoint->extractors.isEmpty())
            continue;

        if(!endpointItem->endpoint->inputTypes.contains(input))
            continue;

        foreach(Extractor *extractor, endpointItem->endpoint->extractors)
        {
            if(extractor->name.compare(output, Qt::CaseInsensitive) == 0)
            {
                appendRow(endpointItem);
                hasExtractor = true;
                break;
            }
        }
    }
    return hasExtractor;
}

QList<Endpoint *> TemplateItem::getCheckedEndpoints() const
{
    QList<OTE::Endpoint*> checkedEndpoints;
    foreach(EndpointItem *endpointItem, endpointItems)
    {
        if(endpointItem->checkState() == Qt::Checked)
            checkedEndpoints << endpointItem->endpoint;
    }

    return checkedEndpoints;
}

} // namespace OTE

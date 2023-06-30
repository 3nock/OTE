//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "Extractor.h"

#include <QDebug>

#include "src/template/Endpoint.h"
#include "src/template/Template.h"

OTE::Extractor::Extractor(OTE::Endpoint *_endpoint):
    endpoint(_endpoint)
{}

OTE::Extractor::~Extractor()
{}

OTE::Extractor *OTE::Extractor::createExtractor(OTE::Endpoint *endpoint, const QByteArray &src)
{
    if(endpoint->responseType == OTE::Endpoint::RESPONSE_TYPE::XML)
    {
        QDomDocument doc;
        if(doc.setContent(src))
        {
            Extractor *extractor = new Extractor(endpoint);

            extractor->extractorXmlDoc = doc;
            extractor->script = doc.toByteArray(0);
            return extractor;
        }
        else
        {
            qWarning() << "Failed to create xml Extractor for Template: "
                       << endpoint->tmplt->info.name
                       << " Endpoint: "
                       << endpoint->name;
        }
    }
    else if(endpoint->responseType == OTE::Endpoint::RESPONSE_TYPE::JSON)
    {
        QJsonDocument doc = QJsonDocument::fromJson(src);
        if(doc.isObject() || doc.isArray())
        {
            Extractor *extractor = new Extractor(endpoint);

            extractor->extractorJsonDoc = doc;
            extractor->script = doc.toJson(QJsonDocument::Compact);
            return extractor;
        }
        else
        {
            qWarning() << "Failed to create json Extractor for Template: "
                       << endpoint->tmplt->info.name
                       << " Endpoint: "
                       << endpoint->name;
        }
    }

    return nullptr;
}

bool OTE::Extractor::createExtractor(OTE::Extractor *extractor, const QByteArray &src)
{
    if(extractor->endpoint->responseType == OTE::Endpoint::RESPONSE_TYPE::XML)
    {
        QDomDocument doc;
        if(doc.setContent(src))
        {
            extractor->extractorXmlDoc = doc;
            extractor->script = doc.toByteArray(0);
            return true;
        }
        else
        {
            qWarning() << "Failed to load xml Extractor for Template: "
                       << extractor->endpoint->tmplt->info.name
                       << " Endpoint: "
                       << extractor->endpoint->name;
        }
    }
    else if(extractor->endpoint->responseType == OTE::Endpoint::RESPONSE_TYPE::JSON)
    {
        QJsonDocument doc = QJsonDocument::fromJson(src);
        if(doc.isObject() || doc.isArray())
        {
            extractor->extractorJsonDoc = doc;
            extractor->script = doc.toJson(QJsonDocument::Compact);
            return true;
        }
        else
        {
            qWarning() << "Failed to load json Extractor for Template: "
                       << extractor->endpoint->tmplt->info.name
                       << " Endpoint: "
                       << extractor->endpoint->name;
        }
    }

    return false;
}

QStringList OTE::Extractor::extract(const QByteArray &src)
{
    QStringList results;

    if(endpoint->responseType == OTE::Endpoint::RESPONSE_TYPE::JSON)
    {
        QJsonDocument resultJsonDoc  = QJsonDocument::fromJson(src);

        if(extractorJsonDoc.isArray() && resultJsonDoc.isArray())
        {
            extractFromJsonArray(extractorJsonDoc.array(), resultJsonDoc.array(), results);
        }
        else if(extractorJsonDoc.isObject() && resultJsonDoc.isObject())
        {
            extractFromJsonObject(extractorJsonDoc.object(), resultJsonDoc.object(), results);
        }
    }
    else if(endpoint->responseType == OTE::Endpoint::RESPONSE_TYPE::XML)
    {
        QDomDocument resultXmlDoc;
        if(resultXmlDoc.setContent(src))
        {
            extractFromXmlNode(extractorXmlDoc.documentElement(), resultXmlDoc.documentElement(), results);
        }
    }

    return results;
}

void OTE::Extractor::extractFromJsonObject(const QJsonObject &extractorParentObj, const QJsonObject &resultParentObj, QStringList &results)
{
    if(extractorParentObj.isEmpty() || resultParentObj.isEmpty())
        return;

    foreach(const QString &extractorKey, extractorParentObj.keys())
    {
        if(extractorKey.compare("$$") == 0)
        {
            QJsonValue extractorValue = extractorParentObj[extractorKey];

            foreach(const QString &resultKey, resultParentObj.keys())
            {
                results << resultKey;

                QJsonValue resultValue = resultParentObj[resultKey];
                if(extractorValue.isObject() && resultValue.isObject())
                    extractFromJsonObject(extractorValue.toObject(), resultValue.toObject(), results);

                else if(extractorValue.isArray() && resultValue.isArray())
                    extractFromJsonArray(extractorValue.toArray(), resultValue.toArray(), results);
            }
        }
        else if((extractorKey.length() > 4) && (extractorKey.startsWith("$$") && extractorKey.endsWith("$$")))
        {
            QString resultKey = extractorKey.mid(2, extractorKey.length()-4);

            QJsonValue resultValue = resultParentObj[resultKey];
            QJsonValue extractorValue = extractorParentObj[extractorKey];

            if(extractorValue.isObject() && resultValue.isObject())
            {
                QJsonDocument doc;
                doc.setObject(resultValue.toObject());
                results << doc.toJson();

                extractFromJsonObject(extractorValue.toObject(), resultValue.toObject(), results);
            }
            else if(extractorValue.isArray() && resultValue.isObject())
            {
                QJsonDocument doc;
                doc.setArray(resultValue.toArray());
                results << doc.toJson();

                extractFromJsonArray(extractorValue.toArray(), resultValue.toArray(), results);
            }
            else if(resultValue.isString())
            {
                results << resultValue.toString();
            }
            else if(resultValue.isDouble())
            {
                results << QString::number(resultValue.toDouble());
            }
        }
        else if(extractorKey.compare("*") == 0)
        {
            QJsonValue extractorValue = extractorParentObj[extractorKey];
            foreach(const QString &resultKey, resultParentObj.keys())
            {
                QJsonValue resultValue = resultParentObj[resultKey];

                if(extractorValue.isObject() && resultValue.isObject())
                    extractFromJsonObject(extractorValue.toObject(), resultValue.toObject(), results);

                else if(extractorValue.isArray() && resultValue.isArray())
                    extractFromJsonArray(extractorValue.toArray(), resultValue.toArray(), results);
            }
        }
        else
        {
            QJsonValue extractorValue = extractorParentObj[extractorKey];
            QJsonValue resultValue = resultParentObj[extractorKey];

            if(extractorValue.isObject() && resultValue.isObject())
                extractFromJsonObject(extractorValue.toObject(), resultValue.toObject(), results);

            else if(extractorValue.isArray() && resultValue.isArray())
                extractFromJsonArray(extractorValue.toArray(), resultValue.toArray(), results);
        }
    }
}

void OTE::Extractor::extractFromJsonArray(const QJsonArray &extractorParentArr, const QJsonArray &resultParentArr, QStringList &results)
{
    if(extractorParentArr.isEmpty() || resultParentArr.isEmpty())
        return;

    foreach(const QJsonValue &extractorValue, extractorParentArr)
    {
        foreach(const QJsonValue &resultValue, resultParentArr)
        {
            if(extractorValue.isString())
            {
                if(extractorValue.toString().compare("$$") == 0)
                {
                    if(resultValue.isString())
                    {
                        results << resultValue.toString();
                    }
                    else if(resultValue.isDouble())
                    {
                        results << QString::number(resultValue.toDouble());
                    }
                    else if(resultValue.isObject())
                    {
                        QJsonDocument doc;
                        doc.setObject(resultValue.toObject());
                        results << doc.toJson();
                    }
                    else if(resultValue.isArray())
                    {
                        QJsonDocument doc;
                        doc.setArray(resultValue.toArray());
                        results << doc.toJson();
                    }
                }
            }
            else if(extractorValue.isObject() && resultValue.isObject())
            {
                extractFromJsonObject(extractorValue.toObject(), resultValue.toObject(), results);
            }
            else if(extractorValue.isArray() && resultValue.isArray())
            {
                extractFromJsonArray(extractorValue.toArray(), resultValue.toArray(), results);
            }
        }
    }
}

void OTE::Extractor::extractFromXmlNode(const QDomNode &extractParentNode, const QDomNode &resultParentNode, QStringList &results)
{
    if(extractParentNode.isText())
    {
        if(extractParentNode.toText().data().compare("$$") == 0)
            results << resultParentNode.toText().data();
    }
    else if(extractParentNode.isElement())
    {
        for(QDomNode extractChildNode = extractParentNode.firstChild(); !extractChildNode.isNull(); extractChildNode = extractChildNode.nextSibling())
        {
            for(QDomNode resultChildNode = resultParentNode.firstChild(); !resultChildNode.isNull(); resultChildNode = resultChildNode.nextSibling())
            {
                if(extractChildNode.isElement() && resultChildNode.isElement())
                {
                    QDomElement extractElement = extractChildNode.toElement();
                    QDomElement resultElemet = resultChildNode.toElement();

                    if(extractElement.tagName().compare(resultElemet.tagName(), Qt::CaseInsensitive) == 0)
                        extractFromXmlNode(extractElement, resultElemet, results);
                }
            }
        }
    }
}

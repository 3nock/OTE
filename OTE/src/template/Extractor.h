//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef TEMPLATE_EXTRACTOR_H
#define TEMPLATE_EXTRACTOR_H

#include <QByteArray>
#include <QDomDocument>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardItem>
#include <QString>
#include <QStringList>

namespace OTE {

class Endpoint;

class Extractor
{
public:
    explicit Extractor(Endpoint *_endpoint);
    ~Extractor();

public:
    QString name;
    QByteArray script;
    Endpoint *endpoint;

    QJsonDocument extractorJsonDoc;
    QDomDocument extractorXmlDoc;

    static bool createExtractor(Extractor* extractor, const QByteArray &src);
    static Extractor *createExtractor(Endpoint *endpoint, const QByteArray &src);

    QStringList extract(const QByteArray &src);

private:
    static void extractFromJsonObject(const QJsonObject &extractorParentObj, const QJsonObject &resultParentObj, QStringList &results);
    static void extractFromJsonArray(const QJsonArray &extractorParentArr, const QJsonArray &resultParentArr, QStringList &results);
    static void extractFromXmlNode(const QDomNode &extractParentNode, const QDomNode &resultParentNode, QStringList &results);
};

class ExtractorItem: public QStandardItem
{
public:
    explicit ExtractorItem(Extractor *_extractor):
        QStandardItem(_extractor->name),
        extractor(_extractor)
    {}

    Extractor *extractor;
};

}

#endif // TEMPLATE_EXTRACTOR_H

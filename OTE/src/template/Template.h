//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef TEMPLATE_TEMPLATE_H
#define TEMPLATE_TEMPLATE_H

#include <QList>
#include <QMap>
#include <QSet>
#include <QStandardItem>
#include <QString>

namespace OTE {

class Endpoint;
class EndpointItem;

class Template
{
public:
    Template();
    ~Template();

    enum AUTH_TYPE {
        QUERY = 0,
        HEADER,
        BASIC,
        BEARER
    };

    struct {
        QString name;
        QString social;
    }author;

    struct {
        QString name;
        QString link;
        QString documentation;
        QString description;
    }info;

    struct {
        bool uses_id = false;
        bool uses_key = false;
        QString id;
        QString key;
        QString link;
        AUTH_TYPE authType = AUTH_TYPE::QUERY;
        QString authHeader;
    }authentication;

    QList<Endpoint*> endpoints;

public:
    bool hasExtractors()const;
    static Template *FromJSON(const QByteArray &json);
    static QByteArray ToJSON(Template *tmplt);
    static void SaveTemplate(Template *tmplt);
    static void loadTemplates();
};

class TemplateItem: public QStandardItem
{
public:
    explicit TemplateItem(OTE::Template *_tmplt);
    ~TemplateItem();

public:
    OTE::Template *tmplt;
    QList<EndpointItem*> endpointItems;

    void configureIcon();
    void configureExtractors();
    bool configureExtractors(const QString &input, const QString &output);
    QList<Endpoint*> getCheckedEndpoints() const;
};

struct InputOutput {
    QStringList inputTypes;
    QStringList outputTypes;
};

extern QList<Template*> gTemplates;
extern InputOutput gInputOutput;

} // namespace OTE

#endif // TEMPLATE_TEMPLATE_H

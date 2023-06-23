//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef TEMPLATE_ENDPOINT_H
#define TEMPLATE_ENDPOINT_H

#include <QStandardItem>
#include <QString>
#include <QStringList>

namespace OTE {

class Template;
class Extractor;

class Endpoint
{
public:
    explicit Endpoint(Template *_tmplt);
    ~Endpoint();

    enum REQUEST_TYPE {
        GET = 0,
        POST = 1
    };
    enum RESPONSE_TYPE {
        JSON = 0,
        XML = 1
    };

    Template *tmplt;
    QString name;
    QString link;
    QString postData;
    QString description;
    QStringList inputTypes;
    REQUEST_TYPE requestType = REQUEST_TYPE::GET;
    RESPONSE_TYPE responseType = RESPONSE_TYPE::JSON;
    QList<Extractor*> extractors;
};

class EndpointItem: public QStandardItem
{
public:
    explicit EndpointItem(Endpoint *_endpoint):
        QStandardItem(_endpoint->name),
        endpoint(_endpoint)
    {}

    Endpoint *endpoint;
};

}

#endif // TEMPLATE_ENDPOINT_H

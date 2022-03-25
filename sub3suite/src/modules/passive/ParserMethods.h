#ifndef PARSERMETHODS_H
#define PARSERMETHODS_H

#include <QSet>
#include <QStack>
#include <QNetworkReply>
#include "gumbo-parser/src/gumbo.h"


GumboNode* getBody(GumboNode *node);

QString getTitle(QNetworkReply *reply);

QSet<QString> getLinks(QNetworkReply *reply);


#endif // PARSERMETHODS_H

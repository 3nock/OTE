//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef CORE_UPDATECHECKER_H
#define CORE_UPDATECHECKER_H

#include <QNetworkAccessManager>

namespace OTE {

class UpdateChecker : public QNetworkAccessManager
{
    Q_OBJECT

public:
    UpdateChecker(QWidget* parent = nullptr);
    void checkForUpdates();
    void checkForUpdates_onStart();

private slots:
    void onFinished(QNetworkReply* reply);

private:
    QWidget* mParent;
    bool onStart = false;
};

} // namespace OTE

#endif // CORE_UPDATECHECKER_H

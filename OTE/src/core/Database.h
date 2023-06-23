//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef CORE_DATABASE_H
#define CORE_DATABASE_H

#include <QDebug>
#include <QString>
#include <QTemporaryFile>

#include "sqlite3.h"

namespace OTE {

struct ExplorerResult {
    QString tmplt;
    QString endpoint;
    QString target;
};

class Database: public QObject
{
public:
    Database(QObject *parent = nullptr);
    ~Database();

public:
    static sqlite3 *db;
    static QString db_name;

    void openTempDB();
    static void openDatabase(const QString &database_name);
    Q_REQUIRED_RESULT static bool saveDatabase(const QString &newDatabase);
    static void createExplorerTables();
    static void createExtractorTables();

    static void clear();
    static void vaccum();
    static void optimize();
    static void setFlags();
    static void checkpoint();

    // transactions
    static void beginTransaction();
    static void endTransaction();

    // insert
    Q_REQUIRED_RESULT static bool insertExplorerResult(const QString &tmplt, const QString &endpoint, const QString &target, const QByteArray &result);
    Q_REQUIRED_RESULT static QStringList insertExtractorResult(const QString &type, const QStringList &results);

    // get
    Q_REQUIRED_RESULT static QByteArray getExplorerResult(const QString &tmplt, const QString &endpoint, const QString &target);

    // remove
    Q_REQUIRED_RESULT static bool removeExplorerResult(const QString &tmplt, const QString &endpoint, const QString &target);
    Q_REQUIRED_RESULT static bool removeExtractorResult(const QString &type, const QString &result);

    // load
    Q_REQUIRED_RESULT static QList<ExplorerResult> loadExplorerResults();
    Q_REQUIRED_RESULT static QMap<QString, QStringList> loadExtractorResults();

private:
    QTemporaryFile *mTempDB;

    Q_REQUIRED_RESULT static QByteArray targetFromExplorer(const ExplorerResult &result);
    Q_REQUIRED_RESULT static ExplorerResult targetToExplorer(const QByteArray &target);
};

} // namespace OTE

#endif // CORE_DATABASE_H

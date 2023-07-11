//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "Database.h"

#include <QFileInfo>

#include "src/template/Template.h"

namespace OTE {

sqlite3* Database::db = nullptr;
QString Database::db_name = "";

Database::Database(QObject *parent):
    QObject(parent),
    mTempDB(new QTemporaryFile(this))
{
    // open temporary database
    openTempDB();
}

Database::~Database()
{
    // close sqlite database
    if(db)
        sqlite3_close(db);

    delete mTempDB;
}

void Database::openTempDB()
{
    if(mTempDB->isOpen())
    {
        qDebug() << "Using Temporary database: " << mTempDB->fileName();
        Database::openDatabase(mTempDB->fileName());
    }
    else if(mTempDB->open())
    {
        qDebug() << "Opened Temporary database: " << mTempDB->fileName();
        Database::openDatabase(mTempDB->fileName());
    }
    else
        qCritical() << "Error: Couldn't open temporary database";
}

void Database::openDatabase(const QString &database)
{
    // Re-analyze & close current database
    if(db)
    {
        optimize();
        sqlite3_close(db);
        db = nullptr;
    }

    // open new database
    db_name = database;
    if(sqlite3_open(database.toUtf8(), &db))
    {
        qCritical() << "Cannot open database: " << sqlite3_errmsg(db);
        sqlite3_close(db);
    }

    // performance tunning
    setFlags();

    // create tables
    createExplorerTables();
    createExtractorTables();
}

bool Database::saveDatabase(const QString &newDatabase)
{
    sqlite3_stmt *stmt = nullptr;
    if(sqlite3_prepare_v2(db, "VACUUM main INTO ?", -1, &stmt, nullptr) != SQLITE_OK)
    {
        qWarning() << "Failed to prepare vacuum statement: " << sqlite3_errmsg(db);
        return false;
    }

    QByteArray databaseName = newDatabase.toUtf8();
    if(sqlite3_bind_text(stmt, 1, databaseName, databaseName.length(), SQLITE_STATIC) != SQLITE_OK)
    {
        qWarning() << "Failed to bind url text: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        qWarning() << "Vacuum Execution failed: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);

    if(QFileInfo::exists(newDatabase))
    {
        QFile file(newDatabase);
        file.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner |
                            QFileDevice::ReadUser | QFileDevice::WriteUser |
                            QFileDevice::ReadGroup | QFileDevice::WriteGroup |
                            QFileDevice::ReadOther | QFileDevice::WriteOther);
    }
    else
        return false;

    clear();
    createExplorerTables();
    createExtractorTables();
    openDatabase(newDatabase);

    return true;
}

void Database::clear()
{
    char *err_msg = nullptr;

    sqlite3_db_config(db, SQLITE_DBCONFIG_RESET_DATABASE, 1, 0);

    if(sqlite3_exec(db, "VACUUM", nullptr, nullptr, nullptr))
    {
        qWarning() << "SQL error: VACUUM: " << QString(err_msg);
        sqlite3_free(err_msg);
        return;
    }

    sqlite3_db_config(db, SQLITE_DBCONFIG_RESET_DATABASE, 0, 0);

    // performance tunning
    setFlags();

    // create tables
    createExplorerTables();
    createExtractorTables();
}

void Database::createExplorerTables()
{
    char *err_msg = nullptr;

    // create table
    if (sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Explorer(target TEXT NOT NULL UNIQUE, result BLOB);", nullptr, nullptr, &err_msg))
    {
        qWarning() << "Create Explorer Table SQL error: " << QString(err_msg);
        sqlite3_free(err_msg);
        return;
    }
}

void Database::createExtractorTables()
{
    char *err_msg = nullptr;

    foreach(const QString &table_name, OTE::gInputOutput.outputTypes)
    {
        // create table
        if (sqlite3_exec(db,
                         QString("CREATE TABLE IF NOT EXISTS %1(result TEXT NOT NULL UNIQUE);").arg(table_name).toUtf8(),
                         nullptr,
                         nullptr,
                         &err_msg))
        {
            qWarning() << "Create " << table_name << " Table SQL error: " << QString(err_msg);
            sqlite3_free(err_msg);
            continue;
        }
    }
}

void Database::vaccum()
{
    char *err_msg = nullptr;

    if (sqlite3_exec(db, "PRAGMA vacuum;", nullptr, nullptr, &err_msg))
    {
        qWarning() << "SQL error: Error pragma vacuum: " << QString(err_msg);
        sqlite3_free(err_msg);
    }
}

void Database::optimize()
{
    char *err_msg = nullptr;

    if (sqlite3_exec(db, "PRAGMA optimize;", nullptr, nullptr, &err_msg))
    {
        qWarning() << "SQL error: Error pragma optimize: " << QString(err_msg);
        sqlite3_free(err_msg);
    }
}

void Database::setFlags()
{
    char *err_msg = nullptr;

    // Journal Mode: enable write ahead logging
    if (sqlite3_exec(db, "PRAGMA journal_mode = WAL;", nullptr, nullptr, &err_msg))
    {
        qWarning() << "SQL Error: Error enabling WAL: " << QString::fromUtf8(err_msg);
        sqlite3_free(err_msg);
    }

    // Synchronous Commit
    if (sqlite3_exec(db, "PRAGMA synchronous = normal;", nullptr, nullptr, &err_msg))
    {
        qWarning() << "SQL Error: Error enabling Synchronous Commit: " << QString::fromUtf8(err_msg);
        sqlite3_free(err_msg);
    }
}

void Database::checkpoint()
{
    char *err_msg = nullptr;
    if (sqlite3_wal_checkpoint(db, nullptr))
    {
        qWarning() << "SQL Error: Error checkpointing WAL: " << QString(err_msg);
        sqlite3_free(err_msg);
        return;
    }
}

void Database::beginTransaction()
{
    char *err_msg = nullptr;
    if(sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &err_msg))
    {
        qWarning() << "BEGIN TRANSACTION SQL error: " << QString(err_msg);
        sqlite3_free(err_msg);
        return;
    }
}

void Database::endTransaction()
{
    char *err_msg = nullptr;
    if(sqlite3_exec(db, "END TRANSACTION", nullptr, nullptr, &err_msg))
    {
        qWarning() << "END TRANSACTION SQL error: " << QString(err_msg);
        sqlite3_free(err_msg);
        return;
    }
}

bool Database::insertExplorerResult(const QString &tmplt, const QString &endpoint, const QString &target, const QByteArray &result)
{
    sqlite3_stmt *stmt = nullptr;

    QByteArray name = QString("%1__%2__%3").arg(tmplt, endpoint, target).toUtf8();
    QByteArray compressedResult = qCompress(result);

    if(sqlite3_prepare_v2(db, "INSERT OR REPLACE INTO Explorer(target,result) VALUES(?,?)", -1, &stmt, nullptr) != SQLITE_OK)
    {
        qWarning() << "Failed to prepare insert statement: " << sqlite3_errmsg(db);
        return false;
    }

    if(sqlite3_bind_text(stmt, 1, name, name.length(), SQLITE_STATIC) != SQLITE_OK)
    {
        qWarning() << "Failed to bind target text: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    if(sqlite3_bind_blob(stmt, 2, compressedResult, compressedResult.length(), SQLITE_STATIC) != SQLITE_OK)
    {
        qWarning() << "Failed to bind result blob: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);

    return true;
}

QStringList Database::insertExtractorResult(const QString &type, const QStringList &results)
{
    QStringList insertedResults;
    sqlite3_stmt *stmt = nullptr;

    if(sqlite3_prepare_v2(db,
                          QString("INSERT INTO %1(result) VALUES(?)").arg(type).toUtf8(),
                          -1,
                          &stmt,
                          nullptr) != SQLITE_OK)
    {
        qWarning() << "Failed to prepare insert statement: " << sqlite3_errmsg(db);
        return insertedResults;
    }

    foreach(const QString &result, results)
    {
        QByteArray baResult = result.toUtf8();
        if(sqlite3_bind_text(stmt, 1, baResult, baResult.length(), SQLITE_STATIC) != SQLITE_OK)
        {
            qWarning() << "Failed to bind target text: " << sqlite3_errmsg(db);
            sqlite3_reset(stmt);
            continue;
        }

        if(sqlite3_step(stmt) == SQLITE_DONE)
            insertedResults << result;

        sqlite3_reset(stmt);
    }

    return insertedResults;
}

QByteArray Database::getExplorerResult(const QString &tmplt, const QString &endpoint, const QString &target)
{
    QByteArray result;
    QByteArray name =  QString("%1__%2__%3").arg(tmplt, endpoint, target).toUtf8();

    sqlite3_stmt *stmt = nullptr;
    if(sqlite3_prepare_v2(db, "SELECT result FROM Explorer WHERE target = ?", -1, &stmt, nullptr) != SQLITE_OK)
    {
        qWarning() << "Failed to prepare Select statement: " << sqlite3_errmsg(db);
        return result;
    }

    if(sqlite3_bind_text(stmt, 1, name, name.length(), SQLITE_STATIC) != SQLITE_OK)
    {
        qWarning() << "Failed to bind url text: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return result;
    }

    if(sqlite3_step(stmt) == SQLITE_ROW)
    {
        result = qUncompress(QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, 0)),
                                                                 sqlite3_column_bytes(stmt, 0)));
    }

    if(sqlite3_finalize(stmt))
        qWarning() << "SQL Error: failed to finalize statement";

    return result;
}

bool Database::removeExplorerResult(const QString &tmplt, const QString &endpoint, const QString &target)
{
    QByteArray name = QString("%1__%2__%3").arg(tmplt, endpoint, target).toUtf8();

    sqlite3_stmt *stmt = nullptr;
    if(sqlite3_prepare_v2(db, "DELETE FROM Explorer WHERE target = ?", -1, &stmt, nullptr) != SQLITE_OK)
    {
        qWarning() << "Failed to prepare Delete statement: " << sqlite3_errmsg(db);
        return false;
    }

    if(sqlite3_bind_text(stmt, 1, name, name.length(), SQLITE_STATIC) != SQLITE_OK)
    {
        qWarning() << "Failed to bind text: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        qWarning() << "Delete Execution failed: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);

    return true;
}

bool Database::removeExtractorResult(const QString &type, const QString &result)
{
    sqlite3_stmt *stmt = nullptr;
    if(sqlite3_prepare_v2(db,
                          QString("DELETE FROM %1 WHERE result = ?").arg(type).toUtf8(),
                          -1,
                          &stmt,
                          nullptr) != SQLITE_OK)
    {
        qWarning() << "Failed to prepare Delete statement: " << sqlite3_errmsg(db);
        return false;
    }

    QByteArray baResult = result.toUtf8();
    if(sqlite3_bind_text(stmt, 1, baResult, baResult.length(), SQLITE_STATIC) != SQLITE_OK)
    {
        qWarning() << "Failed to bind text: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    if(sqlite3_step(stmt) != SQLITE_DONE)
    {
        qWarning() << "Delete Execution failed: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);

    return true;
}

QList<ExplorerResult> Database::loadExplorerResults()
{
    QList<ExplorerResult> results;
    sqlite3_stmt *stmt;

    if(sqlite3_prepare_v2(Database::db, "SELECT target FROM Explorer", -1, &stmt, nullptr))
    {
        qWarning() << "SQL Error: Failed to prepare statement: " << sqlite3_errmsg(Database::db);
        return results;
    }

    for (;;)
    {
        int rc = sqlite3_step(stmt);

        if(rc == SQLITE_DONE)
            break;
        else if(rc != SQLITE_ROW)
            continue;

        results << targetToExplorer(QByteArray(static_cast<const char*>(sqlite3_column_blob(stmt, 0)),
                                                                        sqlite3_column_bytes(stmt, 0)));
    }

    sqlite3_finalize(stmt);

    return results;
}

QMap<QString, QStringList> Database::loadExtractorResults()
{
    QMap<QString, QStringList> results;

    foreach(const QString &type, OTE::gInputOutput.outputTypes)
    {
        sqlite3_stmt *stmt;

        if(sqlite3_prepare_v2(Database::db,
                              QString("SELECT result FROM %1").arg(type).toUtf8(),
                              -1, &stmt,
                              nullptr))
        {
            qWarning() << "SQL Error: Failed to prepare statement: " << sqlite3_errmsg(Database::db);
            return results;
        }

        QStringList resultList;

        for (;;)
        {
            int rc = sqlite3_step(stmt);

            if(rc == SQLITE_DONE)
                break;
            else if(rc != SQLITE_ROW)
                continue;

            resultList << QString::fromUtf8(static_cast<const char*>(sqlite3_column_blob(stmt, 0)),
                                                                     sqlite3_column_bytes(stmt, 0));
        }

        results.insert(type, resultList);

        sqlite3_finalize(stmt);
    }

    return results;
}

QByteArray Database::targetFromExplorer(const ExplorerResult &result)
{
    return QString("%1__%2__%3").arg(result.tmplt, result.endpoint, result.target).toUtf8();
}

ExplorerResult Database::targetToExplorer(const QByteArray &target)
{
    ExplorerResult result;

    int tmplt_pos = target.indexOf("__", 0);
    result.tmplt = target.mid(0, tmplt_pos);

    int endpoint_pos = target.indexOf("__", tmplt_pos+2);
    result.endpoint = target.mid(tmplt_pos+2, endpoint_pos-(tmplt_pos+2));

    result.target = target.mid(endpoint_pos+2);

    return result;
}

} // namespace OTE

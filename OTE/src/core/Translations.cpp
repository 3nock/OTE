//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "Translations.h"

#include <QCoreApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QString>

#if !defined(Q_OS_WIN32) && !defined(Q_OS_MAC)
#include <stdlib.h>
#endif

namespace OTE {

static QString TRANSLATION_FILE_PREFIX = "ote_";
static QString TRANSLATION_FILE_SUFFIX = ".qm";

QStringList Translations::GetPossibleTranslationPaths()
{
    // There are a few different places translations can be stored depending
    // on the platform and where they were installed.
    QStringList possible_qm_locations;

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    possible_qm_locations.append(QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#else
    possible_qm_locations.append(QLibraryInfo::path(QLibraryInfo::TranslationsPath));
#endif

#ifdef Q_OS_MAC
    possible_qm_locations.append(QCoreApplication::applicationDirPath() + "/../translations");
#else
    possible_qm_locations.append(QCoreApplication::applicationDirPath() + "/translations");
#endif

    return possible_qm_locations;
}

QStringList Translations::GetLanguages()
{
    QStringList ui_languages;
    QStringList checked_dirs;
    foreach(QString path, GetPossibleTranslationPaths())
    {
        // Find all translation files and add them to the avaliable list.
        QDir translationDir(path);

        if (translationDir.exists() && !checked_dirs.contains(translationDir.absolutePath()))
        {
            QStringList filters;
            // Look for all ssuite_*.qm files.
            filters << QString(TRANSLATION_FILE_PREFIX+"*"+TRANSLATION_FILE_SUFFIX);
            translationDir.setNameFilters(filters);
            QStringList translation_files = translationDir.entryList();
            foreach(QString file, translation_files)
            {
                QFileInfo fileInfo(file);
                QString basename = fileInfo.baseName();
                QString language = basename.right(basename.length() - TRANSLATION_FILE_PREFIX.length());
                ui_languages.append(language);
            }
            checked_dirs.append(translationDir.absolutePath());
        }
    }
    return ui_languages;
}

} // namespace OTE

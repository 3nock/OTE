//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef CORE_TRANSLATIONS_H
#define CORE_TRANSLATIONS_H

#include <QStringList>

namespace OTE {

class Translations
{
public:
    static QStringList GetPossibleTranslationPaths();
    static QStringList GetLanguages();
};

} // namespace OTE

#endif // CORE_TRANSLATIONS_H

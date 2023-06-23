//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "Utility.h"

namespace OTE {

QString Utility::getEnvironmentVar(const QString &variable_name)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    // The only time this might fall down is on Linux when an
    // environment variable holds bytedata. Don't use this
    // utility function for retrieval if that's the case.
    return qEnvironmentVariable(variable_name.toUtf8().constData(), "").trimmed();
#else
    // This will typically only be used on older Qts on Linux
    return QProcessEnvironment::systemEnvironment().value(variable_name, "").trimmed();
#endif
}

QIcon Utility::getApplicationIcon()
{
    QIcon icon;
    icon.addFile(":/icons/res/icons/logo24.png",  QSize(24, 24));
    icon.addFile(":/icons/res/icons/logo32.png",  QSize(32, 32));
    icon.addFile(":/icons/res/icons/logo48.png",  QSize(48, 48));
    icon.addFile(":/icons/res/icons/logo64.png",  QSize(64, 64));
    icon.addFile(":/icons/res/icons/logo128.png", QSize(128, 128));
    icon.addFile(":/icons/res/icons/logo256.png", QSize(256, 256));
    icon.addFile(":/icons/res/icons/logo512.png", QSize(512, 512));
    return icon;
}

} // namespace OTE

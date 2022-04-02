#ifndef CRASHHANDLER_H
#define CRASHHANDLER_H

#pragma once
#include <QString>

#if defined(Q_OS_LINUX) || defined(Q_OS_WIN32)


namespace Breakpad {
    class CrashHandlerPrivate;
    class CrashHandler
    {
    public:
        static CrashHandler* instance();
    void Init(const QString&  reportPath);

        void setReportCrashesToSystem(bool report);
        bool writeMinidump();

    private:
        CrashHandler();
        ~CrashHandler();
        Q_DISABLE_COPY(CrashHandler)
        CrashHandlerPrivate* d;
    };
}

#endif

#endif // CRASHHANDLER_H

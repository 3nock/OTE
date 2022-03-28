#include "CrashHandler.h"

#include <QDir>
#include <QProcess>
#include <QCoreApplication>

#if defined(Q_OS_LINUX)
#include "client/linux/handler/exception_handler.h"
#elif defined(Q_OS_WIN32)
#include "client/windows/handler/exception_handler.h"
#endif

#if defined(Q_OS_LINUX) || defined(Q_OS_WIN32)
namespace Breakpad {

///
/// \brief The CrashHandlerPrivate class...
///

class CrashHandlerPrivate {
public:
    CrashHandlerPrivate(){
        pHandler = NULL;
    }
    ~CrashHandlerPrivate(){
        delete pHandler;
    }

    void InitCrashHandler(const QString& dumpPath);
    static google_breakpad::ExceptionHandler* pHandler;
    static bool bReportCrashesToSystem;
};

google_breakpad::ExceptionHandler* CrashHandlerPrivate::pHandler = NULL;
bool CrashHandlerPrivate::bReportCrashesToSystem = false;

///
/// DumpCallback...
///

#if defined(Q_OS_WIN32)
bool DumpCallback(const wchar_t* _dump_dir,const wchar_t* _minidump_id,void* context,EXCEPTION_POINTERS* exinfo,MDRawAssertionInfo* assertion,bool success)
#elif defined(Q_OS_LINUX)
bool DumpCallback(const google_breakpad::MinidumpDescriptor &md,void *context, bool success)
#endif
{
    Q_UNUSED(context);
#if defined(Q_OS_WIN32)
    Q_UNUSED(_dump_dir);
    Q_UNUSED(_minidump_id);
    Q_UNUSED(assertion);
    Q_UNUSED(exinfo);
#endif
    qDebug("BreakpadQt crash");

    /*
    NO STACK USE, NO HEAP USE THERE !!!
    Creating QString's, using qDebug, etc. - everything is crash-unfriendly.
    */
    return CrashHandlerPrivate::bReportCrashesToSystem ? success : true;
}

void CrashHandlerPrivate::InitCrashHandler(const QString& dumpPath) {
    if ( pHandler != NULL )
        return;

#if defined(Q_OS_WIN32)
    std::wstring pathAsStr = (const wchar_t*)dumpPath.utf16();
    pHandler = new google_breakpad::ExceptionHandler(
        pathAsStr,
        /*FilterCallback*/ 0,
        DumpCallback,
        /*context*/
        0,
        true
        );
#elif defined(Q_OS_LINUX)
    std::string pathAsStr = dumpPath.toStdString();
    google_breakpad::MinidumpDescriptor md(pathAsStr);
    pHandler = new google_breakpad::ExceptionHandler(
        md,
        /*FilterCallback*/ 0,
        DumpCallback,
        /*context*/ 0,
        true,
        -1
        );
#endif
}

///
/// CrashHandler...
///

CrashHandler* CrashHandler::instance(){
    static CrashHandler globalHandler;
    return &globalHandler;
}

CrashHandler::CrashHandler(){
    d = new CrashHandlerPrivate();
}

CrashHandler::~CrashHandler(){
    delete d;
}

void CrashHandler::setReportCrashesToSystem(bool report){
    d->bReportCrashesToSystem = report;
}

bool CrashHandler::writeMinidump(){
    bool res = d->pHandler->WriteMinidump();
    if (res) {
        qDebug("BreakpadQt: writeMinidump() success.");
    } else {
        qWarning("BreakpadQt: writeMinidump() failed.");
    }
    return res;
}

void CrashHandler::Init( const QString& reportPath ){
    d->InitCrashHandler(reportPath);
}

}

#endif

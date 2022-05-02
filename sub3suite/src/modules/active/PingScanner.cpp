#include "PingScanner.h"


ping::Scanner::Scanner(ping::ScanArgs *args): AbstractScanner (),
    m_args(args)
{
}
ping::Scanner::~Scanner(){
}

void ping::Scanner::lookup(){
    m_target = ping::getTarget(m_args);

#if defined(Q_OS_WIN)
    // Load Winsock
    WSADATA wsd;
    int rc;
    if ((rc = WSAStartup(MAKEWORD(2,2), &wsd)) != 0){
        qWarning() << "WSAStartup() failed: " << rc;
        emit quitThread();
        return;
    }
#endif // WINDOWS

    while(m_target != nullptr) {
        m_mutex.lock();

        /* if received pause signal lock the thread, dont unlock until resume signal*/
        if(pause)
            m_wait.wait(&m_mutex);

        /* check if received stop signal */
        if(stop)
            break;

        m_mutex.unlock();

        log.target = m_target;
        QByteArray target = m_target.toLocal8Bit();

        /* start scan */
#if defined(Q_OS_UNIX)
        if(ping_unix() == -1)
            emit scanLog(log);
#endif
#if defined(Q_OS_WIN)
        gDestination = target.data();
        if(ping_win() == -1)
            emit scanLog(log);
#endif
        /* scan progress */
        m_args->progress++;
        emit scanProgress(m_args->progress);

        /* get next target */
        m_target = getTarget(m_args);
    }

    emit quitThread();
}

QString ping::getTarget(ping::ScanArgs *args){
    QMutexLocker(&args->mutex);

    if(!args->targets.isEmpty())
        return args->targets.dequeue();
    else
        return nullptr;
}

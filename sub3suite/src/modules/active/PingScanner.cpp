#include "PingScanner.h"


ping::Scanner::Scanner(ping::ScanArgs *args): AbstractScanner (),
    m_args(args)
{
}
ping::Scanner::~Scanner(){
}

void ping::Scanner::lookup(){
    m_target = ping::getTarget(m_args);

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

        /* start scan */
#if defined(Q_OS_UNIX)
        if(ping() == -1)
            emit scanLog(log);
#endif
#if defined(Q_OS_WIN)
        QByteArray target = m_target.toLocal8Bit();
        gDestination = target.data();
        if(ping() == -1)
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

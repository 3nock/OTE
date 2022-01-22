/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : the Scanner establishes secure TLS connetion with particular
          target & a specified port that uses SSL, then obtains SSL Certificate
          & extracts valuable info from the SSL-Certificate.
*/

#include "SSLScanner.h"


ssl::Scanner::Scanner(ssl::ScanArgs *args): AbstractScanner (nullptr),
    m_args(args)
{
}
ssl::Scanner::~Scanner(){
}

void ssl::Scanner::lookup(){
    /* get target */
    QSslSocket socket;
    QString target = ssl::getTarget(m_args);

    /* a blocking connection to the target to obtain ssl certificate */
    while(!target.isNull())
    {
        m_mutex.lock();

        /* if received pause signal lock the thread, dont unlock until resume signal*/
        if(m_pause)
            m_wait.wait(&m_mutex);

        /* check if received stop signal */
        if(m_stop)
            break;

        m_mutex.unlock();

        switch(m_args->port){
        case HTTPS:
            socket.connectToHostEncrypted(target, 443);
            break;
        case FTP:
            socket.connectToHostEncrypted(target, 21);
            break;
        case FTPs:
            socket.connectToHostEncrypted(target, 990);
            break;
        case SSH:
            socket.connectToHostEncrypted(target, 22);
            break;
        }

        if(!socket.waitForEncrypted(m_args->config->timeout*1000))
        {
            switch (socket.error())
            {
            case QAbstractSocket::ConnectionRefusedError:
            case QAbstractSocket::HostNotFoundError:
                break;
            default:
                log.target = target;
                log.message = socket.errorString();
                emit scanLog(log);
            }
        }
        else
        {
            /* obtaining the certificate */
            QSslCertificate certificate = socket.peerCertificate();

            /* close connetion after obtaining the certificate */
            socket.close();

            if(!certificate.isNull())
            {
                /* emiting the obtained results */
                switch (m_args->output) {
                case ssl::OUTPUT::RAW:
                    emit resultRaw(target, certificate);
                    break;
                case ssl::OUTPUT::SHA1:
                    emit resultSHA1(certificate.digest(QCryptographicHash::Sha1).toHex());
                    break;
                case ssl::OUTPUT::SHA256:
                    emit resultSHA256(certificate.digest(QCryptographicHash::Sha256).toHex());
                    break;
                case ssl::OUTPUT::SUBDOMAIN:
                    QStringList subdomains;
                    foreach(const QString &domain, certificate.subjectAlternativeNames())
                        subdomains.append(domain);
                    emit resultSubdomain(target, subdomains);
                    break;
                }
            }
        }

        /* scan progress */
        m_args->progress++;
        emit scanProgress(m_args->progress);

        /* next target */
        target = getTarget(m_args);
    }

    /* end of targets */
    emit quitThread();
}

QString ssl::getTarget(ssl::ScanArgs *args){
    /* lock */
    QMutexLocker(&args->mutex);

    if(!args->targets.isEmpty())
        return args->targets.dequeue();
    else
        return nullptr;
}

#include "CertScanner.h"
#include <QSslSocket>

/*
 * implement use of different protocals that use TLS...
 */
certificate::Scanner::Scanner(certificate::ScanArguments args): AbstractScanner (nullptr),
    m_args(args)
{
}
certificate::Scanner::~Scanner(){
}

void certificate::Scanner::lookup(){
    QSslSocket socket;

    ///
    /// for single target...
    ///
    if(m_args.singleTarget)
    {
        /* making a blocking connection to domain/ip target provided */
        socket.connectToHostEncrypted(m_args.target, 443);

        /* waiting SSL-handshake to finish, if done emit the acquired target's ssl-cert */
        if(!socket.waitForEncrypted(m_args.timeout*1000))
            emit errorLog(socket.errorString());
        else
        {
            /* emiting the obtained results */
            if(m_args.raw){
                emit resultRaw(socket.peerCertificate().toPem());
            }
            if(m_args.sha1){
                emit resultSHA1(socket.peerCertificate().digest(QCryptographicHash::Sha1).toHex());
            }
            if(m_args.sha256){
                emit resultSHA256(socket.peerCertificate().digest(QCryptographicHash::Sha256).toHex());
            }
            if(m_args.subdomain){
                foreach(const QString &domain, socket.peerCertificate().subjectAlternativeNames())
                    emit resultSubdomain(domain);
            }
            socket.close();
        }
        emit quitThread();
        return;
    }

    ///
    /// for multiple targets...
    ///
    while(!m_args.targetList.isEmpty())
    {
        /* scan progress */
        m_args.progress++;
        emit scanProgress(m_args.progress);

        /* making a blocking connection to domain/ip target provided */
        socket.connectToHostEncrypted(m_args.targetList.pop(), 443);

        /* waiting SSL-handshake to finish, if done emit the acquired target's ssl-cert */
        if(!socket.waitForEncrypted(m_args.timeout*1000))
            emit errorLog(socket.errorString());

        else
        {
            /* emiting the obtained results */
            if(m_args.raw){
                emit resultRaw(socket.peerCertificate().toPem());
            }
            if(m_args.sha1){
                emit resultSHA1(socket.peerCertificate().digest(QCryptographicHash::Sha1).toHex());
            }
            if(m_args.sha256){
                emit resultSHA256(socket.peerCertificate().digest(QCryptographicHash::Sha256).toHex());
            }
            if(m_args.subdomain){
                foreach(const QString &domain, socket.peerCertificate().subjectAlternativeNames())
                    emit resultSubdomain(domain);
            }
            socket.close();
        }
    }
    emit quitThread();
}

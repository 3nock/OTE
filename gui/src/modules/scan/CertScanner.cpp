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

    /* making a blocking connection to domain/ip target provided */
    socket.connectToHostEncrypted(m_args.target, 443);

    /* waiting SSL-handshake to finish, if done emit the acquired target's ssl-cert */
    if(!socket.waitForEncrypted(m_args.timeout*1000))
        emit errorLog(socket.errorString());
    else{
        emit rawCert(socket.peerCertificate().toPem());
        socket.close();
    }

    emit quitThread();
}

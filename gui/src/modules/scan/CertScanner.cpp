#include "CertScanner.h"
#include <QSslSocket>

/*
 * implement use of different protocals that use TLS...
 */
cert::Scanner::Scanner(cert::ScanArguments args): AbstractScanner (nullptr),
    m_args(args)
{
}
cert::Scanner::~Scanner(){
}

void cert::Scanner::lookup(){
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

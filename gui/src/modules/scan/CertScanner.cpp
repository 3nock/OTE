#include "CertScanner.h"
#include <QSslSocket>


cert::Scanner::Scanner(QString target): AbstractScanner (nullptr),
    m_target(target)
{
}
cert::Scanner::~Scanner(){
}

void cert::Scanner::lookup(){
    QSslSocket socket;

    socket.connectToHostEncrypted(m_target, 443);

    if(!socket.waitForEncrypted())
        emit errorLog(socket.errorString());
    else{
        emit rawCert(socket.peerCertificate().toPem());
        socket.close();
    }

    emit quitThread();
}

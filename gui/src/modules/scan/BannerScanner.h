#ifndef BANNERSCANNER_H
#define BANNERSCANNER_H

#include <QTcpSocket>
#include <QStack>
#include "AbstractScanner.h"
#include "src/dialogs/ConfigDialog.h"


/*
Web
80	HTTP
443	SSL
21	FTP
990	FTPs
22	SFTP / SSH
3306	MySQL

Email
110	POP – Incoming
995	POP SSL – Incoming
143	IMAP – Incoming
993	IMAP SSL – Incoming
25, 80, 3535	SMTP – Outgoing
465	SMTP SSL – Outgoing

Cpanel
2082	cPanel TCP inbound
2083	cPanel SSL TCP inbound
2086	WHM TCP inbound
2087	WHM SSL TCP inbound
2089	WHM SSL TCP inbound
2095	Webmail TCP inbound
2096	Webmail SSL TCP inbound

b->payloads.tcp[80] = &banner_http;
b->payloads.tcp[8080] = &banner_http;
b->payloads.tcp[139] = (void*)&banner_smb0;
b->payloads.tcp[445] = (void*)&banner_smb1;
b->payloads.tcp[443] = (void*)&banner_ssl;   // HTTP/s
b->payloads.tcp[465] = (void*)&banner_ssl;   // SMTP/s
b->payloads.tcp[990] = (void*)&banner_ssl;   // FTP/s
b->payloads.tcp[991] = (void*)&banner_ssl;
b->payloads.tcp[992] = (void*)&banner_ssl;   // Telnet/s
b->payloads.tcp[993] = (void*)&banner_ssl;   // IMAP4/s
b->payloads.tcp[994] = (void*)&banner_ssl;
b->payloads.tcp[995] = (void*)&banner_ssl;   // POP3/s
b->payloads.tcp[2083] = (void*)&banner_ssl;  // cPanel - SSL
b->payloads.tcp[2087] = (void*)&banner_ssl;  // WHM - SSL
b->payloads.tcp[2096] = (void*)&banner_ssl;  // cPanel webmail - SSL
b->payloads.tcp[8443] = (void*)&banner_ssl;  // Plesk Control Panel - SSL
b->payloads.tcp[9050] = (void*)&banner_ssl;  // Tor
b->payloads.tcp[8140] = (void*)&banner_ssl;  // puppet
b->payloads.tcp[11211] = (void*)&banner_memcached;
b->payloads.tcp[23] = (void*)&banner_telnet;
b->payloads.tcp[3389] = (void*)&banner_rdp;
*/

namespace banner {

struct ScanArguments{
    QString target;
    QStack<QString> targetList;
    int progress;

    /* target type */
    bool hostname = false;
    bool ipaddress = false;

    /* service to test */
    bool http = false;
    bool https = false;
    bool ftp = false;
    bool ssh = false;
    bool smtp = false;
};

class Scanner : public AbstractScanner{
    public:
        Scanner(ScanConfig *scanConfig, banner::ScanArguments *scanArguments);
        ~Scanner() override;

    private slots:
        void lookup() override;

    private:
        ScanConfig *m_config;
        banner::ScanArguments *m_args;
        //...
        QTcpSocket *m_socket;
};
}
#endif // BANNERSCANNER_H

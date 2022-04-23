// From Network Programming for Microsoft Windows, Second Edition by 
// Anthony Jones and James Ohlund.  
// Copyright 2002.   Reproduced by permission of Microsoft Press.  
// All rights reserved.
//
//
// Common routines for resolving addresses and hostnames
//
// Files:
//      resolve.cpp     - Common routines
//      resolve.h       - Header file for common routines
//
// Description:
//      This file contains common name resolution and name printing
//      routines and is used by many of the samples on this CD.
//

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <strsafe.h>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>

#include "resolve.h"


///
/// Description:
///    This routine takes a SOCKADDR structure and its length and prints
///    converts it to a string representation. This string is printed
///    to the console via stdout.
///
QString PrintAddress(SOCKADDR *sa, int salen) {
    char    host[NI_MAXHOST], serv[NI_MAXSERV];
    int     hostlen = NI_MAXHOST, servlen = NI_MAXSERV, rc;

    rc = getnameinfo(sa,salen,host,static_cast<DWORD>(hostlen),serv,static_cast<DWORD>(servlen),NI_NUMERICHOST | NI_NUMERICSERV);
    if (rc != 0){
        qWarning() << __FILE__ << ": getnameinfo failed: " << rc;
        return nullptr;
    }

    // If the port is zero then don't print it
    if (strcmp(serv, "0") != 0){
        if (sa->sa_family == AF_INET)
            return QString("[%1]:%2").arg(host).arg(serv);
        else
            return QString("%1:%2").arg(host).arg(serv);
    }
    else
        return QString(host);
}

///
/// Description:
///    This is similar to the PrintAddress function except that instead of
///    printing the string address to the console, it is formatted into
///    the supplied string buffer.
///
int FormatAddress(SOCKADDR *sa, int salen, char *addrbuf, int addrbuflen)
{
    char    host[NI_MAXHOST],serv[NI_MAXSERV];
    int     hostlen = NI_MAXHOST,servlen = NI_MAXSERV,rc;
    HRESULT hRet;

    rc = getnameinfo(sa,salen,host,static_cast<DWORD>(hostlen),serv,static_cast<DWORD>(servlen),NI_NUMERICHOST | NI_NUMERICSERV);
    if (rc != 0){
        qWarning() << __FILE__ << ": getnameinfo failed: " << rc;
        return rc;
    }

    if ( (strlen(host) + strlen(serv) + 1) > static_cast<unsigned>(addrbuflen))
        return WSAEFAULT;

    addrbuf[0] = '\0';

    if (sa->sa_family == AF_INET)
    {
        if(FAILED(hRet = StringCchPrintfA(addrbuf, static_cast<size_t>(addrbuflen), "%s:%s", host, serv))){
            qWarning() << __FILE__ << " StringCchPrintf failed: " << hRet;
            return static_cast<int>(hRet);
        }
    }
    else if (sa->sa_family == AF_INET6)
    {
        if(FAILED(hRet = StringCchPrintfA(addrbuf, static_cast<size_t>(addrbuflen), "[%s]:%s", host, serv))){
            qWarning() << __FILE__ << " StringCchPrintf failed: " << hRet;
            return static_cast<int>(hRet);
        }
    }

    return NO_ERROR;
}

///
/// Description:
///    This routine resolves the specified address and returns a list of addrinfo
///    structure containing SOCKADDR structures representing the resolved addresses.
///    Note that if 'addr' is non-NULL, then getaddrinfo will resolve it whether
///    it is a string listeral address or a hostname.
///
struct addrinfo *ResolveAddress(char *addr, char *port, int af, int type, int proto) {
    struct addrinfo hints, *res = nullptr;
    int             rc;

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags  = ((addr) ? 0 : AI_PASSIVE);
    hints.ai_family = af;
    hints.ai_socktype = type;
    hints.ai_protocol = proto;

    rc = getaddrinfo(addr, port, &hints, &res);
    if (rc != 0) {
        qWarning() << "Invalid address: " << addr << "getaddrinfo failed: " << rc;
        return nullptr;
    }
    return res;
}

///
/// Description:
///    This routine takes a SOCKADDR and does a reverse lookup for the name
///    corresponding to that address.
///
int ReverseLookup(SOCKADDR *sa, int salen, char *buf, int buflen) {
    char    host[NI_MAXHOST];
    int     hostlen=NI_MAXHOST, rc;
    HRESULT hRet;
    
    rc = getnameinfo(sa,salen,host,static_cast<DWORD>(hostlen),nullptr,0,0);
    if (rc != 0){
        qWarning() << "getnameinfo failed: " << rc;
        return rc;
    }

    buf[0] = '\0';
    if(FAILED(hRet = StringCchCopyA(buf, static_cast<size_t>(buflen), host))){
        qWarning() << "StringCchCopy failed: " << hRet;
        return static_cast<int>(hRet);
    }

    return NO_ERROR;
}

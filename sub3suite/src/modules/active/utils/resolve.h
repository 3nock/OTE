/*
   From Network Programming for Microsoft Windows, Second Edition by
   Anthony Jones and James Ohlund.
   Copyright 2002.   Reproduced by permission of Microsoft Press.
   All rights reserved.

   Common routines for resolving addresses and hostnames

   Description:
        This file contains common name resolution and name printing
        routines and is used by many of the samples on this CD.
*/

#ifndef RESOLVE_H_
#define RESOLVE_H_

#include <ws2def.h>
#include <QString>

#ifdef _cplusplus
extern "C" {
#endif

QString PrintAddress(SOCKADDR *sa, int salen);
int FormatAddress(SOCKADDR *sa, int salen, char *addrbuf, int addrbuflen);
int ReverseLookup(SOCKADDR *sa, int salen, char *namebuf, int namebuflen);
struct addrinfo *ResolveAddress(char *addr, char *port, int af, int type, int proto);

#ifdef _cplusplus
}
#endif

#endif

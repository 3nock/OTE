#include "lib-brute.h"

QString nameProcessor_subBrute(QString subdomainName, QString domainName){
    return subdomainName+"."+domainName;
}

QString nameProcessor_tldBrute(QString tldName, QString domainName){
    return domainName+"."+tldName;
}

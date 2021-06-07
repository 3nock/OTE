#include "core.h"

// getting the enumerator name...
QString EnumName(int enumName){
    if(enumName == ENUMNAME_OSINT){
        return "osint";
    }
    if(enumName == ENUMNAME_SUBBRUTE){
        return "subBrute";
    }
    if(enumName == ENUMNAME_TLDBRUTE){
        return "tldBrute";
    }
    if(enumName == ENUMNAME_ACTIVESUBDOMAINS){
        return "activeSubdomains";
    }
}

// filter for the domain target name...
QString TargetNameFilter(QString domainName, int enumName){
    /******************* General Modification *******************/
    domainName = domainName.simplified();
    domainName = domainName.remove(" ");
    domainName = domainName.replace(',', '.');
    // regex takes too much time...
    //domainName = domainName.remove(QRegExp("[^A-Za-z0-9_.\\-~]"));

    /************ modifying according to enumName **************/
    if(enumName == ENUMNAME_TLDBRUTE){
        domainName = domainName.split(".",QString::SkipEmptyParts)[0];
    }
    if(enumName == ENUMNAME_SUBBRUTE || enumName == ENUMNAME_OSINT){
        // if top level domain name wasn't specified, ".com" will be appended on the domainName...
        if(domainName.split(".").count() == 1){
            domainName += ".com";
        }
    }
    return domainName;
}

QString wordlistFilter(QString word){
    return word;
}

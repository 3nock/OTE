#include "utils.h"
#include <QFile>
#include <QDir>

QHostAddress RandomNameserver(bool useCustomNameservers){
    QStringList nameservers;
    QString filename;
    if(useCustomNameservers){
        filename = QDir::currentPath()+"/wordlists/custom_nameservers.txt";
    }else{
        filename = ":/files/res/files/nameservers.txt";
    }
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QString line;
        QTextStream in(&file);
        while (!in.atEnd()){
            nameservers.append(in.readLine());
        }
        file.close();
    }
    return QHostAddress(nameservers[rand()%nameservers.length()]);
}

///
/// filter for the domain target name...
///
QString TargetNameFilter(QString domainName, ENGINE engine){
    /******************* General Modification *******************/
    domainName = domainName.simplified();
    domainName = domainName.remove(" ");
    domainName = domainName.replace(',', '.');
    // regex takes too much time...
    //domainName = domainName.remove(QRegExp("[^A-Za-z0-9_.\\-~]"));

    /************ modifying according to enumName **************/
    if(engine == ENGINE::TLDBRUTE){
        domainName = domainName.split(".",QString::SkipEmptyParts)[0];
    }
    if(engine == ENGINE::SUBBRUTE || engine == ENGINE::OSINT){
        ///
        /// if top level domain name wasn't specified, ".com" will be appended on the domainName...
        ///
        if(domainName.split(".").count() == 1){
            domainName += ".com";
        }
    }
    return domainName;
}

QString wordlistFilter(QString word){
    return word;
}

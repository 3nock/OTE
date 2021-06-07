#include "lib-brute.h"

QString nameProcessor_subBrute(QString subdomainName, QString domainName){
    return subdomainName+"."+domainName;
}

QString nameProcessor_tldBrute(QString tldName, QString domainName){
    return domainName+"."+tldName;
}

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

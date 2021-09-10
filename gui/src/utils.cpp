#include "utils.h"
#include <QFile>
#include <QDir>
//...
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

///
/// getting the enumerator name...
///
QString EnumName(ENGINE engine){
    if(engine == ENGINE::OSINT){
        return "osint";
    }
    if(engine == ENGINE::SUBBRUTE){
        return "subBrute";
    }
    if(engine == ENGINE::TLDBRUTE){
        return "tldBrute";
    }
    if(engine == ENGINE::ACTIVE){
        return "active";
    }
    return "";
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

///
/// getting configuration value...
///
QString GetConfig(QString configType, ENGINE engineName){
    QFile config_file(QDir::currentPath()+"/config/brute-config.json");
    config_file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(config_file.isOpen()){
        ///
        /// reading all data from the key file...
        ///
        QString keys = config_file.readAll();
        config_file.close();
        QJsonDocument apis = QJsonDocument::fromJson(keys.toUtf8());
        QJsonObject keys_object_ = apis.object();
        QJsonObject keys_object = keys_object_[configType].toObject();
        QJsonValue value = keys_object.value(EnumName(engineName));
        return value.toString();
    }
    return EMPTY;
}

///
/// setting configuration value...
///
void SetConfig(QString configType, ENGINE engineName, QString configValue){
    QFile config_file(QDir::currentPath()+"/config/brute-config.json");
    config_file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(config_file.isOpen()){
        ///
        /// reading all data from the key file...
        ///
        QJsonParseError JsonParseError;
        QJsonDocument JsonDocument = QJsonDocument::fromJson(config_file.readAll(), &JsonParseError);
        QJsonObject RootObject = JsonDocument.object();
        config_file.close();
        ///
        /// setting the value...
        ///
        QJsonValueRef ref = RootObject.find(configType).value();
        QJsonObject ref_addvalue = ref.toObject();
        ref_addvalue.insert(EnumName(engineName), configValue);
        ref = ref_addvalue;
        ///
        /// writing to the config.json file...
        ///
        JsonDocument.setObject(RootObject);
        config_file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
        if(config_file.isOpen()){
            config_file.write(JsonDocument.toJson());
            config_file.close();
        }
    }
}

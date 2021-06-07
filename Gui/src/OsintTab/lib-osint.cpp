#include "lib-osint.h"

// getting configuration value...
QString GetConfig(QString configType, int enumName){
    QFile config_file(QDir::currentPath()+"/config/brute-config.json");
    config_file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(config_file.isOpen()){
        // reading all data from the key file...
        QString keys = config_file.readAll();
        config_file.close();
        QJsonDocument apis = QJsonDocument::fromJson(keys.toUtf8());
        QJsonObject keys_object_ = apis.object();
        QJsonObject keys_object = keys_object_[configType].toObject();
        QJsonValue value = keys_object.value(EnumName(enumName));
        return value.toString();
    }
    return "";
}

// setting configuration value...
void SetConfig(QString configType, int enumName, QString configValue){
    QFile config_file(QDir::currentPath()+"/config/brute-config.json");
    config_file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(config_file.isOpen()){
        // reading all data from the key file...
        QJsonParseError JsonParseError;
        QJsonDocument JsonDocument = QJsonDocument::fromJson(config_file.readAll(), &JsonParseError);
        QJsonObject RootObject = JsonDocument.object();
        config_file.close();

        // setting the value...
        QJsonValueRef ref = RootObject.find(configType).value();
        QJsonObject ref_addvalue = ref.toObject();
        ref_addvalue.insert(EnumName(enumName), configValue);
        ref = ref_addvalue;

        // writing to the config.json file...
        JsonDocument.setObject(RootObject);
        config_file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
        if(config_file.isOpen()){
            config_file.write(JsonDocument.toJson());
            config_file.close();
        }
    }
}

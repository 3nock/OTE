#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>

class Config{
    public:
        QSettings mconfig;
        static QSettings &settings(){
            static QSettings settings("subsuite.ini", QSettings::IniFormat);
            return settings;
        }

    private:
        Config(Config const&) = delete;
        Config(Config &&) = delete;
        Config &operator=(Config const&) = delete;
        Config &operator=(Config &&) = delete;
};

#endif // CONFIG_H

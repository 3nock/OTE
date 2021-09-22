#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>

/*
 * A singleton class that saves & retrieves all the configuration
 * settings for all engines
 */

class Config {

    public:
        static QSettings &generalConfig(){
            static QSettings settings("subsuite.ini", QSettings::IniFormat);
            return settings;
        }
        static QSettings &osintConfig(){
            static QSettings settings("osintConfig.ini", QSettings::IniFormat);
            return settings;
        }
        static QSettings &bruteConfig(){
            static QSettings settings("bruteConfig.ini", QSettings::IniFormat);
            return settings;
        }
        static QSettings &projectConfig(){
            static QSettings settings("projectConfig.ini", QSettings::IniFormat);
            return settings;
        }

    private:
        Config(Config const&) = delete;
        Config(Config &&) = delete;
        Config &operator=(Config const&) = delete;
        Config &operator=(Config &&) = delete;
};

#endif // CONFIG_H

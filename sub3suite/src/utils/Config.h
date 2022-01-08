#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>


#define APIKEY Config::apiKeys()
#define CONFIG Config::general()
#define CONFIG_OSINT Config::osint()
#define CONFIG_RAW Config::raw()
#define CONFIG_BRUTE Config::brute()
#define CONFIG_ACTIVE Config::active()
#define CONFIG_DNS Config::dns()
#define CONFIG_SSL Config::ssl()
#define CONFIG_IP Config::ip()
#define CONFIG_PROJECT Config::project()

/* general values */
#define CFG_VAL_THREADS "threads"
#define CFG_VAL_TIMEOUT "timeout"
#define CFG_VAL_RECORD "record_type"
#define CFG_VAL_WILDCARD "use_wildcard"
#define CFG_VAL_LEVEL "level_scan"
#define CFG_VAL_MAXLEVEL "max_level"
#define CFG_VAL_DUPLICATES "duplicates"
#define CFG_VAL_AUTOSAVE "autosave"
#define CFG_VAL_SINGLE_NS "nameserver"
#define CFG_VAL_NAMESERVER "nameserver_type"
/* groups */
#define CFG_GRP_DEFAULT_NAMESERVERS "default_nameservers"
/* arrays */
#define CFG_ARR_CUSTOM_NAMESERVERS "custom_nameservers"
#define CFG_ARR_NAMESERVERS "nameservers"
/* wordlist */
#define CFG_WL_SUBDOMAIN "subdomain_wordlist"
#define CFG_WL_SUBDOMAIN_SPECIAL "subdomain_special_wordlist"
#define CFG_WL_TLD "tld_wordlist"
#define CFG_WL_TLD_SPECIAL "tld_special_wordlist"


/* a singleton class for accessing the .ini config files */
class Config
{
    public:
        static QSettings &apiKeys(){
            static QSettings settings("keys.ini", QSettings::IniFormat);
            return settings;
        }
        static QSettings &general(){
            static QSettings settings("sub3suite.ini", QSettings::IniFormat);
            return settings;
        }
        static QSettings &osint(){
            static QSettings settings("config/osint.ini", QSettings::IniFormat);
            return settings;
        }
        static QSettings &raw(){
            static QSettings settings("config/raw.ini", QSettings::IniFormat);
            return settings;
        }
        static QSettings &brute(){
            static QSettings settings("config/brute.ini", QSettings::IniFormat);
            return settings;
        }
        static QSettings &active(){
            static QSettings settings("config/active.ini", QSettings::IniFormat);
            return settings;
        }
        static QSettings &dns(){
            static QSettings settings("config/dns.ini", QSettings::IniFormat);
            return settings;
        }
        static QSettings &ssl(){
            static QSettings settings("config/ssl.ini", QSettings::IniFormat);
            return settings;
        }
        static QSettings &ip(){
            static QSettings settings("config/ip.ini", QSettings::IniFormat);
            return settings;
        }
        static QSettings &project(){
            static QSettings settings("config/project.ini", QSettings::IniFormat);
            return settings;
        }

    private:
        Config(Config const&) = delete;
        Config(Config &&) = delete;
        Config &operator=(Config const&) = delete;
        Config &operator=(Config &&) = delete;
};

#endif // CONFIG_H

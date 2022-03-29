/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : a singleton class for accessing(saving & loading) configuration settings from .ini files:
          keys.ini -> has apikeys
          sub3suite.ini -> all config values
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <QDebug>


#define APIKEY Config::apiKeys()
#define CONFIG Config::general()
#define DONORS Config::donors()
#define AUTHORS Config::authors()
#define MODULES Config::modules()

/* config groups */
#define CFG_OSINT "osint"
#define CFG_RAW "raw"
#define CFG_BRUTE "brute"
#define CFG_ACTIVE "active"
#define CFG_DNS "dns"
#define CFG_SSL "ssl"
#define CFG_URL "url"
#define CFG_ENUM "enums"

/* general group values */
#define CFG_VAL_THREADS "threads"
#define CFG_VAL_TIMEOUT "timeout"
#define CFG_VAL_RECORD "record"
#define CFG_VAL_WILDCARD "wildcard_scan"
#define CFG_VAL_DUPLICATES "no_duplicates"
#define CFG_VAL_AUTOSAVE "autosave_to_project"
#define CFG_VAL_NAMESERVER "nameserver_type"
#define CFG_VAL_BUILDDATE "build_date"
#define CFG_VAL_MAXPAGES "max_pages"
#define CFG_VAL_SETTIMEOUT "set_timeout"

/* groups */
#define CFG_GRP_RECENT "recent_projects"
#define CFG_GRP_DEFAULT_NS "default_nameservers"

/* arrays */
#define CFG_ARR_NAMESERVERS "nameservers"
#define CFG_ARR_CUSTOM_NS "custom_nameservers"

/* wordlist */
#define CFG_WL_SUBDOMAIN "subdomain_wordlist"
#define CFG_WL_SUBDOMAIN_SPECIAL "subdomain_special_wordlist"
#define CFG_WL_TLD "tld_wordlist"
#define CFG_WL_TLD_SPECIAL "tld_special_wordlist"
#define CFG_WL_SUBSTITUTIONS "wordlist_substitutions"

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
        static QSettings &donors(){
            static QSettings settings(":/files/res/files/DONORS", QSettings::IniFormat);
            return settings;
        }
        static QSettings &authors(){
            static QSettings settings(":/files/res/files/AUTHORS", QSettings::IniFormat);
            return settings;
        }
        static QSettings &modules(){
            static QSettings settings(":/files/res/files/MODULES", QSettings::IniFormat);
            return settings;
        }

    private:
        Config(Config const&) = delete;
        Config(Config &&) = delete;
        Config &operator=(Config const&) = delete;
        Config &operator=(Config &&) = delete;
};

#endif // CONFIG_H
